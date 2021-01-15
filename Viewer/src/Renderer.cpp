#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include <iostream>
#include "glm/ext.hpp"
#include "Renderer.h"
#include "InitShader.h"
#include "Utils.h"

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)
#define Z_INDEX(width,x,y) ((x)+(y)*(width))

Renderer::Renderer(int viewport_width, int viewport_height ,  Scene& scene_) :
	viewport_width_(viewport_width),
	viewport_height_(viewport_height),
	scene(scene_)
{
	InitOpenGLRendering();
	CreateBuffers(viewport_width, viewport_height);
	allocateZBuffer();
	allocateColorBuffer();
}

Renderer::~Renderer()
{
	delete[] color_buffer_;
	for (int i = 0; i < viewport_width_; i++) {
		delete[] Zbuffer[i];
		delete[] localColorBuffer[i];
	}
	delete[] Zbuffer;
	delete[] localColorBuffer;
}

void Renderer::PutPixel(int i, int j, const float z, const glm::vec3& color)
{
	if (i < 0) return; if (i >= viewport_width_) return;
	if (j < 0) return; if (j >= viewport_height_) return;

	
	float CameraFar = scene.GetActiveCamera().GetFar();
	float CameraNear = scene.GetActiveCamera().GetNear();

	if (Zbuffer[i][j] >= z ) {
		/*if (scene.CamOrWorldView ) {
			
			if (z>= CameraNear && z<=CameraFar) {
				color_buffer_[INDEX(viewport_width_, i, j, 0)] = color.x;
				color_buffer_[INDEX(viewport_width_, i, j, 1)] = color.y;
				color_buffer_[INDEX(viewport_width_, i, j, 2)] = color.z;
				Zbuffer[i][j] = z;
			}
		}
		else {
			color_buffer_[INDEX(viewport_width_, i, j, 0)] = color.x;
			color_buffer_[INDEX(viewport_width_, i, j, 1)] = color.y;
			color_buffer_[INDEX(viewport_width_, i, j, 2)] = color.z;
			Zbuffer[i][j] = z;
		}*/
		/*if (z > 1 && z<765) {
				std::cout<< z<<std::endl;
		}*/
		if (scene.GetActiveCamera().GetOrthographicOrPerspective()) {
			//Orthographic
			if (1) {

				color_buffer_[INDEX(viewport_width_, i, j, 0)] = color.x;
				color_buffer_[INDEX(viewport_width_, i, j, 1)] = color.y;
				color_buffer_[INDEX(viewport_width_, i, j, 2)] = color.z;
				Zbuffer[i][j] = z;
				localColorBuffer[i][j] = color;
			}
		}
		else {
			//Perspective
			

			if (1) {
				color_buffer_[INDEX(viewport_width_, i, j, 0)] = color.x;
				color_buffer_[INDEX(viewport_width_, i, j, 1)] = color.y;
				color_buffer_[INDEX(viewport_width_, i, j, 2)] = color.z;
				Zbuffer[i][j] = z;
				localColorBuffer[i][j] = color;
			}
		}
		
	}


}

void Renderer::UpdatePutPixel()
{
	for (int i = 0; i < viewport_width_; i++)
	{
		for (int j = 0; j < viewport_height_; j++)
		{
			PutPixel(i, j, Zbuffer[i][j], localColorBuffer[i][j]);

		}

	}
}

void Renderer::DrawLine(const glm::fvec3& p1, const glm::fvec3& p2, const glm::vec3& color)
{

	// TODO: Implement bresenham algorithm
	// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm

	int x0 = (int)p1[0], y0 = (int)p1[1];
	int x1 = (int)p2[0], y1 = (int)p2[1];

	float z1 = p1[0], z2 = p2[2];

	if (std::abs(y1 - y0) < std::abs(x1 - x0)) {
		if (x0 > x1)
		{
			plotLineLow(x1, y1, x0, y0, color, p1, p2);
		}
		else
		{
			plotLineLow(x0, y0, x1, y1, color, p1, p2);
		}
	}
	else
	{
		if (y0 > y1) {
			plotLineHigh(x1, y1, x0, y0, color, p1, p2);
		}
		else
		{
			plotLineHigh(x0, y0, x1, y1, color, p1, p2);
		}
	}
}

void Renderer::plotLineLow(int x0, int y0, int  x1, int y1, const glm::vec3& color, glm::vec3 p1, glm::vec3 p2)
{
	int diffX = x1 - x0;
	int diffY = y1 - y0;

	glm::vec2 v1 = glm::vec2(p1.x, p1.y);
	glm::vec2 v2 = glm::vec2(p2.x, p2.y);
	float totaldist = glm::distance(v1, v2);
	int yi = 1;

	if (diffY < 0) {
		yi = -1;
		diffY = -diffY;
	}

	int d = (2 * diffY) - diffX;
	int y = y0;

	for (int x = x0;x <= x1; x++) {
		
		glm::vec2 temp = glm::vec2(x, y);
		float tempDist1= glm::distance(v1, temp);
		float tempDist2 = glm::distance( v2, temp);
		float normalDist1 = tempDist1 / totaldist;
		float normalDist2 = tempDist2 / totaldist;
		float z = normalDist1 * p1.z + normalDist2 * p2.z;
		PutPixel(x, y, z, color);
		if (d > 0) {
			y = y + yi;
			d = d + (2 * (diffY - diffX));
		}
		else
		{
			d = d + 2 * diffY;
		}
	}
}

