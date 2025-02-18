#include <spdlog/spdlog.h>

module gBase;

import Cheater;

DWORD pid = 0;
uintptr_t clientDll = 0;
uintptr_t serverDll = 0;
uintptr_t engineDll = 0;
uintptr_t entityList = 0;
bool initBase() {
	ProcessUtils::awaitProcess();
	pid = ProcessUtils::GetProcessIdByName("cs2.exe");
	///卡在出现选择国服或者国际服界面
	while (ProcessUtils::TestGetModuleBaseAddress(pid) == false) {
		spdlog::warn("CS2 has started but hasn\'t fully launched");//卡在选择国服或者国际服界面
		Sleep(SLEEPTIME);
	}
	ProcessUtils::attachProcess(pid);
	clientDll = ProcessUtils::GetModuleBaseAddress(pid, "client.dll");
	serverDll = ProcessUtils::GetModuleBaseAddress(pid, "server.dll");
	engineDll = ProcessUtils::GetModuleBaseAddress(pid, "engine2.dll");
	if (clientDll == 0 || serverDll == 0 || engineDll == 0) {
		spdlog::critical("Failed to get client.dll,server.dll,engine.dll base address");
		return false;
	}

	spdlog::trace("PID       :{}", pid);
	spdlog::trace("clientBase:{:X}", clientDll);
	spdlog::trace("serverBase:{:X}", serverDll);
	spdlog::trace("engineBase:{:X}", engineDll);
	return true;
}