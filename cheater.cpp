#include "Depends/OS-ImGui/OS-ImGui.h"
module Cheater;


import ESP;
import Aimbot;
import Triggerbot;
import steamAvatarer;
import Arounder;
static PlayerManager pm = PlayerManager();
ThreadSafeMap<ull, bool> target_requests_code;
namespace Cheater {
	void dispatch() {
			if (UI::Status::upDate) {
				if (pm.upDate()) {
					Arounder::enable(pm);
					if (UI::Status::ESP::enable) {
						ESP::enable(pm);

					}
					if (UI::Status::ESP::drawSinglePlayer) {
						ESP::drawSinglePlayer(pm);
					}
					if (UI::Status::Aimbot::enable) {
						Aimbot::enable(pm);
					}
					UI::Status::E_COUT = pm.players.size();
					UI::Status::status = true;
				}
			}
			else {
				UI::Status::E_COUT = -1;
				UI::Status::status = false;
			}
	}
	void logicThread(MThreadRenderer& render) {
		int defaultw, deafulth;
		static ID3D11ShaderResourceView* defaultimg;
		while (true) {
			if(UI::Status::upDate) {
				if (target.health > 0) {
					std::string fileName;
					int w;
					int h;
					if (!target_requests_code.contains(target.steamID64)) {
						if (steamAvatarer::getImage(target.steamID64, fileName)) {
							if (target_steamid_images.get(target.steamID64) == nullptr) {
								ID3D11ShaderResourceView* image = Gui.LoadTextureFromFile(fileName.c_str(), w, h);
								if (image != nullptr) {
									target_steamid_images.insert(target.steamID64, (void*)image);
									target_requests_code.insert(target.steamID64, true);
								}
							}
						}else {
							if(!defaultimg)defaultimg= Gui.LoadTextureFromFile("Image\\default.jpg", defaultw, deafulth);
                            target_requests_code.insert(target.steamID64, false);
							target_steamid_images.insert(target.steamID64, (void*)defaultimg);
						}
					}

				}
			}
			else {
				target_steamid_images.clear();
				target_requests_code.clear();
			}
			Sleep(16);
		}
	}
	void mainLoop() {
		UI::drawMenu();
		UI::bindHotkey();
		dispatch();
	}
}