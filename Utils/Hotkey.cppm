export module Hotkey;
import <Windows.h>;
import <functional>;
import <unordered_map>;
static std::unordered_map<int, bool> keyStates;
static std::unordered_map<int, std::function<void()>> callbacks;
export namespace Hotkey {
	export namespace keys {
		int MENU = VK_INSERT;
		int UPDATE = VK_F1;;
		int ESP = VK_F2;
		int AIMBOT = VK_F3;
		int AIMBOT_LOCK = VK_LMENU;
	}
	void bindHotkey(int VKKEY, std::function<void()> function) {
		if (GetAsyncKeyState(VKKEY) & 1) {
			function();
		}
	}
	void bindHotkey_press(int VKKEY, std::function<void()> function) {
		if (GetAsyncKeyState(VKKEY) & 8001) {
			function();
		}
	}
	void bindHotkey_relase(int VKKEY, std::function<void()> function) {
		static bool wasKeyPressed = false;

		// 获取当前按键状态
		bool isKeyPressed = GetAsyncKeyState(VKKEY) & 0x8000;

		// 按下时触发一次
		if (isKeyPressed && !wasKeyPressed) {
			function();  // 按下时触发
		}

		// 松开时触发一次
		if (!isKeyPressed && wasKeyPressed) {
			function();  // 松开时触发
		}

		// 更新按键状态
		wasKeyPressed = isKeyPressed;
	}
	void bindHotkey_trigger_update(int vKey, std::function<void()> callback) {
		callbacks[vKey] = callback;
		keyStates[vKey] = false; // 初始化按键状态
	}

	void Update() {
		for (auto& pair : callbacks) {
			int vKey = pair.first;
			SHORT state = GetAsyncKeyState(vKey);
			bool isPressed = (state & 0x8000) != 0;

			// 检测按键状态变化
			if (isPressed && !keyStates[vKey]) {
				pair.second(); // 触发回调
				keyStates[vKey] = true;
			}
			else if (!isPressed) {
				keyStates[vKey] = false;
			}
		}
	}
}