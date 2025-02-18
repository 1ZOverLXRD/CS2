export module Struct;

import <Windows.h>;

#define SCREEN_WIDTH GetSystemMetrics(SM_CXSCREEN)
#define SCREEN_HEIGHT GetSystemMetrics(SM_CYSCREEN)

export import <cstdint>;
export import <cmath>;
export import <numbers>;

export struct Vector3 {
	float x, y, z;

	// 默认构造函数
	Vector3() : x(0), y(0), z(0) {}

	// 带参数的构造函数
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

	// 向量加法
	Vector3 operator+(const Vector3& other) const {
		return Vector3(x + other.x, y + other.y, z + other.z);
	}

	// 向量减法
	Vector3 operator-(const Vector3& other) const {
		return Vector3(x - other.x, y - other.y, z - other.z);
	}
	bool operator==(const Vector3& other) const {
		return x == other.x && y == other.y && z == other.z;
	}
	// 计算距离
	float distance(const Vector3& other) const {
		return std::sqrt((x - other.x) * (x - other.x) +
			(y - other.y) * (y - other.y) +
			(z - other.z) * (z - other.z));
	}
	Vector3 RelativeAngle()
	{
		return {
			std::atan2(-z, std::hypot(x, y)) * (180.0f / std::numbers::pi_v<float>),
			std::atan2(y, x) * (180.0f / std::numbers::pi_v<float>),
			0.0f
		};
	}

	inline float AngleDifference(const Vector3& other) const
	{
		auto deltaPitch = std::fabs(x - other.x);
		auto deltaYaw = std::fabs(y - other.y);

		if (deltaPitch > 180.0f)
			deltaPitch = 360.0f - deltaPitch;

		if (deltaYaw > 180.0f)
			deltaYaw = 360.0f - deltaYaw;

		return std::sqrt(deltaPitch * deltaPitch + deltaYaw * deltaYaw);
	}

	float Magnitude() {
		return sqrt(x * x + y * y + z * z);
	}

	const Vector3 ToAngle() const noexcept
	{
		return Vector3{
			std::atan2(-z, std::hypot(x, y)) * (180.0f / std::numbers::pi_v<float>),
			std::atan2(y, x) * (180.0f / std::numbers::pi_v<float>),
			0.0f };
	}

	const bool IsZero() const noexcept
	{
		return x == 0.f && y == 0.f && z == 0.f;
	}
};
export struct Vector2 {
	float x, y;

	// 默认构造函数
	Vector2() : x(0), y(0) {}

	// 带参数的构造函数
	Vector2(float x, float y) : x(x), y(y) {}

	// 向量加法
	Vector2 operator+(const Vector2& other) const {
		return Vector2(x + other.x, y + other.y);
	}

	// 向量减法
	Vector2 operator-(const Vector2& other) const {
		return Vector2(x - other.x, y - other.y);
	}

	// 计算距离
	float distance(const Vector2& other) const {
		return std::sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
	}
	float Magnitude() const {
		return std::sqrt(x * x + y * y);
	}
};
export struct Vector4
{
	float w, x, y, z;
};
export struct view_matrix_t
{
	float matrix[4][4];
};
export bool WorldToScreen(const Vector3& worldPos, Vector2& screenPos, const view_matrix_t& viewMatrix)
{
	float clip_x = worldPos.x * viewMatrix.matrix[0][0] + worldPos.y * viewMatrix.matrix[0][1] + worldPos.z * viewMatrix.matrix[0][2] + viewMatrix.matrix[0][3];
	float clip_y = worldPos.x * viewMatrix.matrix[1][0] + worldPos.y * viewMatrix.matrix[1][1] + worldPos.z * viewMatrix.matrix[1][2] + viewMatrix.matrix[1][3];
	float clip_z = worldPos.x * viewMatrix.matrix[2][0] + worldPos.y * viewMatrix.matrix[2][1] + worldPos.z * viewMatrix.matrix[2][2] + viewMatrix.matrix[2][3];
	float clip_w = worldPos.x * viewMatrix.matrix[3][0] + worldPos.y * viewMatrix.matrix[3][1] + worldPos.z * viewMatrix.matrix[3][2] + viewMatrix.matrix[3][3];

	if (clip_w < 0.1f) // To prevent division by zero and culling objects behind the camera
		return false;

	// Normalize to NDC (Normalized Device Coordinates)
	float ndc_x = clip_x / clip_w;
	float ndc_y = clip_y / clip_w;

	// Convert to screen coordinates
	screenPos.x = (SCREEN_WIDTH / 2 * ndc_x) + (ndc_x + SCREEN_WIDTH / 2);
	screenPos.y = -(SCREEN_HEIGHT / 2 * ndc_y) + (ndc_y + SCREEN_HEIGHT / 2);

	return true;
}
export bool WorldToScreen(const Vector3& worldPos, Vector2& screenPos, const view_matrix_t& viewMatrix, int window_width, int window_height)
{
	float clip_x = worldPos.x * viewMatrix.matrix[0][0] + worldPos.y * viewMatrix.matrix[0][1] + worldPos.z * viewMatrix.matrix[0][2] + viewMatrix.matrix[0][3];
	float clip_y = worldPos.x * viewMatrix.matrix[1][0] + worldPos.y * viewMatrix.matrix[1][1] + worldPos.z * viewMatrix.matrix[1][2] + viewMatrix.matrix[1][3];
	float clip_z = worldPos.x * viewMatrix.matrix[2][0] + worldPos.y * viewMatrix.matrix[2][1] + worldPos.z * viewMatrix.matrix[2][2] + viewMatrix.matrix[2][3];
	float clip_w = worldPos.x * viewMatrix.matrix[3][0] + worldPos.y * viewMatrix.matrix[3][1] + worldPos.z * viewMatrix.matrix[3][2] + viewMatrix.matrix[3][3];

	if (clip_w < 0.1f) // To prevent division by zero and culling objects behind the camera
		return false;

	// Normalize to NDC (Normalized Device Coordinates)
	float ndc_x = clip_x / clip_w;
	float ndc_y = clip_y / clip_w;

	// Convert to screen coordinates
	screenPos.x = (window_width / 2 * ndc_x) + (ndc_x + SCREEN_WIDTH / 2);
	screenPos.y = -(window_height / 2 * ndc_y) + (ndc_y + SCREEN_HEIGHT / 2);

	return true;
}