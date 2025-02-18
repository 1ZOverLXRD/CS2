export module BPlayer;
export import Struct;
export import Offseter;
export import gBase;
export import ProcessUtil;

#define assert_addres(x) {if(x==0||x<0){return false;}};
export class BPlayer {
protected:
	uintptr_t nameAddress;
	uintptr_t dw_Sensitivity;
public:

	uintptr_t boneMatrix;
	uintptr_t playerPawn;
	uintptr_t playerController;
	int index;
	DWORD64 bSpottedByMask;
	long steamId;
	unsigned long long steamID64;
	int team;
	bool visiable;
	int maxHealth;
	int health;
	int armor;
	char name[128] = { 0 };
	float sensitivity;
	Vector3 velocity;
	Vector3 position;
};