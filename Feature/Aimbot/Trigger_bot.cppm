#include <Windows.h>
#include <WinUser.h>

export module  Triggerbot;

export import PlayerManager;
export import Player;

static LocalPlayer localPlayer;
static Player player;



static void LCLICK() {
	INPUT input = { 0 };
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &input, sizeof(INPUT));
	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &input, sizeof(INPUT));
}

export namespace Triggerbot {
	void enable(PlayerManager& pm) {
		localPlayer = pm.getLocalPlayer();
		int index = ProcessUtils::rm<int>(localPlayer.playerPawn + Offsets::Client::m_iIDEntIndex);
		if (index > 0) {
			uintptr_t entityListEntry = ProcessUtils::rm<uintptr_t>(entityList + 0x8 * (index >> 9) + 0x10);
			uintptr_t pawn = ProcessUtils::rm<uintptr_t>(entityListEntry + 120 * (index & 0x1FF));
			player.upDate(pawn);
			if (player.health > 0 && player.team != localPlayer.team){
				LCLICK();
			}
		}
	}
}