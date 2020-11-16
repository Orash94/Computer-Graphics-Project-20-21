#include "Camera.h"

Camera::Camera(MeshModel& mesh, glm::vec3& eye_, glm::vec3& at_, glm::vec3& up_):MeshModel(mesh)
{

	eye = eye_;
	at = at_;
	up = up_;
	this->lookAt(eye, at, up);
}

Camera::~Camera()
{
	
}

void Camera::SetCameraLookAt(const glm::vec3& eye_, const glm::vec3& at_, const glm::vec3& up_)
{
	eye = eye_;
	at = at_;
	up = up_;

}

glm::fmat4x4 Camera::lookAt(glm::vec3& eye, glm::vec3& at, glm::vec3& up)
{
	SetCameraLookAt(eye, at, up);
	glm::fvec3 z = glm::normalize(at - eye);
	glm::fvec3 x = glm::normalize(glm::cross(up, z));
	glm::fvec3 y = glm::normalize(glm::cross(z, x));
	glm::fvec4 t = glm::fvec4(0.0f, 0.0f, 0.0f, 1.0f);
	glm::fmat4x4 c = glm::fmat4x4(
		Utils::Euclidean2Homogeneous(x),
		Utils::Euclidean2Homogeneous(y), 
		Utils::Euclidean2Homogeneous( z), 
		t);
	return c * Utils::TransformationTransition(-eye);
}

const glm::mat4x4& Camera::GetProjectionTransformation() const
{
	return projection_transformation_;
}

const glm::mat4x4& Camera::GetViewTransformation() const
{
	return view_transformation_;
}

glm::vec3 Camera::getEye() const
{
	return eye;
}

glm::vec3 Camera::getAt() const
{
	return at;
}

glm::vec3 Camera::getUp() const
{
	return up;
}
