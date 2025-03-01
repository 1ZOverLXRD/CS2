#include "..//..//Depends/OS-ImGui/OS-ImGui.h";

module Radar;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static ImDrawList* draw_list;;
static LocalPlayer localPlayer;
static const float SCREEN_X = GetSystemMetrics(SM_CXSCREEN);
static const float SCREEN_Y = GetSystemMetrics(SM_CYSCREEN);
static const ImVec2 radar_center{ SCREEN_X * 0.9f, SCREEN_Y * 0.17f };
namespace Radar {
	// 修改后的坐标转换函数（移除了距离限制）
	ImVec2 WorldToRadar(const Vector3& worldPos, const Vector3& localPos, const Vector2& localAngle) {
		Vector2 delta = {
		worldPos.x - localPos.x,  // 左右方向
		worldPos.y - localPos.y   // 前后方向
		};

		// 转换为弧度（注意符号处理）
		float yaw = (-localAngle.y + 45) * (M_PI / 180.0f);
		float cosYaw = std::cos(yaw);
		float sinYaw = std::sin(yaw);

		// 应用旋转矩阵（适应X=左右，Y=前后的坐标系）
		float rotatedX = delta.x * cosYaw - delta.y * sinYaw;
		float rotatedY = delta.x * sinYaw + delta.y * cosYaw;

		// 设置缩放比例（示例：1米=3像素）
		const float scale = RADAR_RADIUS / 5000.0f;  // 50米对应整个雷达半径

		return ImVec2(
			radar_center.x + rotatedY * scale,    // 屏幕X轴：右为正
			radar_center.y - rotatedX * scale      // 屏幕Y轴：上为正（需要取反）
		);
	}
	// 绘制函数保持相似，移除了距离限制相关代码
	void draw() {
		draw_list->AddCircleFilled(radar_center, RADAR_RADIUS, ImColor(20, 20, 20, 200));
		draw_list->AddCircle(radar_center, RADAR_RADIUS, ImColor(100, 100, 100, 255));

		// 添加方向指示（示例：北方标记）
		draw_list->AddLine(radar_center,
			{ radar_center.x, radar_center.y - 15 },
			ImColor(255, 255, 255, 200));
		draw_list->AddLine(radar_center, ImVec2(radar_center.x + RADAR_RADIUS, radar_center.y), ImColor(150, 150, 150, 100)); // 东
		draw_list->AddLine(radar_center, ImVec2(radar_center.x - RADAR_RADIUS, radar_center.y), ImColor(150, 150, 150, 100)); // 西
		draw_list->AddLine(radar_center, ImVec2(radar_center.x, radar_center.y + RADAR_RADIUS), ImColor(150, 150, 150, 100)); // 南
		draw_list->AddLine(radar_center, ImVec2(radar_center.x, radar_center.y - RADAR_RADIUS), ImColor(150, 150, 150, 100)); // 北
	}

	void enable(PlayerManager& pm) {
		draw_list = ImGui::GetBackgroundDrawList();
		// 获取雷达绘制区域参数
		draw();
		localPlayer = pm.getLocalPlayer();
		for (auto player : pm.players) {
			ImVec2 radarPos = WorldToRadar(player.position,localPlayer.position,localPlayer.viewAngle);
			// 简单可见性判断（可选）
			if (std::hypot(radarPos.x - radar_center.x, radarPos.y - radar_center.y) > RADAR_RADIUS + 5) {
				continue;  // 完全超出雷达范围的不绘制
			}
			ImColor color = player.team!= localPlayer.team ? ImColor(255, 0, 0, 255) : ImColor(0, 255, 0, 255);
			draw_list->AddCircleFilled(radarPos, 4.0f, color);

			// 添加方向指示器
			float angleRad = player.viewAngle.y * (M_PI / 180.0f);
			draw_list->AddLine(
				radarPos,
				{ radarPos.x + std::cos(angleRad) * 8, radarPos.y - std::sin(angleRad) * 8 },
				color,
				2.0f
			);
		}
	}
}