#include "OS-ImGui_Base.h"
//#include "..//..//UI/Font/SourceHanSansCN-Light.h"
/****************************************************
* Copyright (C)	: Liv
* @file			: OS-ImGui_Base.cpp
* @author		: Liv
* @email		: 1319923129@qq.com
* @version		: 1.1
* @date			: 2024/4/4 13:59
****************************************************/

namespace OSImGui
{
	bool OSImGui_Base::InitImGui(ID3D11Device* device, ID3D11DeviceContext* device_context)
	{
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		//io.Fonts->AddFontFromMemoryTTF((void*)front_data_data, front_data_size, 18.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());

		ImGui::StyleColorsDark();
		io.LogFilename = nullptr;

		if (!ImGui_ImplWin32_Init(Window.hWnd))
			throw OSException("ImGui_ImplWin32_Init() call failed.");
		if (!ImGui_ImplDX11_Init(device, device_context))
			throw OSException("ImGui_ImplDX11_Init() call failed.");

		return true;
	}
	bool OSImGui_Base::InitImGui(ID3D11Device* device, ID3D11DeviceContext* device_context,void* front_data_data, int front_data_size)
	{
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		if (front_data_data != nullptr && front_data_size != 0) { 
			io.Fonts->AddFontFromMemoryTTF((void*)front_data_data, front_data_size, 25.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull()); 
			io.Fonts->TexGlyphPadding = 1; // 减少纹理中字符之间的填充，可提高利用率
		}

		ImGui::StyleColorsDark();
		io.LogFilename = nullptr;

		if (!ImGui_ImplWin32_Init(Window.hWnd))
			throw OSException("ImGui_ImplWin32_Init() call failed.");
		if (!ImGui_ImplDX11_Init(device, device_context))
			throw OSException("ImGui_ImplDX11_Init() call failed.");

		return true;
	}

	void OSImGui_Base::CleanImGui()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		if (g_Device.g_pd3dDevice)
			g_Device.CleanupDeviceD3D();

		// Only destroy window in external mode.
		if (Window.hWnd && !Window.Name.empty())
		{
			DestroyWindow(Window.hWnd);
			UnregisterClassA(Window.ClassName.c_str(), Window.hInstance);
		}
	}

	std::wstring OSImGui_Base::StringToWstring(std::string& str)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		return converter.from_bytes(str);
	}
}