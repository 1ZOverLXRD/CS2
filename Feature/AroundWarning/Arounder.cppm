#include "..//..//Depends/OS-ImGui/OS-ImGui.h"
export module Arounder;
export import PlayerManager;
export import Player;
export import UI;
static LocalPlayer localPlayer;

static auto RED = IM_COL32(255, 0, 0, 255);
static auto	GREEN= IM_COL32(0, 255, 0, 255);
static ImColor getCycleColor()
{
	static float cycleValue = 0.0f;  // 保存循环状态
	static float cycleSpeed = 0.00006f; // 循环速度

	// 更新循环值，并确保它在 [0, 1) 区间内
	cycleValue += cycleSpeed;
	if (cycleValue >= 1.0f) {
		cycleValue -= 1.0f;  // 如果超出1，重新从0开始
	}

	// 使用 sin 函数创建渐变效果，模拟 RGB 渐变
	float red = (sin(cycleValue * 2.0f * 3.14159f) + 1.0f) / 2.0f;   // Red
	float green = (sin((cycleValue + 0.33f) * 2.0f * 3.14159f) + 1.0f) / 2.0f; // Green
	float blue = (sin((cycleValue + 0.66f) * 2.0f * 3.14159f) + 1.0f) / 2.0f; // Blue
	return ImColor(red, green, blue, 1.0f);
}
export namespace Arounder {

	void enable(PlayerManager& manager) {
		localPlayer = manager.getLocalPlayer();
		int teammate_around50m=0;
		int enmy_around_less50m=0;
		for (auto& player : manager.players) {
			auto disitance = (int)player.position.distance(localPlayer.position) / 10;
			if (player.team == localPlayer.team) {
				if (disitance < 60) {
					teammate_around50m++;
				}
			}
			else {
                if (disitance < 60) {
					enmy_around_less50m++;
				}
			}
		}
		std::string less50m = "tm < 60m: " + std::to_string(teammate_around50m);
        std::string enmy_less50m = "em < 60m: " + std::to_string(enmy_around_less50m);

		std::string aimMode = "AimMode:"; aimMode += UI::Status::Aimbot::aimmode_item[static_cast<int>(UI::Status::Aimbot::aimMode)];
		
		Gui.Text(less50m.c_str(), { 980,500 }, GREEN,30.0f);	
		Gui.Text(enmy_less50m.c_str(), { 820,500 }, RED,30.0f);
		Gui.Text(aimMode.c_str(), {15,350}, getCycleColor(), 30.0f);
	}
}