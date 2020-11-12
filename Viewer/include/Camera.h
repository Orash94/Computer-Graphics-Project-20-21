#pragma once
#include <glm/glm.hpp>
#include "Utils.h"
class Camera
{
public:
	Camera();
	virtual ~Camera();

	void SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);
	glm::fmat4x4 lookAt(glm::fvec3& eye, glm::fvec3& at, glm::fvec3& up);
	const glm::mat4x4& GetProjectionTransformation() const;
	const glm::mat4x4& GetViewTransformation() const;

private:
	glm::mat4x4 view_transformation_;
	glm::mat4x4 view_transformation_inverse;
	glm::mat4x4 projection_transformation_;
};
