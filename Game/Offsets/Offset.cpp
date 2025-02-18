#include <curl/curl.h>
#include <spdlog/spdlog.h>
#include "..//..//Depends/rapidjson/document.h"
#include "..//..//Depends/rapidjson/error/en.h"
#include <iostream>
#include <fstream>

module Offseter;
import PConfig;

namespace FileHelper {
	// �Ա��ļ�
	bool compareFiles(const char* localFileName, const char* remoteFileName) {
		std::ifstream localFile(localFileName, std::ios::binary);
		std::ifstream remoteFile(remoteFileName, std::ios::binary);

		if (!localFile.is_open() || !remoteFile.is_open()) {
			return false;
		}

		return std::equal(
			std::istreambuf_iterator<char>(localFile),
			std::istreambuf_iterator<char>(),
			std::istreambuf_iterator<char>(remoteFile)
		);
	}

	// ��ȡ�ļ�
	char* readFile(const std::string& fileName) {
		std::ifstream file(fileName, std::ios::binary);
		if (!file) {
			spdlog::error("�޷����ļ�: {}", fileName);
			return nullptr;
		}

		file.seekg(0, std::ios::end);
		size_t length = file.tellg();
		file.seekg(0, std::ios::beg);

		char* buffer = new char[length + 1];
		file.read(buffer, length);
		buffer[length] = '\0';

		return buffer;
	}

	// ����ļ��Ƿ����
	bool fileExist(const std::string& fileName) {
		std::ifstream file(fileName);
		return file.is_open();
	}
}

namespace InternelHelper {
	// �ص���������д���ļ�
	size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
		size_t total_size = size * nmemb;
		std::ofstream& output_file = *static_cast<std::ofstream*>(userp);
		output_file.write(reinterpret_cast<char*>(contents), total_size);
		return total_size;
	}

	// �����ļ�
	bool download(const std::string& url, int timeout, const std::string& fileName) {
		CURL* curl;
		CURLcode res;

		curl_global_init(CURL_GLOBAL_DEFAULT);
		curl = curl_easy_init();
		if (!curl) {
			spdlog::error("CURL ��ʼ��ʧ�ܣ�");
			curl_global_cleanup();
			return false;
		}

		// ���ļ�
		std::ofstream output_file(fileName, std::ios::binary);
		if (!output_file.is_open()) {
			spdlog::error("�޷����ļ�����д�룺{}", fileName);
			curl_easy_cleanup(curl);
			curl_global_cleanup();
			return false;
		}

		// ���� CURL �������
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, timeout);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &output_file);

		// ִ������
		res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			spdlog::error("����ʧ��:{}��������Ϣ��{}", fileName, curl_easy_strerror(res));
			remove(fileName.c_str());
			curl_easy_cleanup(curl);
			curl_global_cleanup();
			return false;
		}

		spdlog::info("�ɹ������ļ�: {}", fileName);
		curl_easy_cleanup(curl);
		curl_global_cleanup();
		return true;
	}

	// �����ļ�������
	bool downloadWithRetry(const std::string& url, int timeout, const std::string& fileName) {
		bool success = false;
		while (!success) {
			success = download(url, timeout, fileName);
		}
		return success;
	}

	// �û���������������
	bool downloadWithRetry_User(const std::string& url, int timeout, const std::string& fileName) {
		bool success = false;
		while (!success) {
			success = download(url, timeout, fileName);
			if (!success) {
				spdlog::error("����ʧ�ܣ��Ƿ��������أ�(0: ������, 1: ��������):");
				std::string userChoice;
				std::cin >> userChoice;
				if (userChoice == "0") {
					return false;
				}
			}
		}
		return true;
	}

	// ����ȫ������ѡ���Ƿ�����
	bool downloadWithRetryConfig(const std::string& url, int timeout, const std::string& fileName) {
		return ALWAYRETRY ? downloadWithRetry(url, timeout, fileName) : downloadWithRetry_User(url, timeout, fileName);
	}

	// ����ͬ�� GitHub ����
	bool downloadSyncWithGithub(const std::string& url, const std::string& saveName) {
		if (FileHelper::fileExist(saveName)) {
			if (SYNCWITHGITHUBIOFFSET) {
				auto githubName = "github.txt";
				downloadWithRetryConfig(url, DOWNLOAD_TIMEOUT, saveName);
				return FileHelper::compareFiles(saveName.c_str(), githubName);
			}
			else {
				return true;
			}
		}
		else {
			return downloadWithRetryConfig(url, DOWNLOAD_TIMEOUT, saveName);
		}
	}
}

namespace JsonHelper {
	// ��������ֵ
	bool findIntValue(const rapidjson::Value& value, const std::string& fieldName, int& result) {
		if (value.IsObject()) {
			for (auto itr = value.MemberBegin(); itr != value.MemberEnd(); ++itr) {
				if (itr->name.GetString() == fieldName) {
					if (itr->value.IsInt()) {
						result = itr->value.GetInt();
						return true;
					}
				}
				else {
					if (findIntValue(itr->value, fieldName, result)) {
						return true;
					}
				}
			}
		}
		return false;
	}

	// ���� JSON �ַ�������ȡָ���ֶε� int ֵ
	bool parseJsonAndFindInt(const char* jsonStr, const std::string& fieldName, int& result) {
		rapidjson::Document document;
		document.Parse(jsonStr);
		if (document.HasParseError()) {
			spdlog::error("JSON ��������: {}", rapidjson::GetParseError_En(document.GetParseError()));
			return false;
		}
		return findIntValue(document, fieldName, result);
	}
}