void Renderer::plotLineHigh(int x0, int y0, int x1, int y1, const glm::vec3& color, glm::vec3 p1, glm::vec3 p2)
{
	int diffX = x1 - x0;
	int diffY = y1 - y0;

	glm::vec2 v1 = glm::vec2(p1.x, p1.y);
	glm::vec2 v2 = glm::vec2(p2.x, p2.y);
	float totaldist = glm::distance(v1, v2);

	int xi = 1;

	if (diffX < 0) {
		xi = -1;
		diffX = -diffX;
	}

	int d = (2 * diffX) - diffY;
	int x = x0;

	for (int y = y0; y <= y1; y++) {
		
		glm::vec2 temp = glm::vec2(x, y);
		float tempDist1 = glm::distance(v1, temp);
		float tempDist2 = glm::distance(temp, v2);
		float normalDist1 = tempDist1 / totaldist;
		float normalDist2 = tempDist2 / totaldist;
		float z = normalDist1 * p1.z + normalDist2 * p2.z;
		PutPixel(x, y, z, color);
		if (d > 0) {
			x = x + xi;
			d = d + (2 * (diffX - diffY));
		}
		else
		{
			d = d + 2 * diffX;
		}
	}
}

void Renderer::DrawTriangle(const glm::fvec3& v1, const glm::fvec3& v2, const glm::fvec3& v3, const glm::vec3& color ,const  MeshModel& mesh, const Scene& scene ,const Face& face)
{
	


	//here we need to distinguise between the three types of shading
	if (mesh.modelType == MeshModel::modelType::Light) {

		ScanConversionTriangle(v1, v2, v3, color);
	}
	else {
		if (mesh.shadingType == MeshModel::shadingType::Flat) {
			ScanConversionTriangleFlatShading(v1, v2, v3, mesh , scene , face);
		}
		else if (mesh.shadingType == MeshModel::shadingType::Gauraud) {
			ScanConversionTriangleGouraudShading(v1, v2, v3, mesh, scene , face);
		}
		else if (mesh.shadingType == MeshModel::shadingType::Phong) {
			ScanConversionTrianglePhongShading(v1, v2, v3, mesh, scene , face);
		}
		else if (mesh.shadingType == MeshModel::shadingType::None) {
			ScanConversionTriangle(v1, v2, v3, color);
		}
	}

	if (scene.wireFrame) {
		DrawLine(v1, v2, glm::vec3(0, 0, 0));
		DrawLine(v2, v3, glm::vec3(0, 0, 0));
		DrawLine(v1, v3, glm::vec3(0, 0, 0));
	}
	
	
}

void Renderer::DrawBoundingBox(MeshModel& model, const Scene& scene, glm::fmat4x4 trasformation , const glm::vec3& color)
{
	float deltaX = (model.maxX_ - model.minX_) /2;
	float deltaY = (model.maxY_ - model.minY_) / 2;
	float deltaZ = (model.maxZ_ - model.minZ_) / 2;
	glm::fvec3 vecArray[8] = {
	glm::fvec3(deltaX, deltaY, deltaZ),
	glm::fvec3(deltaX, deltaY, -deltaZ),
	glm::fvec3(deltaX, -deltaY, deltaZ),
	glm::fvec3(deltaX, -deltaY, -deltaZ),
	glm::fvec3(-deltaX, deltaY, deltaZ),
	glm::fvec3(-deltaX, deltaY, -deltaZ),
	glm::fvec3(-deltaX, -deltaY, deltaZ),
	glm::fvec3(-deltaX, -deltaY, -deltaZ)
	};
	
	for (int i = 0; i < 8; i++) {
		vecArray[i] =  Utils::applyTransformationToVector(vecArray[i], trasformation);
	}
	/*cubes look like this:
	   e-------f
	  /|      /|
	 / |#1   / |
	a--|----b  |
	|  g----|--h
	| / #2   | /
	c-------d*/

	//face #1 up
	DrawLine(vecArray[0], vecArray[1], color);
	DrawLine(vecArray[0], vecArray[2], color);
	DrawLine(vecArray[3], vecArray[1], color);
	DrawLine(vecArray[3], vecArray[2], color);

	//face #2 down
	DrawLine(vecArray[7], vecArray[6], color);
	DrawLine(vecArray[7], vecArray[5], color);
	DrawLine(vecArray[4], vecArray[6], color);
	DrawLine(vecArray[4], vecArray[5], color);

	//connectors of two faces
	DrawLine(vecArray[0], vecArray[4], color);
	DrawLine(vecArray[1], vecArray[5], color);
	DrawLine(vecArray[2], vecArray[6], color);
	DrawLine(vecArray[3], vecArray[7], color);

}

void Renderer::allocateZBuffer()
{
	Zbuffer = new float* [viewport_width_];
	for (int i = 0; i < viewport_width_; i++)
		Zbuffer[i] = new float[viewport_height_];

	for (int i = 0; i < viewport_width_; i++)
		for (int j = 0; j < viewport_height_; j++)
			Zbuffer[i][j] = FLT_MAX;
}

void Renderer::allocateColorBuffer()
{
	localColorBuffer = new glm::vec3* [viewport_width_];
	for (int i = 0; i < viewport_width_; i++)
		localColorBuffer[i] = new glm::vec3[viewport_height_];

	for (int i = 0; i < viewport_width_; i++)
		for (int j = 0; j < viewport_height_; j++)
			localColorBuffer[i][j] = glm::vec3(0,0,0);
}

