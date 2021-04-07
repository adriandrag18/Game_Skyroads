#pragma once
#include "Component/SimpleScene.h"
#include "Transform.h"

class Object
{
public:
	Transform transform = Transform();
	glm::vec3 color;
	float speed = 0;
	float time = 0;
	float speedMultiplier;
	
	void SetSpeed(float new_speed)
	{
		speed = speedMultiplier * new_speed;
	}

	const glm::vec3 red = glm::vec3(1, 0, 0);
	const glm::vec3 green = glm::vec3(0, 1, 0);
	const glm::vec3 blue = glm::vec3(0, 0, 1);
	const glm::vec3 yellow = glm::vec3(1, 1, 0);
	const glm::vec3 black = glm::vec3(0, 0, 0);
	const glm::vec3 white = glm::vec3(1, 1, 1);
	const std::unordered_map<std::string, glm::vec3> colors = 
		{ {"red", red}, {"green", green}, {"blue", blue}, {"yellow", yellow}, {"black", black}, {"white", white} };
};
