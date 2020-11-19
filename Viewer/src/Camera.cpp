#include "Camera.h"

Camera::Camera(MeshModel& mesh, glm::vec3& eye_, glm::vec3& at_, glm::vec3& up_):MeshModel(mesh)
{

	eye = eye_;
	at = at_;
	up = up_;
	this->lookAt(eye, at, up);

	left = -10.0f;
	right = 10.0f;
	bottom = -10.0f;
	top = 10.0f;
	_near = 1.0f;
	_far = 10.0f;

	fovy = 90.0f;
	aspectRatio = 1.5f;

	OrthographicOrPerspective = true;
	

}
// _projFovy(90.0f), _projAspectRatio(1.5f)
Camera::~Camera()
{
	
}

void Camera::SetCameraLookAt(const glm::vec3& eye_, const glm::vec3& at_, const glm::vec3& up_)
{
	eye = eye_;
	at = at_;
	up = up_;

}

void Camera::SetViewVolumeCoordinates(const float right_, const float left_, const float top_, const float bottom_, const float near_, const float far_)
{
	right = right_;
	left = left_;
	top = top_;
	bottom = bottom_;
	_near = near_;
	_far = far_;
	
	view_transformation_ = Utils::SetViewVolumeOrthographicTransformation(right, left, top, bottom, _near, _far);
}

glm::fmat4x4 Camera::lookAt(glm::vec3& eye, glm::vec3& at, glm::vec3& up)
{
	SetCameraLookAt(eye, at, up);
	glm::fvec3 z = glm::normalize(at - eye);
	glm::fvec3 x = glm::normalize(glm::cross(up, z));
	glm::fvec3 y = glm::normalize(glm::cross(z, x));
	glm::fvec4 t = glm::fvec4(0.0f, 0.0f, 0.0f, 1.0f);
	glm::fmat4x4 c =glm::transpose( glm::fmat4x4(
		Utils::Euclidean2Homogeneous(x),
		Utils::Euclidean2Homogeneous(y), 
		Utils::Euclidean2Homogeneous( z), 
		t));
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

void Camera::SetPerspectiveData(const float near_, const float far_, const float _fovy, const float _aspectRatio)
{
	_near = near_;
	_far = far_;
	fovy = _fovy;
	aspectRatio = _aspectRatio;

	//projection_transformation_=Utils::SetViewVolumePerspectiveTransformation
}

void Camera::setProjection(const int Projection)
{
	if (Projection == 0)
		OrthographicOrPerspective = false;
	else
	{
		OrthographicOrPerspective = true;
	}
}

bool Camera::GetProjection() const
{
	return OrthographicOrPerspective;
}

void Camera::updateLookAt()
{
	glm::fmat4x4 transformation = glm::inverse(getWorldTransformation()) * getObjectTransformation();
	at = Utils::applyTransformationToVector(at, transformation);
	eye = Utils::applyTransformationToVector(eye, transformation);
	up = Utils::applyTransformationToVector(up, transformation);

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

float Camera::GetRight() const
{
	return right;
}

float Camera::GetLeft() const
{
	return left;
}

float Camera::GetTop() const
{
	return top;
}

float Camera::GetBottom() const
{
	return bottom;
}

float Camera::GetNear() const
{
	return _near;
}

float Camera::GetFar() const
{
	return _far;
}

float Camera::GetFovy() const
{
	return fovy;
}

float Camera::GetAspectRatio() const
{
	return aspectRatio;
}


