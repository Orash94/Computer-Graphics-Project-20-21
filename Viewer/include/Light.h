#pragma once
#include <glm/glm.hpp>
#include "glm/ext.hpp"
#include "Utils.h"
#include "MeshModel.h"

class Light : public MeshModel
{
public:
	Light::Light(MeshModel& mesh);
	virtual ~Light();

private:

};
