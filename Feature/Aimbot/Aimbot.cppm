#include "..//..//Depends/OS-ImGui/OS-ImGui.h"
#include <chrono>

export module Aimbot;
export import <unordered_map>;
export import <map>;
export import <windows.h>;
export import <cmath>;
export import <cstdlib>;
export import <ctime>;
export import <string>;
export import <format>;
export import <future>;
export import <thread>;

export import <mutex>;
export import PlayerManager;
export import Player;
export import Struct;
export import Hotkey;
export import UI;
export import Hotkey;
export import steamAvatarer;
static const float center_x = GetSystemMetrics(SM_CXSCREEN) / 2;
static const float center_y = GetSystemMetrics(SM_CYSCREEN) / 2;

std::mutex mtx;

export Player target;
export import DrawUtil;
export import <iostream>;
static LocalPlayer localPlayer;
static bool inArea(Vector2 player) {
	Vector2 d = player - Vector2{ center_x, center_y };
	return (d.x * d.x + d.y * d.y) <= (UI::Status::Aimbot::radius * UI::Status::Aimbot::radius); // 使用平方来避免浮点运算
}

static std::vector<Player> targets;

export{
	template <typename Key, typename Value>
	class ThreadSafeMap {
	private:
		std::map<Key, Value> map_;
		mutable std::mutex mutex_;

	public:
		void insert(const Key& key, const Value& value) {
			std::lock_guard<std::mutex> lock(mutex_);
			map_[key] = value;
		}

		bool try_insert(const Key& key, const Value& value) {
			std::unique_lock<std::mutex> lock(mutex_, std::try_to_lock);
			if (!lock.owns_lock()) {
				return false; // 获取锁失败
			}
			map_[key] = value;
			return true;
		}

		bool contains(const Key& key) const {
			std::lock_guard<std::mutex> lock(mutex_);
			return map_.find(key) != map_.end();
		}

		Value get(const Key& key) const {
			std::lock_guard<std::mutex> lock(mutex_);
			auto it = map_.find(key);
			if (it != map_.end()) {
				return it->second;
			}
		}
		void clear() {
			std::lock_guard<std::mutex> lock(mutex_);
            map_.clear();
		}
	};
}
export ThreadSafeMap<ull, void*>target_steamid_images;


