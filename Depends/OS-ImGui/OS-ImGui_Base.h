#pragma once
#include "stb_image.h"
#include "OS-ImGui_Struct.h"
#include "OS-ImGui_Exception.hpp"
#include <iostream>
#include <string>
#include <functional>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <codecvt>
#include <vector>
#include <dwmapi.h>
#pragma comment(lib,"dwmapi.lib")

/****************************************************
* Copyright (C)	: Liv
* @file			: OS-ImGui_Base.h
* @author		: Liv
* @email		: 1319923129@qq.com
* @version		: 1.1
* @date			: 2024/4/4 13:59
****************************************************/

namespace OSImGui
{
	class D3DDevice
	{
	public:
		ID3D11Device* g_pd3dDevice = nullptr;
		ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
		IDXGISwapChain* g_pSwapChain = nullptr;
		ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
		bool CreateDeviceD3D(HWND hWnd);
		void CleanupDeviceD3D();
		void CreateRenderTarget();
		void CleanupRenderTarget();
		ID3D11ShaderResourceView* LoadTextureFromFile_base(const char* filename,int& out_w,int& out_h);
	};

	static D3DDevice g_Device;

	enum WindowType
	{
		NEW,
		ATTACH,
		INTERNAL
	};

	enum DirectXType
	{
		DX9,
		DX11,
		AUTO
	};

#if _WIN64
	typedef DWORD64 Address;
#else
	typedef DWORD Address;
#endif

	class WindowData
	{
	public:
		HWND  hWnd = NULL;
		HINSTANCE hInstance = nullptr;
		std::string Name;
		std::wstring wName;
		std::string ClassName;
		std::wstring wClassName;
		Vec2 Pos;
		Vec2 Size;
		ImColor BgColor{ 255, 255, 255 };
		bool Transparent = false;
	};

	class OSImGui_Base
	{
	public:
		// 回调函数
		std::function<void()> CallBackFn = nullptr;
		// 退出标识
		bool EndFlag = false;
	public:
		// 窗口数据
		WindowData Window;
		// 目标窗口数据
		WindowData DestWindow;
	public:
		// 创建一个新窗口
		virtual void NewWindow(std::string WindowName, Vec2 WindowSize, std::function<void()> CallBack) {};
		// 退出
		virtual void Quit() { EndFlag = true; };
	public:
		virtual bool CreateMyWindow() = 0;
		virtual void MainLoop() {};
		bool InitImGui(ID3D11Device* device, ID3D11DeviceContext* device_context);
		bool InitImGui(ID3D11Device* device, ID3D11DeviceContext* device_context, void* front_data_data, int front_data_size);
		void CleanImGui();
		std::wstring StringToWstring(std::string& str);
	};
}