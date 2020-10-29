#include "MeshModel.h"
#include <cmath>

MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name) :
	faces_(faces),
	vertices_(vertices),
	normals_(normals),
	model_name_(model_name)
{

}

MeshModel::~MeshModel()
{
}

const Face& MeshModel::GetFace(int index) const
{
	return faces_[index];
}

int MeshModel::GetFacesCount() const
{
	return faces_.size();
}

const std::string& MeshModel::GetModelName() const
{
	return model_name_;
}


const glm::vec3& MeshModel::GetVertexAtIndex(int i) const
{
	return vertices_[i];
}

glm::fmat4x4  MeshModel::TansformationScale(const glm::fvec3 position)
{
	float x = position[0];
	float y = position[1];
	float z = position[2];

	glm::fvec4 vec1 = glm::fvec4(x, 0, 0, 0);
	glm::fvec4 vec2 = glm::fvec4(0, y, 0, 0);
	glm::fvec4 vec3 = glm::fvec4(0, 0, z, 0);
	glm::fvec4 vec4 = glm::fvec4(0, 0, 0, 1.0f);

	return  glm::transpose(glm::fmat4x4(vec1, vec2, vec3, vec4));

}

glm::fmat4x4 MeshModel::TansformationTransition(const glm::fvec3 position)
{
	float x = position[0];
	float y = position[1];
	float z = position[2];

	glm::fvec4 vec1 = glm::fvec4(1.0f, 0, 0, x);
	glm::fvec4 vec2 = glm::fvec4(0, 1.0f, 0, y);
	glm::fvec4 vec3 = glm::fvec4(0, 0, 1.0f, z);
	glm::fvec4 vec4 = glm::fvec4(0, 0, 0, 1.0f);

	return  glm::transpose(glm::fmat4x4(vec1, vec2, vec3, vec4));
}

glm::fmat4x4 MeshModel::TansformationRotateX(const float angle)
{

	glm::fvec4 vec1 = glm::fvec4(1.0f, 0, 0, 0);
	glm::fvec4 vec2 = glm::fvec4(0, cos(angle), -sin(angle), 0);
	glm::fvec4 vec3 = glm::fvec4(0, sin(angle), cos(angle), 0);
	glm::fvec4 vec4 = glm::fvec4(0, 0, 0, 1.0f);

	return glm::transpose(glm::fmat4x4(vec1, vec2, vec3, vec4));
}

glm::fmat4x4 MeshModel::TansformationRotateY(const float angle)
{

	glm::fvec4 vec1 = glm::fvec4(cos(angle), 0, sin(angle), 0);
	glm::fvec4 vec2 = glm::fvec4(0, 1,0, 0);
	glm::fvec4 vec3 = glm::fvec4(-sin(angle), 0, cos(angle), 0);
	glm::fvec4 vec4 = glm::fvec4(0, 0, 0, 1.0f);

	return glm::transpose(glm::fmat4x4(vec1, vec2, vec3, vec4));
}

glm::fmat4x4 MeshModel::TansformationRotateZ(const float angle)
{
	glm::fvec4 vec1 = glm::fvec4(cos(angle), -sin(angle), 0, 0);
	glm::fvec4 vec2 = glm::fvec4(sin(angle), cos(angle), 0, 0);
	glm::fvec4 vec3 = glm::fvec4(0, 0, 1.0f, 0);
	glm::fvec4 vec4 = glm::fvec4(0, 0, 0, 1.0f);

	return glm::transpose(glm::fmat4x4(vec1, vec2, vec3, vec4));
}

glm::fvec3 MeshModel::Homogeneous2Euclidean(const glm::fvec4 vec)
{
	float x = vec[0];
	float y = vec[1];
	float z = vec[2];
	float w = vec[3];
	return glm::fvec3(x/w, y/w, z/w);
}

glm::fvec4 MeshModel::Euclidean2Homogeneous(const glm::fvec3 vec)
{
	float x = vec[0];
	float y = vec[1];
	float z = vec[2];

	return glm::fvec4(x , y , z , 1);
}