void Renderer::PostProcessing()
{
	/*
	* for gaussian blurring
	*/
	if (scene.gaussianBlurring) {
		float** GaussianMask = GetGaussianMask(scene.maskRadius, scene.gaussianSTD);
		applyConvolution(localColorBuffer,GaussianMask, scene.maskRadius);
		for (int i = 0; i < 2* scene.maskRadius +1; i++)
		{
			delete[] GaussianMask[i];
		}
		delete[] GaussianMask;

	}




	/*
	* creating bloom effect
	*/
	else if (scene.bloom) {
		int radius = 2;
		float STD = 3.0f;
		glm::vec3 brightnessVec = glm::vec3(0.2126, 0.7152, 0.0722);
		float PixelBrightness=0;
		float** GaussianMask = GetGaussianMask(radius, STD);

		/*
		* allocating the temporary color buffer
		*/
		glm::vec3** tempImage = new glm::vec3 * [viewport_width_];
		for (int i = 0; i < viewport_width_; i++)
		{
			tempImage[i] = new glm::vec3[viewport_height_];
		}

		/*
		* initializing the temporary color buffer
		*/
		for (int i = 0; i < viewport_width_; i++)
		{
			for (int j = 0; j < viewport_height_; j++)
			{
				tempImage[i][j] = glm::vec3(0, 0, 0);
			}
		}

		/*
		* assings RGB values from local color buffer when the brightness is above the threshold
		*/
		for (int i = 0; i < viewport_width_; i++)
		{
			for (int j = 0; j < viewport_height_; j++)
			{
				PixelBrightness = glm::dot(localColorBuffer[i][j], brightnessVec);
				if (scene.threshold < PixelBrightness) {
					tempImage[i][j] = localColorBuffer[i][j];
				}
			}
		}
		for (int i = 0; i < 3; i++)
		{
			applyConvolution(tempImage, GaussianMask, radius);
		}
		//applyConvolution(tempImage, GaussianMask, radius);

		


		for (int i = 0; i < viewport_width_; i++)
		{
			for (int j = 0; j < viewport_height_; j++)
			{
				if(tempImage[i][j] != glm::vec3(0,0,0))
					localColorBuffer[i][j] = glm::clamp(localColorBuffer[i][j] + tempImage[i][j], 0.0f, 1.0f);
			}

		}
		for (int i = 0; i < 2 * scene.maskRadius + 1; i++)
		{
			delete[] GaussianMask[i];
		}
		delete[] GaussianMask;

		for (int i = 0; i < viewport_width_; i++)
		{
			delete[] tempImage[i];
		}
		delete[] tempImage;
		
	}




	else if (scene.fogEffect) {
		switch (scene.fogType)
		{
		case(1):
			applyLinearFogging();
			break;
		case(2):
			applyExponentialFogging();
			break;
		case(3):
			applyExponentialSquaredFogging();
			break;
		default:
			break;
		}
	}

	else if(scene.grayScales)
	{
		float grayScaleColor;
		glm::vec3 brightnessVec = glm::vec3(0.2126, 0.7152, 0.0722);
		for (int i = 0; i < viewport_width_; i++)
		{
			for (int j = 0; j < viewport_height_; j++) {
				grayScaleColor = glm::dot(localColorBuffer[i][j], brightnessVec);
				localColorBuffer[i][j] = glm::vec3(grayScaleColor, grayScaleColor, grayScaleColor);
			}
		}
	}

	UpdatePutPixel();
}


float** Renderer::GetGaussianMask(int radius, float STD) {
	int maskSize = 2 * radius + 1;
	float sum = 0;

	/*
	* allocate new mask
	*/
	float** mask = new float* [maskSize];
	for (int i = 0; i < maskSize; i++) {
		mask[i] = new float[maskSize];
	}

	/*
	* initializing the mask
	*/
	for (int i = 0; i < maskSize; i++)
	{
		for (int j = 0; j < maskSize; j++) {
			mask[i][j] = 0;
		}
	}

	/*
	* compute the mask values by gaussian formula
	*/
	for (int i = -1*radius; i <= radius; i++)
	{
		for (int j = -1 * radius; j <= radius; j++) {
			//mask(i+maskRadiusX+1, j+maskRadiusY+1) = exp(-1*((i^2 + j^2)/2*(maskSTD^2))); %compute the gaussed value of each coordinate in the mask by formula

			mask[i+radius][j+radius] = std::exp( -1 * (std::pow(i , 2) + std::pow(j , 2)) / (2 * (std::pow(STD,2))));
			sum += mask[i + radius ][j + radius ];
		}
	}

	/*
	* normalizing the mask factors
	*/
	for (int i = 0; i < maskSize; i++)
	{
		for (int j = 0; j < maskSize; j++) {
			mask[i][j] /= sum;
		}
	}

	return mask;
	
}

void Renderer::applyConvolution(glm::vec3** im, float** mask, int radius)
{
	int size = 2 * radius + 1;
	glm::vec3 sum;
	for (int i = 0; i < viewport_width_; i++)
	{
		for (int j = 0; j < viewport_height_; j++)
		{
			sum = glm::vec3(0, 0, 0);
			for (int k = 0; k < size; k++)
			{
				for (int t = 0; t < size; t++)
				{
					if ((i-radius+k)>=0 && (j-radius+t)>=0 && (i-radius+k)<viewport_width_ && (j-radius+t)<viewport_height_)
					{
						sum += mask[k][t] * im[i - radius + k][j - radius + t];
					}
				}
			}
			im[i][j] = sum;
		}

	}
}

void Renderer::applyLinearFogging()
{
	float f;
	float dist;
	for (int i = 0; i < viewport_width_; i++)
	{
		for (int j = 0; j < viewport_height_; j++)
		{
			//dist = glm::distance(scene.GetActiveCamera().getEye(), glm::vec3(i, j, Zbuffer[i][j]));

			glm::vec3 CC = scene.GetActiveCamera().getCenter();
			dist = glm::distance(CC[2],  Zbuffer[i][j]);
			f = (scene.fogEnd - dist) / (scene.fogEnd - scene.fogStart);
			localColorBuffer[i][j] = glm::clamp(f * localColorBuffer[i][j] + (1 - f) * scene.fogColor, 0.0f, 1.0f);
		}

	}
	
}

void Renderer::applyExponentialFogging()
{
	float f;
	float dist;
	for (int i = 0; i < viewport_width_; i++)
	{
		for (int j = 0; j < viewport_height_; j++)
		{
			//dist = glm::distance(scene.GetActiveCamera().getCenter(), glm::vec3(i, j, Zbuffer[i][j]));
			glm::vec3 CC = scene.GetActiveCamera().getCenter();
			dist = glm::distance(CC[2], Zbuffer[i][j]) *100;
			f = std::exp(-1 * dist * (scene.fogDensity * 0.01));
			glm::clamp(f, 0.0f, 1.0f);
			localColorBuffer[i][j] = glm::clamp(f * localColorBuffer[i][j] + (1 - f) * scene.fogColor, 0.0f, 1.0f);
		}

	}
}

