#pragma once
#include <glm/glm.hpp>
#include "Utils.h"
#include "MeshModel.h"
class Camera : public MeshModel
{
public:
	Camera(MeshModel& mesh, glm::vec3& eye_, glm::vec3& at_, glm::vec3& up_);
	virtual ~Camera();

	void SetCameraLookAt(const glm::vec3& eye_, const glm::vec3& at_, const glm::vec3& up_);
	void SetViewVolumeCoordinates(const float right_, const float left_, const float top_, const float bottom_, const float near_, const float far_);
	glm::fmat4x4 lookAt(glm::vec3& eye, glm::vec3& at, glm::vec3& up);
	const glm::mat4x4& GetProjectionTransformation() const;
	const glm::mat4x4& GetViewTransformation() const;

	void SetPerspectiveData(const float near_, const float far_, const float _fovy, const float _aspectRatio);
	void setProjection(const int Projection);
	bool GetProjection() const;

	void updateLookAt();

	glm::vec3 getEye()const;
	glm::vec3 getAt()const;
	glm::vec3 getUp()const;

	float GetRight() const;
	float GetLeft() const;
	float GetTop() const;
	float GetBottom() const;
	float GetNear() const;
	float GetFar() const;
	float GetFovy() const;
	float GetAspectRatio() const;




private:
	glm::mat4x4 view_transformation_;
	glm::mat4x4 view_transformation_inverse;
	glm::mat4x4 projection_transformation_;
	glm::vec3 eye;
	glm::vec3 at;
	glm::vec3 up;

	float right;
	float left;
	float top;
	float bottom;
	float _near;
	float _far;
	float fovy;
	float aspectRatio;

	bool OrthographicOrPerspective; // true = Orthographic. false = perspective
	
};
