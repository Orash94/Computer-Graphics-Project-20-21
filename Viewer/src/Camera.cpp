#include "Camera.h"

Camera::Camera()
{
	
}

Camera::~Camera()
{
	
}

void Camera::SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
{


}

glm::fmat4x4 Camera::lookAt(glm::fvec3& eye, glm::fvec3& at, glm::fvec3& up)
{
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