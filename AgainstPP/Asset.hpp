#pragma once

#include <string>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

#include <vector>

struct PrimitivePhysicsData
{
};

struct PrimitiveGraphicsData
{
};

class Asset
{
public:
	Asset ();
	~Asset ();
	
	std::vector<PrimitiveGraphicsData> PrimitiveGraphicsDatas;
	std::vector<PrimitivePhysicsData> PrimitivePhysicsDatas;

	glm::mat4 TransformationMatrix;

	glm::vec3 Translation;
	glm::quat Rotation;
	glm::vec3 Scale;

	std::string Name;
};

namespace asset
{
	struct asset
	{

	};

	void import_assets (const std::string& file_path, std::vector<asset>& assets);
}