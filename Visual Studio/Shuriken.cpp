#include "Shuriken.h"

#include <iostream>


#include "Laboratoare/Laborator3/Transform2D.h"
#include "Core/Engine.h"
#include "Core/GPU/Mesh.h"

using namespace std;

Shuriken::Shuriken(glm::vec3 color, float sp)
{
	transform = Transform(rand() % 300 + 1320, rand() % 600 + 60, 30, 30, 0);
	speedMultiplier = 2;
	SetSpeed(sp);
	this->color = color;
}


void Shuriken::Init(std::unordered_map<std::string, Mesh*>& meshes) const
{
	for (const auto& item : colors) {
		const auto& color_str = item.first;
		const auto& color = item.second;
		vector<VertexFormat> vertices;
		glm::vec3 center(0, 0, 0);

		vertices.emplace_back(center, color);
		vertices.emplace_back(glm::vec3(1, 0, 0), color);
		vertices.emplace_back(glm::vec3(1, 1, 0), color);
		vertices.emplace_back(center, color);
		vertices.emplace_back(glm::vec3(0, 1, 0), color);
		vertices.emplace_back(glm::vec3(-1, 1, 0), color);
		vertices.emplace_back(center, color);
		vertices.emplace_back(glm::vec3(-1, 0, 0), color);
		vertices.emplace_back(glm::vec3(-1, -1, 0), color);
		vertices.emplace_back(center, color);
		vertices.emplace_back(glm::vec3(0, -1, 0), color);
		vertices.emplace_back(glm::vec3(1, -1, 0), color);
		vertices.emplace_back(center, color);

		vector<unsigned short> indices(vertices.size());
		for (auto i = 0; i < vertices.size(); i++)
			indices[i] = i;

		auto s = "shuriken_" + color_str;
		meshes[s] = new Mesh(s);
		meshes[s]->InitFromData(vertices, indices);
		meshes[s]->SetDrawMode(GL_TRIANGLES);
	}
}

glm::mat3 Shuriken::Update(float deltaTimeSeconds)
{
	time += deltaTimeSeconds;
	transform.position_x -= speed * deltaTimeSeconds;
	if (transform.position_x < -40) {
		transform.position_x = rand() % 300 + 1320;
		transform.position_y = rand() % 600 + 60;
		hit_player = false;
	}
	transform.angle = speed / 100 * time;

	glm::mat3 matrix(1);
	matrix *= Transform2D::Translate(transform.position_x, transform.position_y);
	matrix *= Transform2D::Rotate(transform.angle);
	matrix *= Transform2D::Scale(transform.scale_x, transform.scale_y);

	return matrix;
}
