#include "MeshModel.h"
#include "Utils.h"
#include <cmath>

MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name) :
	faces_(faces),
	vertices_(vertices),
	normals_(normals),
	model_name_(model_name)
{
	ObjectTransformation = Utils::getIdMat();
	WorldTransformation = Utils::getIdMat();
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


std::vector<Face> MeshModel::getFaces() const
{
	return faces_;
}

float* MeshModel::getScale()
{
	return scale;
}

float* MeshModel::getRotate()
{
	return Rotate;
}

float* MeshModel::getTranslate()
{
	return Translate;
}

float MeshModel::getDeltaMinMaxVertices()
{
	float minX = FLT_MAX;
	float minY = FLT_MAX;
	float minZ = FLT_MAX;

	float maxX = FLT_MIN;
	float maxY = FLT_MIN;
	float maxZ = FLT_MIN;

	for (int i = 0; i <= vertices_.size() -1 ; i++) {
		glm::fvec3 vertic = vertices_[i];
		float x = vertic[0];
		float y = vertic[1];
		float z = vertic[2];

		if (x < minX) {
			minX = x;
		}
		if (x > maxX) {
			maxX = x;
		}

		if (y < minY) {
			minY = y;
		}
		if (y > maxY) {
			maxY = y;
		}

		if (z < minZ) {
			minZ = z;
		}
		if (z > maxZ) {
			maxZ = z;
		}
	}

	float deltaX = maxX - minX;
	float deltaY = maxY - minY;
	float deltaZ = maxZ - minZ;

	return glm::max(glm::max(deltaX, deltaY), deltaZ);

}

glm::vec3 MeshModel::getMiddleOfModel()
{
	float minX = FLT_MAX;
	float minY = FLT_MAX;
	float minZ = FLT_MAX;

	float maxX = FLT_MIN;
	float maxY = FLT_MIN;
	float maxZ = FLT_MIN;

	for (int i = 0; i <= vertices_.size() - 1; i++) {
		glm::fvec3 vertic = vertices_[i];
		float x = vertic[0];
		float y = vertic[1];
		float z = vertic[2];

		if (x < minX) {
			minX = x;
		}
		if (x > maxX) {
			maxX = x;
		}

		if (y < minY) {
			minY = y;
		}
		if (y > maxY) {
			maxY = y;
		}

		if (z < minZ) {
			minZ = z;
		}
		if (z > maxZ) {
			maxZ = z;
		}
	}

	float deltaX = -((minX + maxX) / 2);
	float deltaY = -((minY + maxY) / 2);
	float deltaZ = -((minZ + maxZ) / 2);
	return glm::vec3(deltaX, deltaY, deltaZ);
}

