#pragma once

#include <include/glm.h>
#include <cmath>

namespace Transform2D
{
	// Translate matrix
	inline glm::mat3 Translate(float translateX, float translateY)
	{
		// TODO implement translate matrix
		return glm::mat3(1, 0, 0, 0, 1, 0, translateX, translateY, 1);
	}

	// Scale matrix
	inline glm::mat3 Scale(float scaleX, float scaleY)
	{
		// TODO implement scale matrix
		return glm::mat3(scaleX, 0, 0, 0, scaleY, 0, 0, 0,1);
	}

	// Rotate matrix
	inline glm::mat3 Rotate(float radians)
	{
		// TODO implement rotate matrix
		return glm::mat3(cos(radians), sin(radians), 0,
			-sin(radians), cos(radians), 0, 0, 0, 1);
	}
}