namespace UIHelper {
	
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
		return ImColor(red,green,blue,1.0f);
	}
	static void DrawHealthBar(ImVec2 position, ImVec2 size, int currentHealth, int maxHealth) {
		// 确保最大血量大于0
		if (maxHealth <= 0) return;

		// 计算血量比例
		float healthRatio = static_cast<float>(currentHealth) / maxHealth;  // 确保浮点数除法
		healthRatio = healthRatio < 0 ? 0 : (healthRatio > 1 ? 1 : healthRatio);

		// 设置血条填充颜色 (从红色到绿色)
		ImU32 color = IM_COL32(255 * (1 - healthRatio), 255 * healthRatio, 0, 255);

		// 绘制背景 (边框内的区域)
		ImVec2 fillPos = position;
		ImVec2 fillSize = size;
		ImGui::GetBackgroundDrawList()->AddRectFilled(fillPos, ImVec2(fillPos.x + size.x, fillPos.y + size.y), IM_COL32(50, 50, 50, 255), 10.0f);

		// 绘制填充部分 (血量)
		ImVec2 healthBarPos = position;
		ImVec2 healthBarSize = ImVec2(size.x * healthRatio, size.y);
		ImGui::GetBackgroundDrawList()->AddRectFilled(healthBarPos, ImVec2(healthBarPos.x + healthBarSize.x, healthBarPos.y + healthBarSize.y), color, 10.0f);

		// 绘制边框
		ImGui::GetBackgroundDrawList()->AddRect(position, ImVec2(position.x + size.x, position.y + size.y), IM_COL32(255, 255, 255, 255), 10.0f, ImDrawFlags_RoundCornersAll, 2.0f);
	}
	static void drawAera() {
		Gui.Circle({ center_x ,center_y }, UI::Status::Aimbot::radius, getCycleColor(), 1.0f);
	}
	static void drawInfo() {
		std::string info = target.name;
		ImVec2 pos = { center_x + UI::Status::Aimbot::radius + 50, center_y - UI::Status::Aimbot::radius * 0.8f };
		int strlen = info.length();
		int steplen = strlen < 7 ? 23 : 15;
		ImVec2 size = { ((float)190+strlen* steplen),220};
		ImVec2 image_size = { 184,184 };
		ImVec2 image_pos = { pos.x + 5,pos.y + 5 };
		DrawUtil::DrawRoundedRect({center_x+UI::Status::Aimbot::radius +50,center_y- UI::Status::Aimbot::radius*0.8f}, size, getCycleColor(), 3.0f, 1.3f);
		void* image_ptr = target_steamid_images.get(target.steamID64);
		std::string hpinfo = "HP:" + std::to_string(target.health);
		std::string amorinfo = "Amor:"+std::to_string(target.armor);
		std::string disitanceinfo = "Dis:"+std::to_string((int)(target.position.distance(localPlayer.position)/10))+"m";
		
		
		Gui.Text(info.c_str(), { image_pos.x + image_size.x + 20,image_pos.y }, getCycleColor(), 40.0f);
		Gui.Text(hpinfo.c_str(), { image_pos.x + image_size.x + 20,image_pos.y + 30 }, getCycleColor(), 40.0f);
		Gui.Text(amorinfo.c_str(), { image_pos.x + image_size.x + 20,image_pos.y + 60 }, getCycleColor(), 40.0f);
		Gui.Text(disitanceinfo.c_str(), { image_pos.x + image_size.x + 20,image_pos.y + 90 }, getCycleColor(), 40.0f);
		DrawHealthBar({ image_pos.x,image_pos.y + image_size.y + 5 }, { size.x - 10,20 }, target.health, target.maxHealth);
		if (image_ptr!=nullptr) {
			Gui.DrawImage((ID3D11ShaderResourceView*)image_ptr, image_pos, image_size);
		}
	}
}
namespace mouseMoving {
	static void moveTo(int x, int y) {
		INPUT input = { 0 };
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_MOVE;
		input.mi.dx = x;
		input.mi.dy = y;

		// 发送输入事件
		SendInput(1, &input, sizeof(INPUT));
	}
	static void smoothMoveToTarget(int targetX, int targetY) {
		// 获取当前鼠标位置
		POINT currentPos;
		GetCursorPos(&currentPos);

		// 计算每个步骤的偏移量
		double stepX = static_cast<double>(targetX - currentPos.x) / UI::Status::Aimbot::smooth;
		double stepY = static_cast<double>(targetY - currentPos.y) / UI::Status::Aimbot::smooth;

		// 逐步移动鼠标
		for (int i = 0; i < UI::Status::Aimbot::smooth; ++i) {
			int moveX = static_cast<int>(stepX * (i + 1));
			int moveY = static_cast<int>(stepY * (i + 1));

			// 移动鼠标
			moveTo(moveX - static_cast<int>(stepX * i), moveY - static_cast<int>(stepY * i));

			// 延迟一段时间，控制移动速度
			if (UI::Status::Aimbot::deley)Sleep(1);
		}
	}
}
namespace selecter {
	//获取准星最近
	static Player getPlayer_CrossHair(const std::vector<Player>& players, view_matrix_t& viewMatrix) {
		Player nearestPlayer;
		float minDistance = 9999999999.9f;  // 初始为最大值

		// 遍历所有玩家，计算距离
		for (const auto& player : players) {
			Vector2 headPos{}; Vector3 headPosition_bone = ProcessUtils::rm<Vector3>(player.boneMatrix + BoneIndex::HEAD * 32);
			WorldToScreen(headPosition_bone, headPos, viewMatrix);
			float distance = headPos.distance(Vector2{ center_x ,center_y });
			if (distance < minDistance) {
				minDistance = distance;
				nearestPlayer = player; // 更新最近的玩家
			}
		}

		return nearestPlayer;
	}
	//获取距离最近的
	static Player getPlayer_Position(const std::vector<Player>& players, LocalPlayer& localPlayer) {
		float minDistance = (std::numeric_limits<float>::max)();
		float minHp = (std::numeric_limits<float>::max)();
		Player nearestPlayer;
		for (const Player& player : players) {
			auto disitance = player.position.distance(localPlayer.position);
			if (disitance < minDistance ) {
				// 更新最小距离
				minDistance = disitance;
				// 更新最近的玩家
				nearestPlayer = player;
			}
		}
		return nearestPlayer;
	}

