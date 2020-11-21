#include "Camera.h"

Camera::Camera(MeshModel& mesh, glm::vec3& eye_, glm::vec3& at_, glm::vec3& up_):MeshModel(mesh)
{

	eye = eye_;
	at = at_;
	up = up_;
	this->lookAt(eye, at, up);

	left = -0.5f;
	right = 0.5f;
	bottom = -0.5f;
	top = 0.5f;
	_near = 0.5f;
	_far = 1.0f;

	zoom = 1.0f;
	fovy = 0.1f;
	aspectRatio = 1.0f;

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
	
	view_transformation_ = Utils::SetViewVolumeOrthographicTransformation(right, left, top, bottom, -_near, -_far);
}


//return the inverse camera transform
glm::fmat4x4 Camera::lookAt(glm::vec3& eye , glm::vec3& at, glm::vec3& up)
{
	
	glm::vec3 z = normalize(eye - at);
	glm::vec3 x = normalize(glm::cross(up, z));
	glm::vec3 y = normalize(glm::cross(z, x));

	glm::mat4x4 c = glm::transpose(glm::mat4x4({
		x[0], x[1], x[2], 0,
		y[0], y[1], y[2], 0,
		z[0], z[1], z[2], 0,
		0, 0, 0, 1
		}
	));

	glm::mat4x4 translationMatrix = glm::transpose(glm::mat4x4(
		{ 1	,	0	,	0	,	-eye[0],
			0	,	1	,	0	,	-eye[1],
			0	,	0	,	1	,	-eye[2],
			0	,	0	,	0	,	1 }));

	return glm::transpose(c) * translationMatrix;

	/*
	
	glm::fvec3 z = glm::normalize( eye - at);
	glm::fvec3 x = glm::normalize(glm::cross(up, z));
	glm::fvec3 y = glm::normalize(glm::cross(z, x));
	glm::fvec4 t = glm::fvec4(0.0f, 0.0f, 0.0f, 1.0f);
	glm::fmat4x4 c =glm::transpose( glm::fmat4x4(
		Utils::Euclidean2Homogeneous(x),
		Utils::Euclidean2Homogeneous(y), 
		Utils::Euclidean2Homogeneous( z), 
		t));
	return c * Utils::TransformationTransition(-eye);
	*/
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

	float height = glm::abs(glm::tan(Utils::degrees2Radians(fovy)/zoom)) * _near;
	float width = aspectRatio * height;

	view_transformation_ = Utils::SetViewVolumePerspectiveTransformation(width, -width, height, -height, -near_, -far_);
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
	glm::fmat4x4 transformation = glm::inverse(getWorldTransformation()) * getObjectTransformation();
	at = Utils::applyTransformationToVector(glm::vec3(0, 0, -1), transformation);
	eye = Utils::applyTransformationToVector(glm::vec3(0, 0, 0), transformation);
	up = Utils::applyTransformationToVector(glm::vec3(0, 1, 0), transformation);

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

	glm::fmat4x4  transformation = Utils::TransformationRotateY(Utils::degrees2Radians(180.0f));

	for (int i = 0; i < GetVerticesCount();i++) {
		 vertices_[i] = Utils::applyTransformationToVector(GetVertexAtIndex(i), transformation);
	}
}


