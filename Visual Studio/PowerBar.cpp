#include "PowerBar.h"

#include <iostream>


#include "Core/GPU/Mesh.h"
#include "Laboratoare/Laborator3/Transform2D.h"

using namespace std;

PowerBar::PowerBar(glm::vec3 color, Player* ply)
{
	player = ply;
	transform = Transform(player->transform.position_x, player->transform.position_y - 100, 10, 10, 0);
	speed = 100;
	this->color = color;
}

void PowerBar::Init(std::unordered_map<std::string, Mesh*>& meshes) const
{
	for (const auto& item : colors) {
		auto color_str = item.first;
		const auto color = item.second;
		vector<VertexFormat> vertices = {
		VertexFormat(glm::vec3(0, 0, 0), color),
		VertexFormat(glm::vec3(0, 1, 0), color),
		VertexFormat(glm::vec3(1, 1, 0), color),
		VertexFormat(glm::vec3(1, 0, 0), color)
		};

		vector<unsigned short> indices(vertices.size());
		for (auto i = 0; i < vertices.size(); i++)
			indices[i] = i;

		auto s = "power_bar_" + color_str;
		meshes[s] = new Mesh(s);
		meshes[s]->InitFromData(vertices, indices);
		meshes[s]->SetDrawMode(GL_TRIANGLE_FAN);
	}
}

glm::mat3 PowerBar::Update(float deltaTimeSeconds)
{
	time += deltaTimeSeconds;

	if (show) {
		transform.scale_x += 50 * deltaTimeSeconds;
		if (transform.scale_x > 50)
			transform.scale_x = 50;
	}
	transform.position_x = player->transform.position_x;
	transform.position_y = player->transform.position_y - 100;
	glm::mat3 matrix(1);
	matrix *= Transform2D::Translate(transform.position_x, transform.position_y);
	matrix *= Transform2D::Rotate(transform.angle);
	matrix *= Transform2D::Scale(transform.scale_x, transform.scale_y);
	return matrix;
}

void PowerBar::OnMouseBtnPress()
{
	show = true;
}

void PowerBar::OnMouseBtnRelease()
{
	transform.scale_x = 0;
	show = false;
}