void Renderer::applyExponentialSquaredFogging()
{
	float f;
	float dist;
	for (int i = 0; i < viewport_width_; i++)
	{
		for (int j = 0; j < viewport_height_; j++)
		{
			glm::vec3 CC = scene.GetActiveCamera().getCenter();
			dist = glm::distance(CC[2],  Zbuffer[i][j]);
			f = std::exp(-1* std::pow(dist * (scene.fogDensity),2));
			glm::clamp(f, 0.0f, 0.009f);
			localColorBuffer[i][j] = glm::clamp(f * localColorBuffer[i][j] + (1 - f) * scene.fogColor, 0.0f, 1.0f);
		}

	}
}



void Renderer::ScanConversionTriangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 color)
{
	float minX = getMin(p1.x, p2.x, p3.x);
	float maxX = getMax(p1.x, p2.x, p3.x);
	float minY = getMin(p1.y, p2.y, p3.y);
	float maxY = getMax(p1.y, p2.y, p3.y);

	for (int i = minX; i < maxX; i++) {
		for (int j = minY; j < maxY; j++) {
			if (isInsideTheTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, i, j)) {
				if (scene.isTexture) {
					//float noise1 = (std::rand() % 255) * std::exp(-1 * (scene.GetActiveModel().textureFactor + 7));
					float x = pow(rand(), 2) + std::pow(rand(), 2);
					float factor = scene.textureFactor * 0.0001f;
					float noise1 = glm::clamp(x, 0.0f, factor);
					color = glm::clamp(glm::vec3(color.x + noise1, color.y + noise1, color.z + noise1), 0.0f, 1.0f);
				}
				PutPixel(i, j, ZpointComputation(p1, p2, p3, glm::vec2(i, j)), color);
			}
		}

	}

}

float Renderer::getMin(float x, float y, float z)
{
	float temp;
	if (x < y)
		temp = x;
	else
	{
		temp = y;
	}
	if (z < temp)
		temp = z;
	return temp;
}

float Renderer::getMax(float x, float y, float z)
{
	float temp;
	if (x > y)
		temp = x;
	else
	{
		temp = y;
	}
	if (z > temp)
		temp = z;
	return temp;
}

/*
* Checking on which side of the half-plane created by the edges the point is
*/
bool Renderer::isInsideTheTriangle(float x1, float y1, float x2, float y2, float x3, float y3, int x, int y)
{


	float d1, d2, d3;
	bool has_neg, has_pos;

	d1 = area(x, y, x1, y1, x2, y2);
	d2 = area(x, y, x2, y2, x3, y3);
	d3 = area(x, y, x3, y3, x1, y1);

	has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
	has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

	return !(has_neg && has_pos);
}

glm::vec3 Renderer::getDirectionVector(glm::vec3 v1, glm::vec3 v2)
{
	glm::vec3 directionVector = v1 - v2;
	float min = getMin(std::abs(directionVector.x), std::abs(directionVector.y), std::abs(directionVector.z));
	if (min == 0)
		return directionVector;
	directionVector = glm::vec3(directionVector[0] / min, directionVector[1] / min, directionVector[2] / min);
	return directionVector;
}

float Renderer::ZpointComputation(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 insidePoint)
{
	
	glm::fvec3 weights = Utils::triangleInterpolation(p1, p2, p3, insidePoint);

	float z = p1.z * weights[0] + p2.z * weights[1] + p3.z * weights[2];
	return z;
}

float Renderer::area(int x1, int y1, int x2, int y2, int x3, int y3)
{

	return (x1 - x3) * (y2 - y3) - (x2 - x3) * (y1 - y3);

}

void Renderer::ScanConversionTriangleFlatShading(const glm::fvec3& v1, const glm::fvec3& v2, const glm::fvec3& v3, const MeshModel& mesh, const Scene& scene , const Face& face)
{
	
	

	glm::vec3 vectorArray[3];

	for (int k = 0; k < 3; k++) {
		int index = face.GetVertexIndex(k) - 1;
		glm::vec3 v = mesh.GetVertexAtIndex(index);
		vectorArray[k] = Utils::applyTransformationToVector(v, mesh.getNormalTransformation());
	}
	glm::fvec3 tringlrCenter = (vectorArray[0] + vectorArray[1] + vectorArray[2]) / 3.0f;

	glm::fvec3 color = glm::fvec3(0, 0, 0);
	for (int k = 0; k < scene.GetLightCount(); k++) {
		Light light = scene.GetLight(k);
		//(const MeshModel& mesh, const glm::fvec3 Normal, const glm::fvec3 MeshPoint, const glm::fvec3 modelcenter, const  glm::fvec3 lightcenter, const glm::fvec3 cameraCenter, const float Alpha )
		glm::fvec3 LC = light.getCenter();
		glm::fvec3 MC = mesh.getCenter();
		glm::fvec3 CC = scene.GetActiveCamera().getCenter();
		color += light.calculateColor(mesh, face.getFaceNormal(), tringlrCenter, mesh.getCenter(),  light.getCenter(), scene.GetActiveCamera().getCenter(), light.alpha);

		
		//DrawLine(LC, MC, glm::fvec3(0.5, 0.8, 0.8));
		//DrawLine(CC, MC, glm::fvec3(1,1,1));
		for (int i = 0; i < 3; i++) {
			if (color[i] > 1.0f) {
				color[i] = 1;
			}
		}
	}

	float minX = getMin(v1.x, v2.x, v3.x);
	float maxX = getMax(v1.x, v2.x, v3.x);
	float minY = getMin(v1.y, v2.y, v3.y);
	float maxY = getMax(v1.y, v2.y, v3.y);

	for (int i = minX; i < maxX; i++) {
		for (int j = minY; j < maxY; j++) {
			if (isInsideTheTriangle(v1.x, v1.y, v2.x, v2.y, v3.x, v3.y, i, j)) {
				if (scene.isTexture) {
					float x = pow(rand(), 2) + std::pow(rand(), 2);
					float factor = scene.textureFactor * 0.0001f;
					float noise1 = glm::clamp(x, 0.0f, factor);
					color = glm::clamp(glm::vec3(color.x + noise1, color.y + noise1, color.z + noise1), 0.0f, 1.0f);
				}
				PutPixel(i, j, ZpointComputation(v1, v2, v3, glm::vec2(i, j)), color);
			}
		}

	}
}

