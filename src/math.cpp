#include "../include/math.h"

namespace math
{
	float to_radians(float degree)
	{
		return degree / 180.0f * M_PI;
	}
	
	float to_degrees(float radian)
	{
		return radian * 180.0f / M_PI;
	}
	
	glm::vec2 rotate(glm::vec2 pivot, glm::vec2 point, float angle)
	{
		point -= pivot;
		point = glm::rotate(point, angle);
		point += pivot;
		return point;
	}
}
