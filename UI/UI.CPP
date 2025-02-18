#include "..//Depends/OS-ImGui/OS-ImGui.h"
module UI;
import Cheater;
ImVec2 initialPosition(0.0f, 0.0f);
ImVec2 windowSize(500.0f, 450.0f);
static int current_item = 1;
static int ui_aimmode;
namespace UI {
	void bindHotkey()
	{
		Hotkey::bindHotkey(Hotkey::keys::MENU, []() {UI::Status::MENU_OPEN = !UI::Status::MENU_OPEN; });
		Hotkey::bindHotkey(Hotkey::keys::UPDATE, []() {UI::Status::upDate = !UI::Status::upDate; });
		Hotkey::bindHotkey(Hotkey::keys::ESP, []() {UI::Status::ESP::enable = !UI::Status::ESP::enable; });
		Hotkey::bindHotkey(Hotkey::keys::AIMBOT, []() {UI::Status::Aimbot::enable = !UI::Status::Aimbot::enable; });
		Hotkey::bindHotkey(VK_MBUTTON, []() {
			// 切换aimmode
			current_item = (current_item == 1) ? 2 : 1;  // 切换在 DISTANCE (1) 和 HP (2) 之间
			UI::Status::Aimbot::aimMode = static_cast<UI::Status::Aimbot::AIMMODE>(current_item);
			});
		Hotkey::bindHotkey_press(VK_XBUTTON2, []() {UI::Status::Aimbot::radius += 40.0f; });//UP
		Hotkey::bindHotkey_press(VK_XBUTTON1, []() {UI::Status::Aimbot::radius -= 40.0f; });//UP
		Hotkey::bindHotkey_press(VK_PRIOR, []() {UI::Status::Aimbot::radius =600; });//UP
		Hotkey::bindHotkey_press(VK_NEXT, []() {UI::Status::Aimbot::radius -= 160; });//UP
		Hotkey::bindHotkey_relase(Hotkey::keys::AIMBOT_LOCK, []() {UI::Status::Aimbot::lock = !UI::Status::Aimbot::lock; });
		Hotkey::Update();
	}
	void drawMenu() {
		std::string fps = "fps:" + std::to_string(ImGui::GetIO().Framerate);
		std::string status = "status:"; status += UI::Status::status == true ? "ok" : "fail";
		std::string ecout = "enityCout:"; ecout += std::to_string(UI::Status::E_COUT);
		ImGui::SetNextWindowPos(initialPosition, ImGuiCond_Once);
		ImGui::SetNextWindowSize(windowSize, ImGuiCond_Once);
		
		if (Status::MENU_OPEN) {
			ImGui::Begin("1ZOVERLXRD", &Status::MENU_OPEN);

			ImGui::Checkbox("updatePlayer", &UI::Status::upDate);
			ImGui::SameLine();
			ImGui::SliderFloat("thickness", &UI::Status::ESP::thikness, 0, 5);
			ImGui::NextColumn();
			if (UI::Status::upDate) {
				///绘制功能tabBar
				if (ImGui::BeginTabBar("Modules")) {
					drawESP();
					drawAimbot();
				}
				ImGui::EndTabBar();
			}

			///界面状态
			ImGui::Text(fps.c_str());
			ImGui::Text(status.c_str());
			ImGui::Text(ecout.c_str());

			if (ImGui::Button("Quit")) { RUN=false; exit(0); }

			ImGui::End();
		}
	}
	void drawESP()
	{
		if (ImGui::BeginTabItem("ESP Settings"))
		{
			ImGui::Checkbox("enble", &Status::ESP::enable);
			if (Status::ESP::enable) {
				ImGui::Checkbox("line", &Status::ESP::line);
				ImGui::Checkbox("drawTeamate", &Status::ESP::drawTeammate);
				ImGui::SliderInt("maxDisplayer", &Status::ESP::maxDisplayer, 0, 64);
				ImGui::SliderFloat("maxDisitance", &Status::ESP::min_disitance, 0.0f, 10000.0f);
				ImGui::ColorEdit4("e_color", (float*)&Status::ESP::e_color);
				if (Status::ESP::drawTeammate)ImGui::ColorEdit4("t_color", (float*)&Status::ESP::t_color);
			}

			ImGui::Checkbox("singlePlayer", &Status::ESP::drawSinglePlayer);
			if (Status::ESP::drawSinglePlayer) {
				ImGui::InputText("playerId", Status::ESP::playerId, IM_ARRAYSIZE(Status::ESP::playerId), 0, 0);
				ImGui::SliderInt("warningDistance", &Status::ESP::warningDistance, 50, 10000);//警告距离
			}
			ImGui::EndTabItem();
		}
	}
	void drawAimbot() {
		if (ImGui::BeginTabItem("Aimbot Settings"))
		{
			ImGui::Checkbox("enble-[LAlt]", &Status::Aimbot::enable);
			ImGui::Checkbox("TrashvisiableCheck", &Status::Aimbot::visiableCheck);
			ImGui::Checkbox("trigger_bot", &Status::Aimbot::trigger_bot);
			ImGui::Checkbox("lock Teamate", &Status::Aimbot::lock_tammmate);
			
			if (ImGui::Combo("Aimode", &current_item, UI::Status::Aimbot::aimmode_item, IM_ARRAYSIZE(UI::Status::Aimbot::aimmode_item))) {
				UI::Status::Aimbot::aimMode = static_cast<UI::Status::Aimbot::AIMMODE>(current_item);
			}
			
			ImGui::Checkbox("lockLowHp", &Status::Aimbot::locklowHp);
			if (Status::Aimbot::locklowHp)ImGui::SliderInt("lock_minHp", &Status::Aimbot::lockMinHP, 1, 1000);

			ImGui::SliderInt("smooth", &Status::Aimbot::smooth, 1,100);//平滑
			ImGui::Checkbox("deley", &Status::Aimbot::deley);//平滑
			ImGui::SliderFloat("random", &Status::Aimbot::randomvalue, 0, 10);
			ImGui::SliderFloat("Radius[pgup-pgdown]", &Status::Aimbot::radius, 0, 600);
			ImGui::EndTabItem();
		}
	}
}