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

		// ��ȡ��ǰ����״̬
		bool isKeyPressed = GetAsyncKeyState(VKKEY) & 0x8000;

		// ����ʱ����һ��
		if (isKeyPressed && !wasKeyPressed) {
			function();  // ����ʱ����
		}

		// �ɿ�ʱ����һ��
		if (!isKeyPressed && wasKeyPressed) {
			function();  // �ɿ�ʱ����
		}

		// ���°���״̬
		wasKeyPressed = isKeyPressed;
	}
	void bindHotkey_trigger_update(int vKey, std::function<void()> callback) {
		callbacks[vKey] = callback;
		keyStates[vKey] = false; // ��ʼ������״̬
	}

	void Update() {
		for (auto& pair : callbacks) {
			int vKey = pair.first;
			SHORT state = GetAsyncKeyState(vKey);
			bool isPressed = (state & 0x8000) != 0;

			// ��ⰴ��״̬�仯
			if (isPressed && !keyStates[vKey]) {
				pair.second(); // �����ص�
				keyStates[vKey] = true;
			}
			else if (!isPressed) {
				keyStates[vKey] = false;
			}
		}
	}
}