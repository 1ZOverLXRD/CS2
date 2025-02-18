export module localPlayer;

import BPlayer;

#define assert_addres(x) {if(x==0||x<0){return false;}};

export class LocalPlayer :public BPlayer
{
public:
	view_matrix_t viewMatrix;
	LocalPlayer() {};
	bool upDate() {

		

		viewMatrix = ProcessUtils::rm<view_matrix_t>(clientDll + Offsets::ooffsseett::dwViewMatrix);
		playerPawn = ProcessUtils::rm<uintptr_t>(clientDll + Offsets::ooffsseett::dwLocalPlayerPawn); assert_addres(playerPawn);
		playerController = ProcessUtils::rm<uintptr_t>(clientDll + Offsets::ooffsseett::dwLocalPlayerController); assert_addres(playerController);

		dw_Sensitivity = ProcessUtils::rm<uintptr_t>(clientDll + Offsets::ooffsseett::dwSensitivity); // private
		nameAddress = ProcessUtils::rm<uintptr_t>(playerController + Offsets::Client::m_sSanitizedPlayerName); assert_addres(nameAddress);

		team = ProcessUtils::rm<int>(playerPawn + Offsets::Client::m_iTeamNum);
		health = ProcessUtils::rm<int>(playerPawn + Offsets::Client::m_iHealth);
		maxHealth = ProcessUtils::rm<int>(playerPawn + Offsets::Client::m_iMaxHealth);

		armor = ProcessUtils::rm<int>(playerPawn + Offsets::Client::m_ArmorValue);
		ProcessUtils::readArray<char>(nameAddress, name, sizeof(name)); // Name TODO
		velocity = ProcessUtils::rm<Vector3>(playerPawn + Offsets::Client::m_vecAbsVelocity);
		position = ProcessUtils::rm<Vector3>(playerPawn + Offsets::Client::m_vOldOrigin);
		steamId = ProcessUtils::rm<long>(playerController + Offsets::Client::m_steamID);
		steamID64 = 76561197960265728ULL + static_cast<unsigned long long>(steamId);
		bSpottedByMask= ProcessUtils::rm<DWORD64>(playerPawn + Offsets::Client::bSpottedByMask);
		return true;
	}
};