void Renderer::ScanConversionTriangleGouraudShading(const glm::fvec3& v1, const glm::fvec3& v2, const glm::fvec3& v3, const MeshModel& mesh, const Scene& scene , const Face& face)
{

	std::vector<glm::vec3>  normals  = mesh.getVerticesNormalsPerFace();
	glm::fmat4x4 transformation = mesh.normalTransformation;

	glm::vec3 vertices[3];

	for (int k = 0; k < 3; k++) {
		int index = face.GetVertexIndex(k) - 1;
		glm::vec3 v = mesh.GetVertexAtIndex(index);
		vertices[k] = Utils::applyTransformationToVector(v, mesh.getNormalTransformation());
	}
	glm::fvec3 normalTransformation[3];
	glm::fvec3 verticesColor[3];

	for (int k = 0; k < 3; k++) {
		int normalIndex = face.GetNormalIndex(k) - 1;
		glm::vec3 normal = normals[normalIndex];
		normal = Utils::applyTransformationToNormal(normal, transformation);
		normalTransformation[k] = normal;
	}

	for (int k = 0; k < 3; k++) {
		glm::fvec3 color = glm::fvec3(0, 0, 0);
		for (int l = 0; l < scene.GetLightCount(); l++) {
			Light light = scene.GetLight(l);

			color += light.calculateColor(mesh, normalTransformation[k], vertices[k], mesh.getCenter(), light.getCenter(), scene.GetActiveCamera().getCenter(), light.alpha);
			if (scene.isTexture) {
				float x = pow(rand(), 2) + std::pow(rand(), 2);
				float factor = scene.textureFactor * 0.0001f;
				float noise1 = glm::clamp(x, 0.0f, factor);
				color = glm::clamp(glm::vec3(color.x + noise1, color.y + noise1, color.z + noise1), 0.0f, 1.0f);
			}
			for (int i = 0; i < 3; i++) {
				if (color[i] > 1.0f) {
					color[i] = 1.0f;
				}
			}

		}
		verticesColor[k] = color;
	}
	


	float minX = getMin(v1.x, v2.x, v3.x);
	float maxX = getMax(v1.x, v2.x, v3.x);
	float minY = getMin(v1.y, v2.y, v3.y);
	float maxY = getMax(v1.y, v2.y, v3.y);

	for (int i = minX; i < maxX; i++) {
		for (int j = minY; j < maxY; j++) {
			if (isInsideTheTriangle(v1.x, v1.y, v2.x, v2.y, v3.x, v3.y, i, j)) {

				glm::fvec3 weights = Utils::triangleInterpolation(v1, v2, v3, glm::fvec2(i,j));
				glm::fvec3 color = weights[0]* verticesColor[0] + weights[1] * verticesColor[1] + weights[2] * verticesColor[2];

				
				PutPixel(i, j, ZpointComputation(v1, v2, v3, glm::vec2(i, j)), color);
			}
		}

	}
}

void Renderer::ScanConversionTrianglePhongShading(const glm::fvec3& v1, const glm::fvec3& v2, const glm::fvec3& v3, const MeshModel& mesh, const Scene& scene , const Face& face)
{
	std::vector<glm::vec3>  normals = mesh.getVerticesNormalsPerFace();
	glm::fmat4x4 transformation = mesh.normalTransformation;



	glm::fvec3 normalTransformation[3];

	for (int k = 0; k < 3; k++) {
		int normalIndex = face.GetNormalIndex(k) - 1;
		glm::vec3 normal = normals[normalIndex];
		normalTransformation[k] = Utils::applyTransformationToNormal(normal, transformation);
	}

	float minX = getMin(v1.x, v2.x, v3.x);
	float maxX = getMax(v1.x, v2.x, v3.x);
	float minY = getMin(v1.y, v2.y, v3.y);
	float maxY = getMax(v1.y, v2.y, v3.y);


	for (int i = minX; i < maxX; i++) {
		for (int j = minY; j < maxY; j++) {
			if (isInsideTheTriangle(v1.x, v1.y, v2.x, v2.y, v3.x, v3.y, i, j)) {

				glm::fvec3 weights = Utils::triangleInterpolation(v1, v2, v3, glm::fvec2(i, j));
				glm::fvec3 pointNormal  = weights[0] * normalTransformation[0] + weights[1] * normalTransformation[1] + weights[2] * normalTransformation[2];
				float z = weights[0] * v1[2] + weights[1] * v2[2] + weights[2] * v3[2];

				glm::fvec3 color = glm::fvec3(0, 0, 0);
				for (int k = 0; k < scene.GetLightCount(); k++) {
					Light light = scene.GetLight(k);
					
					glm::fvec3 meshPoint = glm::fvec3(i, j, z);
					glm::fmat4x4 inverseAfterProjection= glm::inverse(afterProjectionMatrix);
					meshPoint = Utils::applyTransformationToVector(meshPoint, inverseAfterProjection);

					color += light.calculateColor(mesh, pointNormal, meshPoint, mesh.getCenter(), light.getCenter(), scene.GetActiveCamera().getCenter(), light.alpha);
					if (scene.isTexture) {
						float x = pow(rand(), 2) + std::pow(rand(), 2);
						float factor = scene.textureFactor * 0.0001f;
						float noise1 = glm::clamp(x, 0.0f, factor);
						color = glm::clamp(glm::vec3(color.x + noise1, color.y + noise1, color.z + noise1), 0.0f, 1.0f);
					}
					for (int i = 0; i < 3; i++) {
						if (color[i] > 1.0f) {
							color[i] = 1;
						}
					}

				}
				
				PutPixel(i, j, ZpointComputation(v1, v2, v3, glm::vec2(i, j)), color);
			}
		}

	}
}

