#include <Windows.h>
#include <TlHelp32.h>
#include <spdlog/spdlog.h>
module ProcessUtil;

import <string>;

import PConfig;

namespace ProcessUtils {
	DWORD GetProcessIdByName(const std::string& processName) {
		// �������̿���
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hSnapshot == INVALID_HANDLE_VALUE) {
			return 0; // �޷���������
		}

		PROCESSENTRY32 pe;
		pe.dwSize = sizeof(PROCESSENTRY32);

		// �������н���
		if (Process32First(hSnapshot, &pe)) {
			do {
				// �ȽϽ�����
				if (processName.compare(pe.szExeFile) == 0) {
					CloseHandle(hSnapshot);  // �رտ���
					return pe.th32ProcessID;  // ���ؽ��̵� PID
				}
			} while (Process32Next(hSnapshot, &pe));
		}

		CloseHandle(hSnapshot);  // �رտ���
		return 0;  // δ�ҵ�����
	}
	uintptr_t GetModuleBaseAddress(DWORD pid, const std::string& moduleName) {
		MODULEENTRY32 moduleEntry;
		HANDLE handle = NULL;
		handle = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid); //  ��ȡ���̿����а�����th32ProcessID��ָ���Ľ��̵����е�ģ�顣
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
			//spdlog::debug("�ȴ���Ϸ����");
			Sleep(SLEEPTIME);
		}
	}
}