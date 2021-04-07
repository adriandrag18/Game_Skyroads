#pragma once
#include "Object.h"

class Balloon :public Object
{
public:
	float initial_x;
	bool burst = false;
	void Init(std::unordered_map<std::string, Mesh*>& meshes) const;
	explicit Balloon(glm::vec3 color, float sp);
	glm::mat3 Update(float deltaTimeSeconds);
	void Rise(float deltaTimeSeconds);
	void Fall(float deltaTimeSeconds);
	void Burst();
};
