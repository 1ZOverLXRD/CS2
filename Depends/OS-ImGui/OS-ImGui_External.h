#pragma once
#include "OS-ImGui_Base.h"

/****************************************************
* Copyright (C)	: Liv
* @file			: OS-ImGui_External.h
* @author		: Liv
* @email		: 1319923129@qq.com
* @version		: 1.1
* @date			: 2024/4/4 14:12
****************************************************/
class MThreadRenderer
{
private:
    std::mutex m_mutex;
    ImDrawData m_drawData;

    std::vector<ImDrawList> m_drawLists;    // the thread render to these draw lists
    std::vector<ImDrawList> m_copyList;     // calling EndFrame() will copy the m_drawLists to this
    std::vector<ImDrawList*> m_copyPointer; // pointer to each drawlist in m_copyList, needed for m_drawData

    ImGuiViewportP* m_viewport;
public:

    // numDrawList = number of draw lists needed, 2 for background and foreground
    MThreadRenderer(size_t numDrawList = 2, ImGuiViewport* viewport = nullptr)
    {
        m_viewport = static_cast<ImGuiViewportP*>(viewport ? viewport : ImGui::GetMainViewport());

        m_copyList.resize(numDrawList, &GImGui->DrawListSharedData);

        for (size_t i = 0; i < numDrawList; i++)
        {
            m_drawLists.emplace_back(&GImGui->DrawListSharedData);
            m_copyPointer.emplace_back(&m_copyList[i]);
        }

        m_drawData.Valid = true;
        m_drawData.CmdLists = m_copyPointer.data();
        m_drawData.CmdListsCount = m_copyPointer.size();
    }

    // index=0 for the background drawlist, index=1 for the foreground, basically the z-index
    inline ImDrawList& GetDrawList(size_t index = 0) noexcept
    {
        return m_drawLists[index];
    }

    inline void BeginFrame()
    {
        for (auto& drawList : m_drawLists)
        {
            drawList._ResetForNewFrame();
            drawList.PushTextureID(GImGui->IO.Fonts->TexID);
            drawList.PushClipRect(m_viewport->Pos, { m_viewport->Pos.x + m_viewport->Size.x,m_viewport->Pos.y + m_viewport->Size.y}, false);
        }
    }

    inline void EndFrame()
    {
        m_mutex.lock();

        m_copyList = m_drawLists;

        m_drawData.TotalVtxCount = m_drawData.TotalIdxCount = 0;
        m_drawData.DisplayPos = m_viewport->Pos;
        m_drawData.DisplaySize = m_viewport->Size;
        m_drawData.FramebufferScale = GImGui->IO.DisplayFramebufferScale;

        for (auto& drawList : m_copyList)
        {
            drawList._PopUnusedDrawCmd();
            m_drawData.TotalVtxCount += drawList.VtxBuffer.Size;
            m_drawData.TotalIdxCount += drawList.IdxBuffer.Size;
        }

        m_mutex.unlock();
    }

    inline void Render() noexcept
    {
        m_mutex.lock();
        ImGui_ImplDX11_RenderDrawData(&m_drawData);
        m_mutex.unlock();
    }
};
namespace OSImGui
{
#ifndef OSIMGUI_INTERNAL

	class OSImGui_External : public OSImGui_Base
	{
	private:
        
		// 启动类型
		WindowType Type = NEW;
	public:
        ID3D11ShaderResourceView* LoadTextureFromFile_external(const char* filename, int& out_w, int& out_h);
		void NewWindow(std::string WindowName, Vec2 WindowSize, std::function<void()> CallBack, void* front_data_data=nullptr, int front_data_size=0);
		// 附加到另一个窗口上
		void AttachAnotherWindow(std::string DestWindowName, std::string DestWindowClassName, std::function<void()> CallBack, void* front_data_data=nullptr, int front_data_size=0);
		void AttachAnotherWindow(DWORD pid, std::function<void()> CallBack, void* front_data_data = nullptr, int front_data_size = 0);
		void AttachAnotherWindow(DWORD pid, std::function<void()> CallBack, std::function<void(MThreadRenderer&)> logicCallback, void* front_data_data = nullptr, int front_data_size = 0);
	private:
		void MainLoop();
		void MainLoop(MThreadRenderer& mthreadRenderer);
		bool UpdateWindowData();
		bool CreateMyWindow();

		bool PeekEndMessage();
	};

#endif
}