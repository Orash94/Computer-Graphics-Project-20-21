#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Face.h"

class MeshModel
{
public:
	MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name);
	virtual ~MeshModel();
	const Face& GetFace(int index) const;
	int GetFacesCount() const;
	const std::string& GetModelName() const;
	const glm::vec3& GetVertexAtIndex(int i) const;

	glm::fmat4x4 TansformationScale(const glm::fvec3 position);
	glm::fmat4x4 TansformationTransition(const glm::fvec3 position);
	glm::fmat4x4 TansformationRotateX(const float angle);
	glm::fmat4x4 TansformationRotateY(const float angle);
	glm::fmat4x4 TansformationRotateZ(const float angle);

	glm::fvec3 Homogeneous2Euclidean(const glm::fvec4 vec);
	glm::fvec4 Euclidean2Homogeneous(const glm::fvec3 vec);
	
	std::vector<Face> getFaces() const;
	
private:
	std::vector<Face> faces_;
	std::vector<glm::vec3> vertices_;
	std::vector<glm::vec3> normals_;
	std::string model_name_;
};
