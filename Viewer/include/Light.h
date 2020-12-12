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
	glm::fvec3 calculateAmbient(glm::fvec3 MeshModelAmbientColor, glm::fvec3 LightAmbientColor );
	glm::fvec3 calculateDiffuse(glm::fvec3 MeshModelDiffuseColor, glm::fvec3 LightDiffuseColor, glm::fvec3 Normal,  glm::fvec3 lightcenter, glm::fvec3 modelcenter);
	glm::fvec3 calculateSpecular(glm::fvec3 MeshModelSpecularColor, glm::fvec3 LightSpecularColor, glm::fvec3 Normal, glm::fvec3 MeshPoint ,glm::fvec3 lightcenter, glm::fvec3 cameraCenter , float Alpha);
	glm::fvec3 calculateColor(MeshModel& mesh, glm::fvec3 Normal, glm::fvec3 MeshPoint,  glm::fvec3 modelcenter,  glm::fvec3 lightcenter, glm::fvec3 cameraCenter ,float Alpha);

	float alpha = 1;
private:

};
