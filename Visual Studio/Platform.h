#pragma once
#include "Component/SimpleScene.h"

class Platform
{
public:
	Platform(glm::vec3 position, glm::vec3 color, bool obstacle, bool power_ups);

	glm::vec3 position;
	glm::vec3 color;
	bool obstacle;
	bool power_up;
	bool collided_obstacle;
	bool obstacle_destroyed;
	bool type;
	float power_radius = 0.5;
	bool power_collected;
	
};

inline Platform::Platform(const glm::vec3 position, const glm::vec3 color, const bool obstacle, const bool power_up)
	: position(position), color(color), obstacle(obstacle), power_up(power_up), collided_obstacle(false),
	obstacle_destroyed(false), type(rand() % 2), power_collected(false) {}
