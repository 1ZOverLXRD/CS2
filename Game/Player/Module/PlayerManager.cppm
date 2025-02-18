#include <iomanip>

export module PlayerManager;
export import localPlayer;
export import Player;
export import <string_view>;
export import <vector>;
export import <Windows.h>;
export import <iostream>;

export int localPlayerControllerIndex = 0;
#define assert_addres(x) if(!x)continue;
export class PlayerManager {
private:
	LocalPlayer localPlayer = LocalPlayer();

public:
	std::vector<Player> players = std::vector<Player>(64);
	LocalPlayer getLocalPlayer() {
		return localPlayer;
	}
	bool upDateOtherPlayer() {
		players.clear();

		entityList = ProcessUtils::rm<uintptr_t>(clientDll + Offsets::ooffsseett::dwEntityList);
		for (int i = 1; i < 64; i++)
		{
			auto player = Player();
			uintptr_t listEntry = ProcessUtils::rm<uintptr_t>(entityList + (8 * (i & 0x7FFF) >> 9) + 16); assert_addres(listEntry);
			uintptr_t currentController = ProcessUtils::rm<uintptr_t>(listEntry + 120 * (i & 0x1FF)); assert_addres(currentController);

			uint32_t pawnHandle = ProcessUtils::rm<uint32_t>(currentController + Offsets::Client::m_hPlayerPawn); assert_addres(pawnHandle);
			uintptr_t listEntry2 = ProcessUtils::rm<uintptr_t>(entityList + 0x8 * ((pawnHandle & 0x7FFF) >> 9) + 16); assert_addres(listEntry2); assert_addres(listEntry2);
			uintptr_t currentPawn = ProcessUtils::rm<uintptr_t>(listEntry2 + 120 * (pawnHandle & 0x1FF)); assert_addres(currentPawn);
			if (localPlayer.playerController == currentController) {
				localPlayerControllerIndex = i;
				continue;
			}
			if (localPlayer.playerPawn == currentPawn)continue;
			if (player.upDate(currentPawn, currentController)) {
				if (player.health > 0 && player.steamId!=0 && (player.team==2||player.team==3)) {
					player.index = i;
					players.push_back(player);
				}
			}
		}
		return true;
	}
	bool upDate() {
		auto result = localPlayer.upDate();
		return result && upDateOtherPlayer();
	}
};