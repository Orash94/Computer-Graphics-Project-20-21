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
	setMinMax();
	getMiddleOfModel();
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


void MeshModel:: setMinMax() {

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

	minX_ = minX;
	minY_ = minY;
	minZ_ = minZ;
	maxX_ = maxX;
	maxY_ = maxY;
	maxZ_ = maxZ;

}

void MeshModel::setTransformationUpdates(const float* nScale, const float* nRotate, const float* nTrasnlate)
{
	for (int i = 0; i < 3; i++) {
		scale[i] = nScale[i];
		Rotate[i] = nRotate[i];
		Translate[i] = nTrasnlate[i];
	}
}

void MeshModel::setObjectTransformation(const glm::fmat4x4 transform = Utils::getIdMat())
{
	ObjectTransformation = transform;

}

void MeshModel::setWorldTransformation(const glm::fmat4x4 transform = Utils::getIdMat())
{
	WorldTransformation = transform;
}

float& MeshModel::getScaleX() 
{
	return scale[0];
}

float MeshModel::getInitialScale()
{
	


	float deltaX = maxX_ - minX_;
	float deltaY = maxY_ - minY_;
	float deltaZ = maxZ_ - minZ_;

	return glm::max(glm::max(deltaX, deltaY), deltaZ);

}

void MeshModel::getMiddleOfModel()
{
	

	float deltaX = -((minX_ + maxX_) / 2);
	float deltaY = -((minY_ + maxY_) / 2);
	float deltaZ = -((minZ_ + maxZ_) / 2);
	glm::vec3 middled = glm::vec3(deltaX, deltaY, deltaZ);
	for (int i = 0; i <= vertices_.size() - 1; i++) {
		vertices_[i] += middled;
	}
	for (int i = 0; i <= normals_.size() - 1; i++) {
		normals_[i] += middled;
	}

}

