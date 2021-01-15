#include "Camera.h"

Camera::Camera(MeshModel& mesh, glm::vec3& eye_, glm::vec3& at_, glm::vec3& up_):MeshModel(mesh)
{

	eye = eye_;
	at = at_;
	up = up_;

	
	left = -0.5f;
	right = 0.5f;
	bottom = -0.5f;
	top = 0.5f;
	_near = 0.5f;
	_far = 1.0f;

	zoom = 1.0f;
	fovy = 0.1f;
	aspectRatio = 1.0f;


	lookAtOrTransformation = true;
	OrthographicOrPerspective = true;
	
	setCameraDirection();
	SetPerspectiveData(_near, _far, fovy, aspectRatio);
	SetViewVolumeCoordinates(right, left, top, bottom, _near, _far);
}
Camera::~Camera()
{
	
}

void Camera::SetCameraLookAt(const glm::vec3& eye_, const glm::vec3& at_, const glm::vec3& up_)
{
	if (lookAtOrTransformation == false) {
		eye = eye_;
		at = at_;
		up = up_;
	}
}

void Camera::SetViewVolumeCoordinates(const float right_, const float left_, const float top_, const float bottom_, const float near_, const float far_)
{
	right = right_;
	left = left_;
	top = top_;
	bottom = bottom_;
	_near = near_;
	_far = far_;
	
	view_transformation_ = Utils::SetViewVolumeOrthographicTransformation(right, left, top, bottom, -_near, -_far);
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

	float height = glm::abs(glm::tan(glm::radians(fovy)/zoom)) * _near;
	float width = aspectRatio * height;

	view_transformation_ = Utils::SetViewVolumePerspectiveTransformation(width, -width, height, -height, near_, far_);
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

void Camera::SetZoom(const float _zoom)
{
	zoom = _zoom;
}

bool Camera::GetProjection() const
{
	return OrthographicOrPerspective;
}

void Camera::updateLookAt()
{
	if (lookAtOrTransformation == true) {
		glm::fmat4x4 transformation = glm::inverse(getWorldTransformation()) * getObjectTransformation();
		//up is treated as normal
		up = Utils::applyTransformationToNormal(glm::vec3(0, 1, 0), transformation);
		at = Utils::applyTransformationToVector(glm::vec3(0, 0, -1), transformation);
		eye = Utils::applyTransformationToVector(glm::vec3(0, 0, 0), transformation);

	}
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

float Camera::GetZoom() const
{
	return zoom;
}


// initlise camera to look at negative z direction
void Camera::setCameraDirection()
{
	
	glm::fmat4x4  transformation = Utils::TransformationRotateY(glm::radians(180.0f));

	for (int i = 0; i < GetVerticesCount();i++) {
		 vertices_[i] = Utils::applyTransformationToVector(GetVertexAtIndex(i), transformation);
	}
}

void Camera::setLookAtOrTransformation(const bool flag)
{
	lookAtOrTransformation = flag;
}

bool Camera::GetLookAtOrTransformation() const
{
	return lookAtOrTransformation;
}


