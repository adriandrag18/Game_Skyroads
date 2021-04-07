#include "Arrow.h"

#include <iostream>

#include "Player.h"

#include "Laboratoare/Laborator3/Transform2D.h"
#include "Core/Engine.h"
#include "Core/GPU/Mesh.h"

using namespace std;


Arrow::Arrow(glm::vec3 color, Player *player, float sp)
{

	transform = Transform(50, 300, 50, 50, 0);
	speed_x = 0;
	speed_y = 0;
	speedMultiplier = 5;
	SetSpeed(sp);
	this->player = player;
	this->color = color;
}


void Arrow::Init(std::unordered_map<std::string, Mesh*>& meshes) const
{
	for (const auto& item : colors)
	{
		auto color_str = item.first;
		const auto color = item.second;
		vector<VertexFormat> vertices;
		vector<unsigned short> indices;

		vertices.clear();
		vertices.emplace_back(glm::vec3(1.2f, 0, 0), color);
		vertices.emplace_back(glm::vec3(2.2f, 0, 0), color);
		indices.clear();
		indices = { 0, 1 };

		auto s = "arrow_body_" + color_str;
		meshes[s] = new Mesh(s);
		meshes[s]->InitFromData(vertices, indices);
		meshes[s]->SetDrawMode(GL_LINES);

		vertices.clear();
		vertices.emplace_back(glm::vec3(2.2f, -0.1f, 0), color);
		vertices.emplace_back(glm::vec3(2.4f, 0, 0), color);
		vertices.emplace_back(glm::vec3(2.2f, 0.1f, 0), color);
		indices.clear();
		indices = { 0, 1, 2 };

		s = "arrow_tip_" + color_str;
		meshes[s] = new Mesh(s);
		meshes[s]->InitFromData(vertices, indices);
		meshes[s]->SetDrawMode(GL_TRIANGLES);
	}
}

glm::mat3 Arrow::Update(float deltaTimeSeconds)
{
	time += deltaTimeSeconds;
	
	if (fired) {
		speed_y -= 196 * deltaTimeSeconds;
		transform.position_x += speed_x * deltaTimeSeconds;
		transform.position_y += speed_y * deltaTimeSeconds;
		transform.angle = atan(speed_y / speed_x) + (speed_x < 0 ? M_PI : 0) + (speed_x > 0 && speed_y < 0 ? 2 * M_PI : 0);
	}
	else
		transform = player->transform;
	
	glm::mat3 matrix(1);
	matrix *= Transform2D::Translate(transform.position_x, transform.position_y);
	matrix *= Transform2D::Rotate(transform.angle);
	matrix *= Transform2D::Scale(transform.scale_x, transform.scale_y);

	return matrix;
}

void Arrow::OnMouseBtnPress()
{
	if (fired)
		return;
	speed_x = time;
}

void Arrow::OnMouseBtnRelease()
{
	if (fired)
		return;

	speed_x = min(time - speed_x, 3.0f);
	speed_x *= speed;
	speed_y = speed_x * sin(player->transform.angle);
	speed_x = speed_x * cos(player->transform.angle);
	fired = true;
}
