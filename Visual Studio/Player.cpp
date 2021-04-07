#include "Player.h"

#include <corecrt_math_defines.h>

#include "Laboratoare/Laborator3/Transform2D.h"
#include "Core/Engine.h"
#include "Core/GPU/Mesh.h"

using namespace std;

Player::Player(glm::vec3 color, float sp)
{
	this->color = color;
	speedMultiplier = 1;
	SetSpeed(sp);
	transform = Transform(50, 300, 50, 50, 0);
}

void Player::Init(std::unordered_map<std::string, Mesh*>& meshes) const
{
	for (const auto& item : colors) {
		auto color_str = item.first;
		const auto color = item.second;
		vector<VertexFormat> vertices;
		for (float step = -M_PI / 2.0; step < M_PI / 2; step += 0.001)
			vertices.emplace_back(glm::vec3(cos(step), sin(step), 0), color);

		vector<unsigned short> indices(vertices.size());
		for (auto i = 0; i < vertices.size(); i++)
			indices[i] = i;
		
		string s = "bow_" + color_str;
		meshes[s] = new Mesh(s);
		meshes[s]->InitFromData(vertices, indices);
		meshes[s]->SetDrawMode(GL_LINE_LOOP);
	}
}

glm::mat3 Player::Update(float deltaTimeSeconds)
{
	time += deltaTimeSeconds;
	glm::mat3 matrix(1);
	matrix *= Transform2D::Translate(transform.position_x, transform.position_y);
	matrix *= Transform2D::Rotate(transform.angle);
	matrix *= Transform2D::Scale(transform.scale_x, transform.scale_y);
	return matrix;
}

void Player::OnMouseMove(int mouseX, int mouseY)
{
	const auto delta_x = mouseX - transform.position_x;
	const auto delta_y = 720 - mouseY - transform.position_y;
	const auto angle = atan(delta_y / delta_x) + (delta_x < 0 ? M_PI : 0) + 
		(delta_x > 0 && delta_y < 0 ? 2 * M_PI : 0);
	transform.angle = angle;
}
