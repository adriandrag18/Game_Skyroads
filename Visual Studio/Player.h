#pragma once
#include "Object.h"

class Player final : public Object
{
public:
	void Init(std::unordered_map<std::string, Mesh*>& meshes) const;
	explicit Player(glm::vec3 color, float sp);
	glm::mat3 Update(float deltaTimeSeconds);
	void OnMouseMove(int mouseX, int mouseY);
};

