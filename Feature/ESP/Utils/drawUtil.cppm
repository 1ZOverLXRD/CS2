#include "..//..//..//Depends/OS-ImGui/OS-ImGui.h"

export module DrawUtil;
export import Struct;
export import UI;
export namespace DrawUtil {
	bool draw2DBOX(Vector3 position, view_matrix_t viewMatrix, ImColor color, Vector2& outHeadScreen, Vector2& bottom, float& left, float& right) {
		// 计算框的尺寸
		Vector2 headScreenPos, bottomscreenPos;

		Vector3 headPosition = position + Vector3{ 0.0f, 0.0, 72.0f };

		if (!WorldToScreen(headPosition, headScreenPos, viewMatrix))return false;
		if (!WorldToScreen(position, bottomscreenPos, viewMatrix))return false;

		float boxHeight = headScreenPos.y - bottomscreenPos.y;
		float boxWeight = boxHeight * 0.618;
		left = bottomscreenPos.x - boxWeight / 2;
		right = bottomscreenPos.x + boxWeight / 2;
		Gui.Rectangle({ left, bottomscreenPos.y }, { boxWeight, boxHeight }, color, UI::Status::ESP::thikness);
		outHeadScreen = headScreenPos;
		bottom = bottomscreenPos;
		return true;
	}
	bool draw2DBOX(Vector3 position, view_matrix_t viewMatrix, ImColor color) {
		// 计算框的尺寸
		Vector2 headScreenPos, bottomscreenPos;

		Vector3 headPosition = position + Vector3{ 0.0f, 0.0, 72.0f };

		if (!WorldToScreen(headPosition, headScreenPos, viewMatrix))return false;
		if (!WorldToScreen(position, bottomscreenPos, viewMatrix))return false;

		float boxHeight = headScreenPos.y - bottomscreenPos.y;
		float boxWeight = boxHeight * 0.618;
		float left = bottomscreenPos.x - boxWeight / 2;
		float right = bottomscreenPos.x + boxWeight / 2;

		Gui.Rectangle({ left, bottomscreenPos.y }, { boxWeight, boxHeight }, color, UI::Status::ESP::thikness);
		return true;
	}

	void DrawRoundedRect(ImVec2 pos, ImVec2 size, ImU32 col,float rounding , float thickness) {
        ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
        ImVec2 p_min = pos;
        ImVec2 p_max = ImVec2(pos.x + size.x, pos.y + size.y);
        int segments = 12;
        float angle_step = IM_PI / 2.0f / segments;

        // 左上角（逆时针180°~270°）
        for (int i = 0; i < segments; ++i) {
            float angle1 = IM_PI + i * angle_step; // 180°开始
            float angle2 = angle1 + angle_step;
            ImVec2 p1 = ImVec2(p_min.x + rounding + cosf(angle1) * rounding, p_min.y + rounding + sinf(angle1) * rounding);
            ImVec2 p2 = ImVec2(p_min.x + rounding + cosf(angle2) * rounding, p_min.y + rounding + sinf(angle2) * rounding);
            draw_list->AddLine(p1, p2, col, thickness);
        }

        // 右上角（顺时针270°~360°）
        for (int i = 0; i < segments; ++i) {
            float angle1 = 3 * IM_PI / 2 + i * angle_step; // 270°开始
            float angle2 = angle1 + angle_step;
            ImVec2 p1 = ImVec2(p_max.x - rounding + cosf(angle1) * rounding, p_min.y + rounding + sinf(angle1) * rounding);
            ImVec2 p2 = ImVec2(p_max.x - rounding + cosf(angle2) * rounding, p_min.y + rounding + sinf(angle2) * rounding);
            draw_list->AddLine(p1, p2, col, thickness);
        }

        // 左下角（逆时针90°~180°）
        for (int i = 0; i < segments; ++i) {
            float angle1 = IM_PI / 2 + i * angle_step; // 90°开始
            float angle2 = angle1 + angle_step;
            ImVec2 p1 = ImVec2(p_min.x + rounding + cosf(angle1) * rounding, p_max.y - rounding + sinf(angle1) * rounding);
            ImVec2 p2 = ImVec2(p_min.x + rounding + cosf(angle2) * rounding, p_max.y - rounding + sinf(angle2) * rounding);
            draw_list->AddLine(p1, p2, col, thickness);
        }

        // 右下角（顺时针0°~90°）
        for (int i = 0; i < segments; ++i) {
            float angle1 = 0 + i * angle_step; // 0°开始
            float angle2 = angle1 + angle_step;
            ImVec2 p1 = ImVec2(p_max.x - rounding + cosf(angle1) * rounding, p_max.y - rounding + sinf(angle1) * rounding);
            ImVec2 p2 = ImVec2(p_max.x - rounding + cosf(angle2) * rounding, p_max.y - rounding + sinf(angle2) * rounding);
            draw_list->AddLine(p1, p2, col, thickness);
        }

        // 绘制四条边
        draw_list->AddLine(ImVec2(p_min.x + rounding, p_min.y), ImVec2(p_max.x - rounding, p_min.y), col, thickness); // 上边
        draw_list->AddLine(ImVec2(p_min.x + rounding, p_max.y), ImVec2(p_max.x - rounding, p_max.y), col, thickness); // 下边
        draw_list->AddLine(ImVec2(p_min.x, p_min.y + rounding), ImVec2(p_min.x, p_max.y - rounding), col, thickness); // 左边
        draw_list->AddLine(ImVec2(p_max.x, p_min.y + rounding), ImVec2(p_max.x, p_max.y - rounding), col, thickness); // 右边
	}
}