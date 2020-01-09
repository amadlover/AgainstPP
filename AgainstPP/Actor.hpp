#pragma once

#include <string>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

#include <vector>

struct PrimitiveGraphicsData
{
};

class Actor
{
public:
	Actor ();
	~Actor ();
	
	std::vector<PrimitiveGraphicsData> PrimitiveGraphicsDatas;

	glm::mat4 TransformationMatrix;

	glm::vec3 Translation;
	glm::quat Rotation;
	glm::vec3 Scale[3];

	std::string Name;
};