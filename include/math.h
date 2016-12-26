#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace math
{
	float to_radians(float degree);
	float to_degrees(float radian);
	
	glm::vec2 rotate(glm::vec2 pivot, glm::vec2 point, float angle);
	
}
