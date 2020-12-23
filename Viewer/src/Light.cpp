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

glm::fvec3 Light::calculateDiffuse(glm::fvec3 MeshModelDiffuseColor, glm::fvec3 LightDiffuseColor, glm::fvec3 Normal , glm::fvec3 lightDirection)
{
	Normal = glm::normalize(Normal);
	lightDirection = glm::normalize(lightDirection);

	glm::fvec3 Id = Utils::twoVectorsComponentMulti(MeshModelDiffuseColor, LightDiffuseColor);


	glm::fvec3 reflevtive = lightDirection - 2.0f * (glm::dot(lightDirection, Normal) * Normal);

	float degree = Utils::getDegreeBetweenTwoVectors(Normal, glm::normalize(reflevtive));
	Id  = Id * glm::cos(degree);

	return Id;
}

glm::fvec3 Light::calculateSpecular(glm::fvec3 MeshModelSpecularColor, glm::fvec3 LightSpecularColor, glm::fvec3 Normal, glm::fvec3 MeshPoint, glm::fvec3 lightcenter, glm::fvec3 cameraCenter, float Alpha)
{
	glm::fvec3 Is = Utils::twoVectorsComponentMulti(MeshModelSpecularColor, LightSpecularColor);

	glm::fvec3 lightDirection = glm::normalize(lightcenter - MeshPoint);
	glm::fvec3 CameraDirection = glm::normalize(cameraCenter - MeshPoint);
	glm::fvec3 faceNoraml = glm::normalize(Normal);
	glm::fvec3 B = (glm::dot(lightDirection, faceNoraml) * faceNoraml) - lightDirection;
	glm::fvec3 reflection = glm::normalize(lightDirection + 2.0f * B);

	float degree = Utils::getDegreeBetweenTwoVectors(reflection, CameraDirection);
	degree = glm::cos(degree);
	float power = pow(degree, Alpha);
	Is = Is * power;

	return Is;
}

glm::fvec3 Light::calculateColor(const MeshModel& mesh, const glm::fvec3 Normal, const glm::fvec3 MeshPoint, const glm::fvec3 modelcenter, const  glm::fvec3 lightcenter, const glm::fvec3 cameraCenter, const float Alpha )
{
	glm::fvec3 AmbientLight = calculateAmbient(mesh.ambientColor, ambientColor);
	
	//glm::fvec3 DiffuseLight =calculateDiffuse(mesh.diffuseColor, diffuseColor, Normal,  lightDirection);

	glm::fvec3 SpecularLight = calculateSpecular(mesh.specularColor, specularColor, Normal, MeshPoint, lightcenter, cameraCenter,  Alpha);

	//glm::fvec3 light = AmbientLight + DiffuseLight + SpecularLight;

	//return light ;
	return glm::fvec3();
}



