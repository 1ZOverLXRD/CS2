#include <Windows.h>
#include <TlHelp32.h>
#include <spdlog/spdlog.h>
module ProcessUtil;

import <string>;

import PConfig;

namespace ProcessUtils {
	DWORD GetProcessIdByName(const std::string& processName) {
		// 创建进程快照
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hSnapshot == INVALID_HANDLE_VALUE) {
			return 0; // 无法创建快照
		}

		PROCESSENTRY32 pe;
		pe.dwSize = sizeof(PROCESSENTRY32);

		// 遍历所有进程
		if (Process32First(hSnapshot, &pe)) {
			do {
				// 比较进程名
				if (processName.compare(pe.szExeFile) == 0) {
					CloseHandle(hSnapshot);  // 关闭快照
					return pe.th32ProcessID;  // 返回进程的 PID
				}
			} while (Process32Next(hSnapshot, &pe));
		}

		CloseHandle(hSnapshot);  // 关闭快照
		return 0;  // 未找到进程
	}
	uintptr_t GetModuleBaseAddress(DWORD pid, const std::string& moduleName) {
		MODULEENTRY32 moduleEntry;
		HANDLE handle = NULL;
		handle = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid); //  获取进程快照中包含在th32ProcessID中指定的进程的所有的模块。
		if (!handle) {
			CloseHandle(handle);
			return NULL;
		}
		ZeroMemory(&moduleEntry, sizeof(MODULEENTRY32));
		moduleEntry.dwSize = sizeof(MODULEENTRY32);
		if (!Module32First(handle, &moduleEntry)) {
			CloseHandle(handle);
			return NULL;
		}
		do {
			if (moduleName.compare(moduleEntry.szModule) == 0) { return (uintptr_t)moduleEntry.hModule; }
		} while (Module32Next(handle, &moduleEntry));
		CloseHandle(handle);
		return 0;
	}
	bool TestGetModuleBaseAddress(DWORD pid)
	{
		//spdlog::debug("s:{} c:{}", moduleName, moduleEntry.szModule);
		return (GetModuleBaseAddress(pid, "client.dll") != 0) ? true : false;
	}
	void awaitProcess() {
		while ((GetProcessIdByName("cs2.exe")) == 0) {
			//spdlog::debug("等待游戏启动");
			Sleep(SLEEPTIME);
		}
	}
}