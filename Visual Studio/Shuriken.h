#pragma once
#include "Object.h"

class Shuriken : public Object
{
public:
	bool hit_player = false;

	void Init(std::unordered_map<std::string, Mesh*>& meshes) const;
	explicit Shuriken(glm::vec3 color, float sp);
	glm::mat3 Update(float deltaTimeSeconds);
};