glm::vec3 Renderer::DrawFaceNormal(MeshModel& mesh, Face& face, glm::fmat4x4 trasformation, glm::fmat4x4 FinalTrasformation, const glm::vec3& color)
{
		glm::vec3 vectorArray[3];

		for (int k = 0; k < 3; k++) {
			int index = face.GetVertexIndex(k) - 1;
			vectorArray[k] = mesh.GetVertexAtIndex(index);
		}

		//glm::fvec3 v0 = Utils::applyTransformationToVector(vectorArray[0], trasformation);
		//glm::fvec3 v1 = Utils::applyTransformationToVector(vectorArray[1], trasformation);
		//glm::fvec3 v2 = Utils::applyTransformationToVector(vectorArray[2], trasformation);
		
		glm::fvec3 v0 = vectorArray[0];
		glm::fvec3 v1 = vectorArray[1];
		glm::fvec3 v2 = vectorArray[2];

		glm::fvec3 ActualCenter = (v0 + v1 + v2) / 3.0f; //center of face
		ActualCenter = Utils::applyTransformationToVector(ActualCenter, FinalTrasformation);

		glm::vec3 Actualnormal = glm::normalize(glm::cross((v1 - v0), (v2 - v0)));

		Actualnormal = Utils::applyTransformationToNormal(Actualnormal, trasformation);

		glm::vec3 normalizedNormal = Actualnormal;
		
		Actualnormal = Actualnormal * mesh.FaceNormalsLength;

		//face normals check
		if (mesh.displayFaceNormals) {
			DrawLine(ActualCenter, ActualCenter + Actualnormal, glm::fvec3(1, 1, 1));
		}
		return normalizedNormal;
}

void Renderer::DrawVerticesNormal(MeshModel& mesh, glm::fmat4x4 trasformation, const glm::vec3& color , float normalLength)
{
	for (int i = 0; i < mesh.GetVerticesCount(); i++) {
		glm::fvec3 v = mesh.GetVertexAtIndex(i);
		v = Utils::applyTransformationToVector(v, trasformation);

		glm::fvec3 vn = mesh.getVerticesNormals()[i];
		vn = vn * normalLength;
		DrawLine(v, v + vn, color);
	}
	
}

void Renderer::DrawVerticesNormalPerFace(MeshModel& mesh, glm::fmat4x4 trasformation, const glm::vec3& color, float normalLength)
{
	std::vector<Face> faces = mesh.getFaces();
	std::vector < glm::vec3 > normals = mesh.getVerticesNormalsPerFace();

	for (int j = 0; j < mesh.GetFacesCount(); j++)
	{
		Face& face = faces[j];

		//extract vertices of face
		for (int k = 0; k < 3; k++) {
			int index = face.GetVertexIndex(k) - 1;
			glm::vec3 v = mesh.GetVertexAtIndex(index);
			glm::vec3 vertex = Utils::applyTransformationToVector(v, trasformation);

			int normalIndex = face.GetNormalIndex(k) - 1;
			glm::vec3 normal = normals[normalIndex];
			normal = Utils::applyTransformationToNormal(normal,trasformation) * normalLength;
			DrawLine(vertex, vertex + normal, color);
		}

	}
}

void Renderer::CreateBuffers(int w, int h)
{
	CreateOpenGLBuffer(); //Do not remove this line.
	color_buffer_ = new float[3 * w * h];
	allocateZBuffer();
	allocateColorBuffer();
	ClearColorBuffer(glm::vec3(0.0f, 0.0f, 0.0f));
}

//##############################
//##OpenGL stuff. Don't touch.##
//##############################

// Basic tutorial on how opengl works:
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
// don't linger here for now, we will have a few tutorials about opengl later.
void Renderer::InitOpenGLRendering()
{
	// Creates a unique identifier for an opengl texture.
	glGenTextures(1, &gl_screen_tex_);

	// Same for vertex array object (VAO). VAO is a set of buffers that describe a renderable object.
	glGenVertexArrays(1, &gl_screen_vtc_);

	GLuint buffer;

	// Makes this VAO the current one.
	glBindVertexArray(gl_screen_vtc_);

	// Creates a unique identifier for a buffer.
	glGenBuffers(1, &buffer);

	// (-1, 1)____(1, 1)
	//	     |\  |
	//	     | \ | <--- The exture is drawn over two triangles that stretch over the screen.
	//	     |__\|
	// (-1,-1)    (1,-1)
	const GLfloat vtc[]={
		-1, -1,
		 1, -1,
		-1,  1,
		-1,  1,
		 1, -1,
		 1,  1
	};

	const GLfloat tex[]={
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1};

	// Makes this buffer the current one.
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// This is the opengl way for doing malloc on the gpu. 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc)+sizeof(tex), NULL, GL_STATIC_DRAW);

	// memcopy vtc to buffer[0,sizeof(vtc)-1]
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);

	// memcopy tex to buffer[sizeof(vtc),sizeof(vtc)+sizeof(tex)]
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

	// Loads and compiles a sheder.
	GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );

	// Make this program the current one.
	glUseProgram(program);

	// Tells the shader where to look for the vertex position data, and the data dimensions.
	GLint  vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition,2,GL_FLOAT,GL_FALSE,0,0 );

	// Same for texture coordinates data.
	GLint  vTexCoord = glGetAttribLocation( program, "vTexCoord" );
	glEnableVertexAttribArray( vTexCoord );
	glVertexAttribPointer( vTexCoord,2,GL_FLOAT,GL_FALSE,0,(GLvoid *)sizeof(vtc) );

	//glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );

	// Tells the shader to use GL_TEXTURE0 as the texture id.
	glUniform1i(glGetUniformLocation(program, "texture"),0);
}

