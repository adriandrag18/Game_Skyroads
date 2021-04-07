#pragma once
class Transform
{
public:
	float position_x;
	float position_y;
	float scale_x;
	float scale_y;
	float angle;

	Transform() = default;
	
	Transform(const float position_x, const float position_y, const float scale_x, const float scale_y,
	          const float angle);

	Transform& operator=(const Transform& rhs) = default;

	~Transform() = default;
};

inline Transform::Transform(const float position_x, const float position_y, const float scale_x, const float scale_y,
                            const float angle):
	position_x(position_x), position_y(position_y), scale_x(scale_x), scale_y(scale_y), angle(angle)
{
}
