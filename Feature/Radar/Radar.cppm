export module Radar;

export import PlayerManager;
export import UI;

export import <iostream>;

// 假设雷达窗口参数
export float RADAR_RADIUS = 150.0f;     // 雷达实际显示半径
export float SCALE = RADAR_RADIUS;
export namespace Radar {
	void enable(PlayerManager& pm);
}