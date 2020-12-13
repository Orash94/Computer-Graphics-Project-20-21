#include "Light.h"

Light::Light(MeshModel& mesh, lightType typeLights):MeshModel(mesh)
{
	modelType = MeshModel::modelType::Light;
	typeOfLight = typeLights;
}

Light::~Light()
{
	
}

glm::fvec3 Light::calculateAmbient(glm::fvec3 MeshModelAmbientColor, glm::fvec3 LightAmbientColor)
{
	glm::fvec3 Ia = Utils::twoVectorsComponentMulti(MeshModelAmbientColor, LightAmbientColor);
	return Ia;
}

glm::fvec3 Light::calculateDiffuse(glm::fvec3 MeshModelDiffuseColor, glm::fvec3 LightDiffuseColor, glm::fvec3 Normal, glm::fvec3 lightcenter, glm::fvec3 modelcenter)
{

	glm::fvec3 Id = Utils::twoVectorsComponentMulti(MeshModelDiffuseColor, LightDiffuseColor);

	glm::fvec3 lightDirection = lightcenter - modelcenter;
	float degree = Utils::getDegreeBetweenTwoVectors(lightDirection, Normal);

	Id  = Id * glm::cos(degree);

	return Id;
}

glm::fvec3 Light::calculateSpecular(glm::fvec3 MeshModelSpecularColor, glm::fvec3 LightSpecularColor, glm::fvec3 Normal, glm::fvec3 MeshPoint, glm::fvec3 lightcenter, glm::fvec3 cameraCenter, float Alpha)
{
	glm::fvec3 Is = Utils::twoVectorsComponentMulti(MeshModelSpecularColor, LightSpecularColor);

	glm::fvec3 lightDirection = lightcenter - MeshPoint;
	glm::fvec3 CameraDirection = cameraCenter - MeshPoint;

	glm::fvec3 B = glm::dot(lightDirection, Normal) * glm::normalize(Normal) - lightcenter;
	glm::fvec3 reflection = lightDirection + 2.0f * B;

	float degree = Utils::getDegreeBetweenTwoVectors(reflection, CameraDirection);
	degree = glm::cos(degree);
	float power = pow(degree, Alpha);
	Is = Is * power;

	return Is;
}

glm::fvec3 Light::calculateColor(const MeshModel& mesh, const glm::fvec3 Normal, const glm::fvec3 MeshPoint, const glm::fvec3 modelcenter, const  glm::fvec3 lightcenter, const glm::fvec3 cameraCenter, const float Alpha )
{
	if (typeOfLight == Light::lightType::Parallel) {
		glm::fvec3 Id = calculateDiffuse(mesh.diffuseColor, diffuseColor, Normal, lightcenter, modelcenter);
		return Id;
	}
	else if (typeOfLight == Light::lightType::Point) {
		glm::fvec3 Ia = calculateAmbient(mesh.ambientColor, ambientColor);
		glm::fvec3 Is = calculateSpecular(mesh.specularColor, specularColor, Normal, MeshPoint, lightcenter, cameraCenter, Alpha);

		glm::fvec3 I = Ia + Is;

		return I;
	}
	
}



