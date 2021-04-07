#pragma once
#include "Object.h"
#include "Player.h"

class Arrow :public Object
{
public:
	float speed_x;
	float speed_y;
	Player *player;
	bool fired = false;

	void Init(std::unordered_map<std::string, Mesh*>& meshes) const;
	explicit Arrow(glm::vec3 color, Player* player, float sp);
	glm::mat3 Update(float deltaTimeSeconds);
	void OnMouseBtnPress();
	void OnMouseBtnRelease();
};

