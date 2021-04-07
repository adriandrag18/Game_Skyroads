#include "Balloon.h"

#include <corecrt_math_defines.h>

#include "Core/GPU/Mesh.h"
#include "Laboratoare/Laborator3/Transform2D.h"
using namespace std;

Balloon::Balloon(glm::vec3 color, const float sp)
{
	transform = Transform(rand() % 720 + 280, rand() % 300 - 400, 30, 50, 0);
	initial_x = rand() % 720 + 280;
	speedMultiplier = 1;
	SetSpeed(sp);
	this->color = color;
}

void Balloon::Init(std::unordered_map<std::string, Mesh*>& meshes) const
{
	for (const auto& item : colors) {
		const auto& color_str = item.first;
		const auto& color = item.second;
		
		vector<VertexFormat> vertices;
		vertices.emplace_back(glm::vec3(0, 1, 0), color == red ? yellow : red);
		for (float step = 0; step < 2 * M_PI; step += 0.001)
			vertices.emplace_back(glm::vec3(cos(step), 1 + sin(step), 0), color);

		vector<unsigned short> indices(vertices.size());
		for (auto i = 0; i < vertices.size(); i++)
			indices[i] = i;
		
		auto id = "balloon_body_" + color_str;
		meshes[id] = new Mesh(id);
		meshes[id]->InitFromData(vertices, indices);
		// meshes[id]->SetDrawMode(GL_LINE_LOOP);
		meshes[id]->SetDrawMode(GL_TRIANGLE_FAN);

		vertices.clear();
		vertices.emplace_back(glm::vec3(-0.2f, -0.1f, 0), color);
		vertices.emplace_back(glm::vec3(0.2f, -0.1f, 0), color);
		vertices.emplace_back(glm::vec3(0, 0.1, 0), color);
		indices.clear();
		indices = { 0, 1, 2 };

		id = "balloon_triangle_" + color_str;
		meshes[id] = new Mesh(id);
		meshes[id]->InitFromData(vertices, indices);
		meshes[id]->SetDrawMode(GL_TRIANGLES);

		vertices.clear();
		vertices.emplace_back(glm::vec3(0, -0.1f, 0), color);
		vertices.emplace_back(glm::vec3(0, -1.1f, 0), color);
		indices.clear();
		indices = { 0, 1 };

		id = "balloon_string_" + color_str;
		meshes[id] = new Mesh(id);
		meshes[id]->InitFromData(vertices, indices);
		meshes[id]->SetDrawMode(GL_LINES);
	}
}

glm::mat3 Balloon::Update(float deltaTimeSeconds)
{
	time += deltaTimeSeconds;
	if (burst)
		Fall(deltaTimeSeconds);
	else
		Rise(deltaTimeSeconds);
	
	glm::mat3 matrix(1);
	matrix *= Transform2D::Translate(transform.position_x, transform.position_y);
	matrix *= Transform2D::Rotate(transform.angle);
	matrix *= Transform2D::Scale(transform.scale_x, transform.scale_y);
	return matrix;
}

void Balloon::Rise(float deltaTimeSeconds)
{
	transform.position_y += speed * deltaTimeSeconds;
	if (transform.position_y > 720 + 2.1 * 50) {
		transform.position_y = rand() % 300 + 400;
		initial_x = rand() % 720 + 280;
	}
	transform.position_x = initial_x + speed * sin(speed / 100 * time);
	transform.angle = sin(speed / 100 *  (time - M_PI / 2)) / 5;
}

void Balloon::Fall(float deltaTimeSeconds)
{
	transform.scale_x -= 15 * deltaTimeSeconds;
	transform.scale_y -= 25 * deltaTimeSeconds;
	if (transform.scale_x < 6)
		transform.scale_x = 6;
	
	speed -= 196 * deltaTimeSeconds;
	transform.position_y += speed * deltaTimeSeconds;
}

void Balloon::Burst()
{
	burst = true;
	speed = 0;
}