void Renderer::CreateOpenGLBuffer()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, gl_screen_tex_);

	// malloc for a texture on the gpu.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, viewport_width_, viewport_height_, 0, GL_RGB, GL_FLOAT, NULL);
	glViewport(0, 0, viewport_width_, viewport_height_);
}

void Renderer::SwapBuffers()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, gl_screen_tex_);

	// memcopy's colorBuffer into the gpu.
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, viewport_width_, viewport_height_, GL_RGB, GL_FLOAT, color_buffer_);

	// Tells opengl to use mipmapping
	glGenerateMipmap(GL_TEXTURE_2D);

	// Make glScreenVtc current VAO
	glBindVertexArray(gl_screen_vtc_);

	// Finally renders the data.
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::ClearColorBuffer(const glm::vec3& color)
{
	for (int i = 0; i < viewport_width_; i++)
	{
		for (int j = 0; j < viewport_height_; j++)
		{
			Zbuffer[i][j] = FLT_MAX;
			PutPixel(i, j, 0 , color);
		}
	}
}

void Renderer::Render(const Scene& scene)
{
	
	int windowsWidth = viewport_width_;
	int windowsHeight = viewport_height_;

	int centerX = windowsWidth / 2;
	int centerY = windowsHeight / 2;

	// updating camera params;
	Camera& cam = scene.GetActiveCamera();
	cam.right = (float)centerX;
	cam.left = -(float)centerX;
	cam.top = (float)centerY;
	cam.bottom = -(float)centerY;
	cam.aspectRatio = windowsWidth / windowsHeight;
	if(cam.GetOrthographicOrPerspective()){
		cam.SetViewVolumeCoordinates(cam.right, cam.left, cam.top, cam.bottom, cam.GetNear(), cam.GetFar());
	}
	else {
		cam.SetPerspectiveData(cam.GetNear(), cam.GetFar(), cam.GetFovy(), cam.GetAspectRatio());
	}
	


	glm::fmat4x4 translateAfterProjection = Utils::TransformationTransition(glm::fvec3(1, 1, 0));
	glm::fmat4x4 scaleAfterProjection = Utils::TransformationScale(glm::fvec3(centerX, centerY, 1));

	glm::fmat4x4 AfterProjection = scaleAfterProjection * translateAfterProjection;
	afterProjectionMatrix = AfterProjection;
	if (scene.getShowAxis()) {
		DrawLine(glm::fvec3(0, centerY, 0), glm::fvec3(windowsWidth, centerY, 0), glm::fvec3(0, 0, 0));
		DrawLine(glm::fvec3(centerX, 0, 0), glm::fvec3(centerX, windowsHeight, 0), glm::fvec3(0, 0, 0));
	}
	
	//rendering the MeshModels
	if (scene.GetModelCount() > 0) {
		for (int i = 0; i < scene.GetModelCount(); i++)
		{
			MeshModel& mesh = scene.GetModel(i);
			float proportion = 300.f/mesh.getMaxDitancePoints();

			
			glm::fmat4x4 scale = Utils::TransformationScale(glm::fvec3(proportion, proportion, proportion));
			glm::fmat4x4 translate = Utils::TransformationTransition(glm::fvec3(centerX, centerY, 0));

			glm::fmat4x4 transformationMatrix = mesh.getTransformation();


			glm::fmat4x4 finalTransformation =   transformationMatrix * scale   ;

			//rendering active camera view
			Camera& currentCam = scene.GetActiveCamera();
			glm::fmat4x4 inverserCameraTransformation = glm::lookAt(currentCam.getEye(), currentCam.getAt(), currentCam.getUp());
			glm::fmat4x4 viewVolumeTransformation= currentCam.GetViewTransformation();

			glm::fmat4x4 CameraTransformation = viewVolumeTransformation * inverserCameraTransformation;
			finalTransformation = CameraTransformation * finalTransformation;
			glm::mat4x4 normalMatrix = finalTransformation;
			

			mesh.normalTransformation = finalTransformation;
			//transfer objects to center screen with transalte transformation
			finalTransformation = AfterProjection * finalTransformation;
			mesh.finalTransformation = finalTransformation;
			//bounding box check

			if (mesh.displayBoundingBox) {
				DrawBoundingBox(mesh, scene, finalTransformation, glm::vec3(0, 0, 1));
			}

			std::vector<Face> faces = mesh.getFaces();

			std::vector<glm::vec3> verticesTransformation;
			for (int j = 0; j < mesh.GetVerticesCount(); j++)
			{
				glm::fvec3  vertex = mesh.GetVertexAtIndex(j);
				glm::fvec3 transformedVertex = Utils::applyTransformationToVector(vertex, finalTransformation); 
				verticesTransformation.push_back(transformedVertex);
			}

			
			//draw faces
			for (int j = 0; j < mesh.GetFacesCount(); j++)
			{
				Face& face = faces[j];

				glm::vec3 vectorArray[3];

				//extract verices of face
				for (int k = 0; k < 3; k++) {
					int index = face.GetVertexIndex(k) - 1;
					vectorArray[k] = verticesTransformation[index];
				}

				
				glm::fvec3  faceNormal = DrawFaceNormal(mesh ,face, normalMatrix, finalTransformation, glm::vec3(1, 0, 1));
				mesh.setFaceNormal(j ,faceNormal);
				
				
				if (scene.GetActiveCamera().GetOrthographicOrPerspective() == false) {
					glm::vec3 triangleCenter = (vectorArray[0] + vectorArray[1] + vectorArray[2]) / 3.0f;
					glm::vec3 at = scene.GetActiveCamera().getAt();
					float zAt = at.z;
					glm::vec3 camCenter = scene.GetActiveCamera().getCenter();
					if ((camCenter[2] + zAt * triangleCenter[2]) < (camCenter.z + zAt * scene.GetActiveCamera().GetNear())) {
						continue;
					}
				}
				DrawTriangle(vectorArray[0], vectorArray[1], vectorArray[2], mesh.GetColor(), mesh , scene , face);
			}

			//call function 
			mesh.setVerteciesNormals();

			//vertices normals check
			if (mesh.displayVerticesNormals) {
				DrawVerticesNormal(mesh, finalTransformation, glm::vec3(0, 0, 0.545), mesh.VerticesNormalsLength);
			}

			//vertices normals per face check
			if (mesh.displayVerticesNormalsPerFace) {
				DrawVerticesNormalPerFace(mesh, finalTransformation, glm::vec3(0, 0.545, 0.545), mesh.VerticesNormalsPerFaceLength);
			}
		}

	}

	//rendering the  camers to the screen
	if (scene.GetCameraCount() > 0 ) {
		for (int i = 0; i < scene.GetCameraCount(); i++) {

			
			Camera& tempCam = scene.GetCamera(i);
			float proportion = 50.0f / tempCam.getMaxDitancePoints();

			glm::fmat4x4 scale = Utils::TransformationScale(glm::fvec3(proportion, proportion, proportion));
			glm::fmat4x4 translate = Utils::TransformationTransition(glm::fvec3(centerX, centerY, 0));

			glm::fmat4x4 transformationMatrix;

			//check if we are using LookAt or Transformation
			if (tempCam.GetLookAtOrTransformation() == true) {
				transformationMatrix = tempCam.getTransformation();
			}
			else {
				transformationMatrix = glm::inverse(glm::lookAt(tempCam.getEye(), tempCam.getAt(), tempCam.getUp()));
			}

			glm::fmat4x4 finalTransformation =  transformationMatrix * scale;

			

			//rendering active camera view
			Camera& currentCam = scene.GetActiveCamera();
			glm::fmat4x4 inverserCameraTransformation = glm::lookAt(currentCam.getEye(), currentCam.getAt(), currentCam.getUp());
			glm::fmat4x4 viewVolumeTransformation;
			viewVolumeTransformation = currentCam.GetViewTransformation();
			glm::fmat4x4 CameraTransformation = viewVolumeTransformation * inverserCameraTransformation;
			finalTransformation =   CameraTransformation * finalTransformation;

			tempCam.normalTransformation = finalTransformation;
			finalTransformation = AfterProjection * finalTransformation;
			tempCam.finalTransformation = finalTransformation;

			
			if (scene.GetActiveCameraIndex() == i) {
				continue;
			}
			std::vector<Face> faces = tempCam.getFaces();

			for (int j = 0; j < tempCam.GetFacesCount(); j++)
			{
				Face& face = faces[j];

				glm::vec3 vectorArray[3];

				for (int k = 0; k < 3; k++) {
					int index = face.GetVertexIndex(k) - 1;
					glm::vec3 v = tempCam.GetVertexAtIndex(index);
					vectorArray[k] = Utils::applyTransformationToVector(v, finalTransformation);
				}

				DrawTriangle(vectorArray[0], vectorArray[1], vectorArray[2], glm::vec3(0.5f, 0.5f, 0.5f), tempCam, scene, face);

			}
		}
	}

	//rendering the  lights to the screen
	if (scene.GetLightCount() > 0 ) {
		for (int i = 0; i < scene.GetLightCount(); i++) {
			Light& tempLight = scene.GetLight(i);
			float proportion = 50.0f / tempLight.getMaxDitancePoints();

			glm::fmat4x4 scale = Utils::TransformationScale(glm::fvec3(proportion, proportion, proportion));
			glm::fmat4x4 translate = Utils::TransformationTransition(glm::fvec3(centerX, centerY, 0));

			glm::fmat4x4 transformationMatrix = tempLight.getTransformation();

			glm::fmat4x4 finalTransformation = transformationMatrix * scale;

			if (scene.GetCamOrWorldView())  // rendering the active camera view
			{
				Camera& currentCam = scene.GetActiveCamera();
				glm::fmat4x4 inverserCameraTransformation = glm::lookAt(currentCam.getEye(), currentCam.getAt(), currentCam.getUp());
				glm::fmat4x4 viewVolumeTransformation;

				viewVolumeTransformation = currentCam.GetViewTransformation();
				glm::fmat4x4 CameraTransformation = viewVolumeTransformation * inverserCameraTransformation;
				finalTransformation = CameraTransformation * finalTransformation;
			}
			tempLight.normalTransformation = finalTransformation;
			finalTransformation = AfterProjection * finalTransformation;
			tempLight.finalTransformation = finalTransformation;
			
			std::vector<Face> faces = tempLight.getFaces();

			for (int j = 0; j < tempLight.GetFacesCount(); j++)
			{
				Face& face = faces[j];

				glm::vec3 vectorArray[3];

				for (int k = 0; k < 3; k++) {
					int index = face.GetVertexIndex(k) - 1;
					glm::vec3 v = tempLight.GetVertexAtIndex(index);
					vectorArray[k] = Utils::applyTransformationToVector(v, finalTransformation);
				}

				DrawTriangle(vectorArray[0], vectorArray[1], vectorArray[2], glm::vec3(1, 1, 1) , tempLight, scene , face);

			}
		}
	}
	PostProcessing();
	
}

int Renderer::GetViewportWidth() const
{
	return viewport_width_;
}

int Renderer::GetViewportHeight() const
{
	return viewport_height_;
}

void Renderer::SetViewport(int width, int height)
{
	viewport_height_ = height;
	viewport_width_ = width;
	CreateBuffers(width, height);
}

