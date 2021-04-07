#pragma once
#include "Object.h"
#include "Player.h";

class PowerBar : public Object
{
public:
	Player* player;
	bool show = false;
	void Init(std::unordered_map<std::string, Mesh*>& meshes) const;
	explicit PowerBar(glm::vec3 color, Player* ply);
	glm::mat3 Update(float deltaTimeSeconds);
	void OnMouseBtnPress();
	void OnMouseBtnRelease();
};