	//获取血量最低
	static Player getPlayer_HP(const std::vector<Player>& players, LocalPlayer& localPlayer) {
		float minHp = (std::numeric_limits<float>::max)();
		Player nearestPlayer;
		for (const Player& player : players) {
			if (player.health < minHp) {
				// 更新最小距离
				minHp = player.health;
				// 更新最近的玩家
				nearestPlayer = player;
			}
		}
		return nearestPlayer;
	}
	static Player selectMode(UI::Status::Aimbot::AIMMODE& aimMode, const std::vector<Player>& players, LocalPlayer& localPlayer) {
		switch (aimMode) {
			case UI::Status::Aimbot::AIMMODE::CROSSHAIR: {
				return getPlayer_CrossHair(players, localPlayer.viewMatrix);
				break;
			}
			case UI::Status::Aimbot::AIMMODE::DISITANCE: {
				return getPlayer_Position(players, localPlayer);
				break;
			}
			case UI::Status::Aimbot::AIMMODE::HP: {
				return getPlayer_HP(players, localPlayer);
				break;
			}													
		}
	}
}

export namespace Aimbot {
	void enable(PlayerManager& pm) {
		
		UIHelper::drawAera();
		
		if (pm.players.empty())return;
		localPlayer = pm.getLocalPlayer();
		targets.clear();

		///获取一个合适的玩家
		for (Player player : pm.players) {
			Vector2 headPos{}; Vector3 headPosition_bone = ProcessUtils::rm<Vector3>(player.boneMatrix + BoneIndex::HEAD * 32);
			if (!WorldToScreen(headPosition_bone, headPos, localPlayer.viewMatrix))continue;
			if (UI::Status::Aimbot::visiableCheck && !player.visiable)continue;
			if (UI::Status::Aimbot::locklowHp) {
				if (player.health < UI::Status::Aimbot::lockMinHP && player.team!= localPlayer.team) {
					targets.push_back(player);
					break;
				}
			}
			if (inArea(headPos)) {
				///根据UI判断是否添加
				if ((UI::Status::Aimbot::lock_tammmate))targets.push_back(player);
				if (!UI::Status::Aimbot::lock_tammmate) {
					if (player.team != localPlayer.team)targets.push_back(player);
				}
			}
		}

		target= selecter::selectMode(UI::Status::Aimbot::aimMode, targets, localPlayer);
		Vector2 boxTopScreenPos, headbone_screenpos, bottomscreenPos{};
		float left, right;
		Vector3 headbone_worldPosition = ProcessUtils::rm<Vector3>(target.boneMatrix + BoneIndex::HEAD * 32);
		headbone_worldPosition = { headbone_worldPosition.x,headbone_worldPosition.y,headbone_worldPosition.z + 1 };
		if (WorldToScreen(headbone_worldPosition, headbone_screenpos, localPlayer.viewMatrix) && !(std::string_view{ target.name }.empty())) {
			auto color = UIHelper::getCycleColor();
			DrawUtil::draw2DBOX(target.position, localPlayer.viewMatrix, color, boxTopScreenPos, bottomscreenPos, left, right);
			UIHelper::drawInfo();
			if (UI::Status::Aimbot::lock) {
				mouseMoving::smoothMoveToTarget(headbone_screenpos.x, headbone_screenpos.y);
			}
		}
	}
}