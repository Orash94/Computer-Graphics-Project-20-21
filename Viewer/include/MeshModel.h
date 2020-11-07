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

	

	std::vector<Face> getFaces() const;
	float* getScale();
	float* getRotate();
	float* getTranslate();
	float getInitialScale();
	void getMiddleOfModel();
	void setMinMax();

	void setTransformationUpdates(const float* nScale, const float* nRotate, const float* nTrasnlate);

	void setObjectTransformation(const glm::fmat4x4 transform);
	void setWorldTransformation(const glm::fmat4x4 transform);

	float& getScaleX();




private:
	float scale[3] = { 1.0f, 1.0f, 1.0f };
	float Rotate[3] = { 0.0f, 0.0f, 0.0f };
	float Translate[3] = { 0.0f, 0.0f, 0.0f };
	glm::fmat4x4 ObjectTransformation;
	glm::fmat4x4 WorldTransformation;
	std::vector<Face> faces_;
	std::vector<glm::vec3> vertices_;
	std::vector<glm::vec3> normals_;
	std::string model_name_;
	float minX_ = FLT_MAX;
	float minY_ = FLT_MAX;
	float minZ_ = FLT_MAX;

	float maxX_ = FLT_MIN;
	float maxY_ = FLT_MIN;
	float maxZ_ = FLT_MIN;
	
};
