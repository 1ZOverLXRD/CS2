#include "Depends/OS-ImGui/OS-ImGui.h"
export module Cheater;

export import PlayerManager;

export import UI;
export import ProcessUtil;
export import PlayerManager;
export bool RUN = true;
export namespace Cheater {
	void logicThread(MThreadRenderer& render);
	void dispatch();
	void mainLoop();
}