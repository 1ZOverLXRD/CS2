export module Offseter;

import <iostream>;
import <fstream>;

export enum OFFSET_CODE {
	OK,//成功
	FNE,//文件不存在
};

export namespace Offsets {
	export bool initOffset();
	export namespace Client {
		constexpr int bSpottedByMask = 0x23D0+0xC; ///entitySpottedState + bSpottedByMask
		int m_iMaxHealth = 0x340;
		int m_iTeamNum = 0x3E3; // uint8
		int m_iHealth = 0x344; // int32
		int m_ArmorValue = 0x2404; // int32
		int m_hPlayerPawn = 0x80C; // CHandle<C_CSPlayerPawn>
		int m_flFlashBangTime = 0x13F8; // float32
		int m_iIDEntIndex = 0x1458; // CEntityIndex
		int m_iShotsFired = 0x23E4; // int32
		int m_aimPunchAngle = 0x1584; // QAngle
		int m_vOldOrigin = 0x1324; // Vector
		int m_vecViewOffset = 0xCB0; // CNetworkViewOffsetVector
		int m_pGameSceneNode = 0x328; // CGameSceneNode*
		int m_bBombPlanted = 0x9A5; // bool
		int m_bBombDropped = 0x9A4; // bool
		int m_nBombSite = 0xF94; // int32
		int m_bIsScoped = 0x23D0; // bool
		int m_sSanitizedPlayerName = 0x770; // CUtlString
		int m_fFlags = 0x3EC; // uint32
		int m_vecAbsVelocity = 0x3F0; // Vector
		int m_vecAbsOrigin = 0xD0; // Vector
		int m_hOwnerEntity = 0x440; // CHandle<C_BaseEntity>
		int m_iPing = 0x740; // uint32
		int m_bHasExploded = 0xFC5; // bool
		int m_bBeingDefused = 0xFCC; // bool
		int m_flDefuseLength = 0xFDC; // float32
		int m_bWarmupPeriod = 0x41; // bool
		int m_bCTTimeOutActive = 0x4E; // bool
		int m_bTerroristTimeOutActive = 0x4D; // bool
		int m_pCameraServices = 0x11E0; // (CPlayer_CameraServices*)
		int m_iFOV = 0x210; // uint32
		int m_bDormant = 0xEF; // bool
		int m_pWeaponServices = 0x11A8; // CPlayer_WeaponServices*
		int m_hMyWeapons = 0x40; // C_NetworkUtlVectorBase<CHandle<C_Base
		int m_AttributeManager = 0x1148; // C_AttributeContainer
		int m_Item = 0x50; // C_EconItemView
		int m_iItemDefinitionIndex = 0x1BA; // uint16
		int m_MeshGroupMask = 0x198; // uint64
		int m_nFallbackStatTrak = 0x1604; // int32
		int m_nFallbackPaintKit = 0x15F8; // int32
		int m_nFallbackSeed = 0x15FC; // int32
		int m_flFallbackWear = 0x1600; // float32
		int m_pViewModelServices = 0x1368; // CPlayer_ViewModelServices*
		int m_hViewModel = 0x40; // CHandle<C_BaseViewModel>[3]
		int m_steamID = 0x6E8;//CBasePlayerController
	}
	export namespace ooffsseett {///Offsets.json
		uintptr_t dwEntityList = 0x19CA858; // uintptr_t
		uintptr_t dwLocalPlayerPawn = 0x182FAD8; // uintptr_t
		uintptr_t dwLocalPlayerController = 0x1A1A6C0; // uintptr_t
		uintptr_t dwViewAngles = 0x1A36990; // uintptr_t
		uintptr_t dwViewMatrix = 0x1A2CB00; // uintptr_t
		uintptr_t dwSensitivity = 0x1A29158; // uintptr_t
		uintptr_t dwSensitivity_sensitivity = 0x40; // uintptr_t
		uintptr_t dwGameRules = 0x1A28438; // uintptr_t
		uintptr_t dwPlantedC4 = 0x1A32070; // uintptr_t
		uintptr_t dwGlobalVars = 0x1823C50; // uintptr_t
		uintptr_t dwWeaponC4 = 0x19CDC50;  // uintptr_t
	}
}