namespace Offsets {
	// ���¼��ؿͻ���ƫ��
	static void reloadClient() {
		auto json_content = FileHelper::readFile(CLIENTDLL_JSON_FILENAME);
		int result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_iTeamNum", result)) Client::m_iTeamNum = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_iMaxHealth", result)) Client::m_iMaxHealth = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_steamID", result)) Client::m_steamID = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_iHealth", result)) Client::m_iHealth = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_ArmorValue", result)) Client::m_ArmorValue = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_hPlayerPawn", result)) Client::m_hPlayerPawn = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_flFlashBangTime", result)) Client::m_flFlashBangTime = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_iIDEntIndex", result)) Client::m_iIDEntIndex = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_iShotsFired", result)) Client::m_iShotsFired = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_aimPunchAngle", result)) Client::m_aimPunchAngle = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_vOldOrigin", result)) Client::m_vOldOrigin = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_vecViewOffset", result)) Client::m_vecViewOffset = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_pGameSceneNode", result)) Client::m_pGameSceneNode = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_bBombPlanted", result)) Client::m_bBombPlanted = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_bBombDropped", result)) Client::m_bBombDropped = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_nBombSite", result)) Client::m_nBombSite = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_bIsScoped", result)) Client::m_bIsScoped = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_sSanitizedPlayerName", result)) Client::m_sSanitizedPlayerName = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_fFlags", result)) Client::m_fFlags = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_vecAbsVelocity", result)) Client::m_vecAbsVelocity = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_vecAbsOrigin", result)) Client::m_vecAbsOrigin = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_hOwnerEntity", result)) Client::m_hOwnerEntity = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_iPing", result)) Client::m_iPing = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_bHasExploded", result)) Client::m_bHasExploded = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_bBeingDefused", result)) Client::m_bBeingDefused = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_flDefuseLength", result)) Client::m_flDefuseLength = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_bWarmupPeriod", result)) Client::m_bWarmupPeriod = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_bCTTimeOutActive", result)) Client::m_bCTTimeOutActive = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_bTerroristTimeOutActive", result)) Client::m_bTerroristTimeOutActive = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_pCameraServices", result)) Client::m_pCameraServices = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_iFOV", result)) Client::m_iFOV = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_bDormant", result)) Client::m_bDormant = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_pWeaponServices", result)) Client::m_pWeaponServices = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_hMyWeapons", result)) Client::m_hMyWeapons = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_AttributeManager", result)) Client::m_AttributeManager = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_Item", result)) Client::m_Item = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_iItemDefinitionIndex", result)) Client::m_iItemDefinitionIndex = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_MeshGroupMask", result)) Client::m_MeshGroupMask = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_nFallbackStatTrak", result)) Client::m_nFallbackStatTrak = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_nFallbackPaintKit", result)) Client::m_nFallbackPaintKit = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_nFallbackSeed", result)) Client::m_nFallbackSeed = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_flFallbackWear", result)) Client::m_flFallbackWear = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_pViewModelServices", result)) Client::m_pViewModelServices = result;
		if (JsonHelper::parseJsonAndFindInt(json_content, "m_hViewModel", result)) Client::m_hViewModel = result;
	}

	// ���¼���ƫ��
	static void reloadOffsetJson() {
		auto offset_json = FileHelper::readFile(OFFSET_JSON_FILENAME);
		rapidjson::Document document;
		if (document.Parse(offset_json).HasParseError()) {
			spdlog::error("JSON ��������ƫ���ļ�����ʧ��");
			return;
		}

		const auto& offset_clientDll = document["client.dll"];
		Offsets::ooffsseett::dwEntityList = offset_clientDll["dwEntityList"].GetInt64();
		Offsets::ooffsseett::dwLocalPlayerPawn = offset_clientDll["dwLocalPlayerPawn"].GetInt64();
		Offsets::ooffsseett::dwLocalPlayerController = offset_clientDll["dwLocalPlayerController"].GetInt64();
		Offsets::ooffsseett::dwViewAngles = offset_clientDll["dwViewAngles"].GetInt64();
		Offsets::ooffsseett::dwViewMatrix = offset_clientDll["dwViewMatrix"].GetInt64();
		Offsets::ooffsseett::dwSensitivity = offset_clientDll["dwSensitivity"].GetInt64();
		Offsets::ooffsseett::dwSensitivity_sensitivity = offset_clientDll["dwSensitivity_sensitivity"].GetInt64();
		Offsets::ooffsseett::dwGameRules = offset_clientDll["dwGameRules"].GetInt64();
		Offsets::ooffsseett::dwPlantedC4 = offset_clientDll["dwPlantedC4"].GetInt64();
		Offsets::ooffsseett::dwGlobalVars = offset_clientDll["dwGlobalVars"].GetInt64();
		Offsets::ooffsseett::dwWeaponC4 = offset_clientDll["dwWeaponC4"].GetInt64();
	}

	// ��������ƫ��
	bool downloadAllOffset() {
		bool cd = InternelHelper::downloadSyncWithGithub(CLIENTDLL_JSON_URL, CLIENTDLL_JSON_FILENAME);
		bool oj = InternelHelper::downloadSyncWithGithub(OFFSET_JSON_URL, OFFSET_JSON_FILENAME);
		return cd && oj;
	}

	// ��������ƫ��
	void updateAllOffset() {
		if (downloadAllOffset()) {
			reloadClient();
			reloadOffsetJson();
			spdlog::info("ƫ�Ƹ��³ɹ� ({}github)", SYNCWITHGITHUBIOFFSET ? "��ͬ��" : "δͬ��");
		}
	}

	// ��ʼ��ƫ��
	bool initOffset() {
		updateAllOffset();
		return true;
	}
}