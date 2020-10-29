#pragma once
#include "Scene.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

class Renderer
{
public:
	Renderer(int viewportWidth, int viewportHeight);
	virtual ~Renderer();
	void Render(const Scene& scene);
	void SwapBuffers();
	void ClearColorBuffer(const glm::vec3& color);
	int GetViewportWidth() const;
	int GetViewportHeight() const;
	
private:
	void PutPixel(const int i, const int j, const glm::vec3& color);
	void DrawLine(const glm::fvec2& p1, const glm::fvec2& p2, const glm::vec3& color);
	void plotLineLow(int x0, int y0, int  x1, int y1, const glm::vec3& color);
	void plotLineHigh(int x0, int y0, int x1, int y1, const glm::vec3& color);
	void plotTriangle(const glm::fvec2& v1, const glm::fvec2& v2, const glm::fvec2& v3, const glm::vec3& color);

	void CreateBuffers(int w, int h);
	void CreateOpenGLBuffer();
	void InitOpenGLRendering();

	float* color_buffer_;
	int viewport_width_;
	int viewport_height_;
	GLuint gl_screen_tex_;
	GLuint gl_screen_vtc_;
};
