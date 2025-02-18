#include "Depends/simpleini/SimpleIni.h"
export module PConfig;//程序的配置

const std::string configFile = "pConfig.ini";

export int SLEEPTIME = 1000;//等待游戏启动

export int DOWNLOAD_TIMEOUT = 5L;//下载文件TIMEOUT

export bool SYNCWITHGITHUBIOFFSET = false;//是否同步github的偏移

export bool ALWAYRETRY = true;//总是重试

export std::string OFFSET_JSON_FILENAME = "offsets.json";//offset偏移文件
export std::string OFFSET_JSON_URL = "https://raw.githubusercontent.com/a2x/cs2-dumper/refs/heads/main/output/offsets.json";//offset偏移文件

export const std::string CLIENTDLL_JSON_FILENAME = "client_dll.json";//offset偏移文件
export const std::string CLIENTDLL_JSON_URL = "https://raw.githubusercontent.com/a2x/cs2-dumper/refs/heads/main/output/client_dll.json";//offset偏移文件

void createDefaultConfig(const std::string fileName) {
	// 创建 SimpleIni 对象
	CSimpleIniA ini;

	// 设置默认配置
	ini.SetValue("Settings", "SLEEPTIME", std::to_string(SLEEPTIME).c_str());
	ini.SetValue("Settings", "OFFSET_FILENAME", OFFSET_JSON_FILENAME.c_str());
	ini.SetValue("Settings", "OFFSETURL", OFFSET_JSON_URL.c_str());
	ini.SetValue("Settings", "DOWNLOAD_TIMEOUT", std::to_string(DOWNLOAD_TIMEOUT).c_str());
	ini.SetValue("Settings", "SYNCWITHGITHUBIOFFSET", SYNCWITHGITHUBIOFFSET ? "true" : "false");
	ini.SetValue("Settings", "ALWAYRETRY", ALWAYRETRY ? "true" : "false");

	// 保存配置到文件
	SI_Error rc = ini.SaveFile(fileName.c_str());
	if (rc < 0) {
		return;
	}
}

void readConfig(std::string fileName) {
	CSimpleIniA ini;
	SI_Error rc = ini.LoadFile(fileName.c_str());
	if (rc < 0) {
		createDefaultConfig(fileName);
	}

	// 读取配置项
	SLEEPTIME = std::stoi(ini.GetValue("Settings", "SLEEPTIME"));
	OFFSET_JSON_FILENAME = ini.GetValue("Settings", "OFFSET_FILENAME");
	OFFSET_JSON_URL = ini.GetValue("Settings", "OFFSETURL");
	DOWNLOAD_TIMEOUT = std::stoi(ini.GetValue("Settings", "DOWNLOAD_TIMEOUT"));
	std::string syncStr = ini.GetValue("Settings", "SYNCWITHGITHUBIOFFSET");
	SYNCWITHGITHUBIOFFSET = (syncStr == "true");
	std::string syncStr2 = ini.GetValue("Settings", "ALWAYRETRY");
	ALWAYRETRY = (syncStr2 == "true");
}

export void loadConfig() {
	readConfig(configFile);
}