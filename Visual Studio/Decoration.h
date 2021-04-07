#pragma once
#include <glm/detail/type_vec.hpp>
#include <glm/detail/type_vec3.hpp>

class Decoration
{
public:
	explicit Decoration(glm::vec3 position, bool light, glm::vec3 color, bool r) : has_light(light) , position(position), color(color), right(r)
	{
	}

	bool has_light;
	glm::vec3 position;
	glm::vec3 color;
	bool right;
};

