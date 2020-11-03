#pragma once
#include <glm/glm.hpp>
#include <string>
#include "MeshModel.h"

class Utils
{
public:
	static glm::vec3 Vec3fFromStream(std::istream& issLine);
	static glm::vec2 Vec2fFromStream(std::istream& issLine);
	static std::shared_ptr<MeshModel> LoadMeshModel(const std::string& filePath);
	static std::string GetFileName(const std::string& filePath);

	static glm::fmat4x4 TansformationScale(const glm::fvec3 position);
	static glm::fmat4x4 TansformationTransition(const glm::fvec3 position);
	static glm::fmat4x4 TansformationRotateX(const float angle);
	static glm::fmat4x4 TansformationRotateY(const float angle);
	static glm::fmat4x4 TansformationRotateZ(const float angle);
	static glm::fmat4x4 TansformationShear(const glm::fvec2 vector);
	static glm::fvec3 Homogeneous2Euclidean(const glm::fvec4 vec);
	static glm::fvec4 Euclidean2Homogeneous(const glm::fvec3 vec);

	static void resetMatrix(glm::fmat4x4& mat);
};
