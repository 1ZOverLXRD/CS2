export module UI;
import Hotkey;
export namespace UI {
	export namespace Status {
		bool MENU_OPEN = true;
		int E_COUT = 0;//��������
		bool status = true;//����״̬
		bool upDate = true;//ˢ������

		export namespace ESP {
			bool enable = false;
			
			bool drawTeammate = false;
			bool line = false;
			int maxDisplayer = 64;
			bool drawSinglePlayer = false;
			int warningDistance = 50;
			char playerId[256] = "";//��������
			float thikness = 1.25f;
			float e_color[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
			float t_color[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
			float min_disitance = 1000;
		}
		export namespace Aimbot {
			export enum AIMMODE {
				CROSSHAIR = 0,
				DISITANCE = 1,
				HP = 2,
			};

			bool deley = false;
			bool enable = false;
			bool trigger_bot = false;
			bool lock = false;
			bool visiableCheck = true;//�������ʧЧ����һ��bS...�Ż�ˢ��
			bool lock_tammmate = false;

			const char* aimmode_item[] = { "CROSSHIRE", "DISITANCE","HP"};
			AIMMODE aimMode = AIMMODE::DISITANCE;

			float radius = 150;

			float randomvalue = 0;

			int lockMinHP = 200;
			bool locklowHp = false;

			int smooth = 3;
		}
	}
	void bindHotkey();
	void drawMenu();
	void drawESP();
	void drawAimbot();
}