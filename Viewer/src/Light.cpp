#include "Light.h"

Light::Light(MeshModel& mesh):MeshModel(mesh)
{
	
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

	glm::fvec3 B = glm::dot(lightDirection, Normal) * glm::normalize(Normal) - lightDirection;
	glm::fvec3 r = lightDirection + 2.0f * B;

	float degree = pow((Utils::getDegreeBetweenTwoVectors(r, CameraDirection)) , Alpha);
	Is = Is * degree;

	return Is;
}

glm::fvec3 Light::calculateColor(MeshModel& mesh, glm::fvec3 Normal, glm::fvec3 MeshPoint, glm::fvec3 modelcenter,  glm::fvec3 lightcenter, glm::fvec3 cameraCenter, float Alpha )
{
	
	glm::fvec3 Ia = calculateAmbient(mesh.ambientColor, ambientColor);
	glm::fvec3 Id = calculateDiffuse(mesh.diffuseColor, diffuseColor, Normal, lightcenter, modelcenter);
	glm::fvec3 Is = calculateSpecular(mesh.specularColor, specularColor, Normal, MeshPoint, lightcenter, cameraCenter , Alpha );
	glm::fvec3 I = Ia + Id + Is;

	return I;
}



