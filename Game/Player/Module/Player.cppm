#define assert_addres(x) {if(x==0||x<0){return false;}};

export module Player;
import BPlayer;

export enum BoneIndex : int32_t
{
	HEAD = 6,
	NECK = 5,
	CHEST = 4,
	STOMACH = 2,
	COCK = 0,
	LSHOULDER = 8,
	LARM = 9,
	LHAND = 10,
	RSHOULDER = 13,
	RARM = 14,
	RHAND = 15,
	LTHIGH = 22,
	LCALF = 23,
	LFOOT = 24,
	RTHIGH = 25,
	RCALF = 26,
	RFOOT = 27,
};
export struct BoneConnection_Struct
{
	int bone1;
	int bone2;
	BoneConnection_Struct(int b1, int b2) : bone1(b1), bone2(b2) {}
};
export BoneConnection_Struct boneConnections[] = {
	BoneConnection_Struct(6, 5),  // head to neck
	BoneConnection_Struct(5, 4),  // neck to spine
	BoneConnection_Struct(4, 0),  // spine to hip
	BoneConnection_Struct(4, 8), // spine to left shoulder
	BoneConnection_Struct(8, 9), // left shoulder to left arm
	BoneConnection_Struct(9, 11), // arm to hand
	BoneConnection_Struct(4, 13),
	BoneConnection_Struct(13, 14),
	BoneConnection_Struct(14, 16),
	BoneConnection_Struct(4, 2),  // spine to spine_1
	BoneConnection_Struct(0, 22), // hip to left_hip
	BoneConnection_Struct(0, 25), // hip to right_hip
	BoneConnection_Struct(22, 23), // left_hip to left_knee
	BoneConnection_Struct(23, 24), // left knee to left foot
	BoneConnection_Struct(25, 26), // right_hip to right_knee
	BoneConnection_Struct(26, 27) // right knee to right foot
};

export class Player :public BPlayer {
public:
	bool upDate(uintptr_t currentPawn, uintptr_t currentController) {
		uintptr_t NameAddress = ProcessUtils::rm<uintptr_t>(currentController + Offsets::Client::m_sSanitizedPlayerName); assert_addres(NameAddress);
		ProcessUtils::readArray<char>(NameAddress, name, sizeof(name));
		team = ProcessUtils::rm<int>(currentPawn + Offsets::Client::m_iTeamNum);
		health = ProcessUtils::rm<int>(currentPawn + Offsets::Client::m_iHealth);
		maxHealth = ProcessUtils::rm<int>(currentPawn + Offsets::Client::m_iMaxHealth);
		armor = ProcessUtils::rm<int>(currentPawn + Offsets::Client::m_ArmorValue);
		position = ProcessUtils::rm<Vector3>(currentPawn + Offsets::Client::m_vOldOrigin);

		//BoneMatrix
		uintptr_t node = ProcessUtils::rm<uintptr_t>(currentPawn + Offsets::Client::m_pGameSceneNode); assert_addres(node); // m_pGameSceneNode
		boneMatrix = ProcessUtils::rm<uintptr_t>(node + 0x1F0); // m_groundNormal

		steamId = ProcessUtils::rm<long>(currentController + Offsets::Client::m_steamID);
		steamID64 = 76561197960265728ULL + static_cast<unsigned long long>(steamId);

		bSpottedByMask = ProcessUtils::rm<DWORD64>(currentPawn + Offsets::Client::bSpottedByMask);
		visiable = bSpottedByMask == 0 ? false : true;
		return true;
	}
	bool upDate(uintptr_t currentPawn) {
		team = ProcessUtils::rm<int>(currentPawn + Offsets::Client::m_iTeamNum);
		health = ProcessUtils::rm<int>(currentPawn + Offsets::Client::m_iHealth);
		maxHealth = ProcessUtils::rm<int>(currentPawn + Offsets::Client::m_iMaxHealth);
		armor = ProcessUtils::rm<int>(currentPawn + Offsets::Client::m_ArmorValue);
		position = ProcessUtils::rm<Vector3>(currentPawn + Offsets::Client::m_vOldOrigin);

		uintptr_t node = ProcessUtils::rm<uintptr_t>(currentPawn + Offsets::Client::m_pGameSceneNode); assert_addres(node); // m_pGameSceneNode
		boneMatrix = ProcessUtils::rm<uintptr_t>(node + 0x1F0); // m_groundNormal
		
		bSpottedByMask = ProcessUtils::rm<DWORD64>(currentPawn + Offsets::Client::bSpottedByMask); // m_groundNormal
		visiable = bSpottedByMask == 0 ? false : true;
		return true;
	}
	bool operator==(const Player& other) const {
		return other.position == this->position;
	}
	bool operator!=(const Player& other) const {
		return other.position != this->position;
	}
};