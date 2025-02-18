#include "..//..//Depends/OS-ImGui/OS-ImGui.h"

export module ESP;
export import Hotkey;

export import <algorithm>;
export import <windows.h>;
export import <format>;
export import <string>;
export import <string_view>;
export import PlayerManager;
export import UI;
export import Aimbot;

static const float center = (float)GetSystemMetrics(SM_CXSCREEN) / 2;
static ImColor getPlayerColor(const LocalPlayer& localPlayer, const Player& player) {
	bool isEnemy = localPlayer.team != player.team;
	if (isEnemy) {
		return ImColor{ UI::Status::ESP::e_color[0], UI::Status::ESP::e_color[1], UI::Status::ESP::e_color[2], UI::Status::ESP::e_color[3] };
	}
	return ImColor{ UI::Status::ESP::t_color[0], UI::Status::ESP::t_color[1], UI::Status::ESP::t_color[2], UI::Status::ESP::t_color[3] };
}
// 绘制玩家的信息
static void drawPlayerInfo(const Player& player, const LocalPlayer& localPlayer, const Vector2& headScreenPos, const Vector2& bottomscreenPos, float left, float right, const ImColor& color, int disitance) {
	if (player.bSpottedByMask & (DWORD64)1 << (localPlayerControllerIndex)
		||
		localPlayer.bSpottedByMask & (DWORD64)1 << player.index) {
		Gui.Text("Spotted", { headScreenPos.x, headScreenPos.y }, IM_COL32(255, 0, 0, 255), 20.0f);
	}
	std::string hadeInfo = std::format("{}:{}-[{}m]", player.index, player.name, disitance);
	std::string bottomInfo = std::format("[{}]---[{}]", player.health, player.armor);
	Gui.Text(bottomInfo.c_str(), { right, bottomscreenPos.y }, color, 20.0f);
	Gui.Text(hadeInfo.c_str(), { right, headScreenPos.y }, color, 20.0f);
	if (UI::Status::ESP::line) {
		Gui.Line({ center, 0 }, { headScreenPos.x, headScreenPos.y }, color, 1.0f);
	}
}
static void drawPlayerHeadCircle(const Vector2& boneheadScreen, const Vector2& neckScreen, const Vector2& headScreenPos, const ImColor& color) {
	Gui.Text("+", { boneheadScreen.x,boneheadScreen.y }, color,20.0f);
	
}

export namespace ESP {
	void enable(PlayerManager& pm) {
		if (pm.players.empty())return;
		LocalPlayer localPlayer = pm.getLocalPlayer();
		for (Player player : pm.players) {
			///初始化屏幕坐标和V3坐标
			Vector3 boneHeadPosition = ProcessUtils::rm<Vector3>(player.boneMatrix + BoneIndex::HEAD * 32);
			Vector3 neckPosition = ProcessUtils::rm<Vector3>(player.boneMatrix + BoneIndex::NECK * 32);
			Vector2 boneheadScreen, neckScreen, headScreenPos, bottomscreenPos;
			float left, right;
			// 距离
			int disitance = player.position.distance(localPlayer.position) / 10;
			// 是否敌人&队友
			bool isEnemy = localPlayer.team != player.team;
			bool isTeammate = UI::Status::ESP::drawTeammate && !isEnemy;
			// 绘制颜色
			ImColor color = getPlayerColor(localPlayer, player);
			if (WorldToScreen(boneHeadPosition, boneheadScreen, localPlayer.viewMatrix) && WorldToScreen(neckPosition, neckScreen, localPlayer.viewMatrix)) {
				if ((isEnemy || isTeammate) && disitance < UI::Status::ESP::min_disitance && target != player && DrawUtil::draw2DBOX(player.position, localPlayer.viewMatrix, color, headScreenPos, bottomscreenPos, left, right)) {
					
					// 绘制玩家信息
					drawPlayerInfo(player, localPlayer, headScreenPos, bottomscreenPos, left, right, color, disitance);
					// 绘制玩家圆形
					drawPlayerHeadCircle(boneheadScreen, neckScreen, headScreenPos, color);
				}
			}
		}
	}
	void drawSinglePlayer(PlayerManager& pm) {
		if (pm.players.empty())return;
		LocalPlayer localPlayer = pm.getLocalPlayer();
		for (Player player : pm.players) {
			if (std::string_view{std::to_string( player.index) }.compare(UI::Status::ESP::playerId) == 0) {
				Vector2 bottomscreenPos, headScreenPos;
				Vector3 headPosition = player.position + Vector3{ 0.0f, 0.0f, 72.0f };
				int disitance = player.position.distance(localPlayer.position) / 10;
				///ESP
				if (WorldToScreen(headPosition, headScreenPos, localPlayer.viewMatrix) && WorldToScreen(player.position, bottomscreenPos, localPlayer.viewMatrix))
				{
					// 计算框的尺寸
					float boxHeight = headScreenPos.y - bottomscreenPos.y;
					float boxWeight = boxHeight * 0.618;
					float left = bottomscreenPos.x - boxWeight / 2;
					float right = bottomscreenPos.x + boxWeight / 2;
					bool warningColor = disitance > UI::Status::ESP::warningDistance;
					ImColor color = warningColor ? ImColor{ 1.0f ,1.0f,0.0f, 1.0f } : ImColor{ UI::Status::ESP::t_color[0], UI::Status::ESP::t_color[1], UI::Status::ESP::t_color[2], UI::Status::ESP::t_color[3] };
					// 绘制矩形框
					std::string info = player.name; info += '['; info += std::to_string(disitance); info += "m]";
					Gui.Text(info, { right, bottomscreenPos.y }, color, 20.0f);
					Gui.Rectangle({ left, bottomscreenPos.y }, { boxWeight, boxHeight }, color, 1.0f);
					if (UI::Status::ESP::line) {
						Gui.Line({ center, 0 }, { headScreenPos.x, headScreenPos.y }, color, 1.0f);
					}
				}
				///警告部分
				if (disitance > UI::Status::ESP::warningDistance) {
					Gui.Text("WARNING!!!", { center, 150 }, ImColor{ 255, 0, 0 }, 30.0f, true);
				}
			}
		}
	}
}