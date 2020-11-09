#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include <iostream>

#include "Renderer.h"
#include "InitShader.h"
#include "Utils.h"

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)
#define Z_INDEX(width,x,y) ((x)+(y)*(width))

Renderer::Renderer(int viewport_width, int viewport_height) :
	viewport_width_(viewport_width),
	viewport_height_(viewport_height)
{
	InitOpenGLRendering();
	CreateBuffers(viewport_width, viewport_height);
}

Renderer::~Renderer()
{
	delete[] color_buffer_;
}

void Renderer::PutPixel(int i, int j, const glm::vec3& color)
{
	if (i < 0) return; if (i >= viewport_width_) return;
	if (j < 0) return; if (j >= viewport_height_) return;
	
	color_buffer_[INDEX(viewport_width_, i, j, 0)] = color.x;
	color_buffer_[INDEX(viewport_width_, i, j, 1)] = color.y;
	color_buffer_[INDEX(viewport_width_, i, j, 2)] = color.z;
}

void Renderer::DrawLine(const glm::fvec2& p1, const glm::fvec2& p2, const glm::vec3& color)
{

	// TODO: Implement bresenham algorithm
	// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm

	int x0 = (int)p1[0], y0 = (int)p1[1];
	int x1 = (int)p2[0], y1 = (int)p2[1];

	if (std::abs(y1 - y0) < std::abs(x1 - x0)) {
		if (x0 > x1)
		{
			plotLineLow(x1, y1, x0, y0,color);
		}
		else
		{
			plotLineLow(x0, y0, x1, y1, color);
		}
	}
	else
	{
		if (y0 > y1) {
			plotLineHigh(x1,y1,x0,y0, color);
		}
		else
		{
			plotLineHigh(x0,y0,x1,y1, color);
		}
	}
}

void Renderer::plotLineLow(int x0, int y0,int  x1, int y1, const glm::vec3& color)
{
	int diffX = x1 - x0;
	int diffY = y1 - y0;

	int yi = 1;

	if (diffY < 0) {
		yi = -1;
		diffY = -diffY;
	}

	int d = (2 * diffY) - diffX;
	int y = y0;

	for (int x = x0;x <= x1; x++) {
		PutPixel(x, y, color);
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

void Renderer::plotLineHigh(int x0, int y0, int x1, int y1, const glm::vec3& color)
{
	int diffX = x1 - x0;
	int diffY = y1 - y0;

	int xi = 1;

	if (diffX < 0) {
		xi = -1;
		diffX = -diffX;
	}

	int d = (2 * diffX) - diffY;
	int x = x0;

	for (int y = y0; y <= y1; y++) {
		PutPixel(x, y, color);
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

void Renderer::DrawTriangle(const glm::fvec2& v1, const glm::fvec2& v2, const glm::fvec2& v3, const glm::vec3& color)
{
	DrawLine( v1, v2, color);
	DrawLine(v2, v3, color);
	DrawLine(v1, v3, color);
}

void Renderer::CreateBuffers(int w, int h)
{
	CreateOpenGLBuffer(); //Do not remove this line.
	color_buffer_ = new float[3 * w * h];
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
			PutPixel(i, j, color);
		}
	}
}

void Renderer::Render(const Scene& scene)
{
	int windowsWidth = viewport_width_;
	int windowsHeight = viewport_height_;

	int centerX = windowsWidth / 2;
	int centerY = windowsHeight / 2;
	int boundingBoxEdgeLength = glm::max(centerX, centerY);
	
	if (scene.GetModelCount() > 0) {
		for (int i = 0; i < scene.GetModelCount(); i++)
		{
			MeshModel& mesh = scene.GetModel(i);
			float proportion = boundingBoxEdgeLength/mesh.getInitialScale();
			
			std::vector<Face> faces = mesh.getFaces();
			
			glm::fmat4x4 scale = Utils::TransformationScale(glm::fvec3(proportion, proportion, proportion));
			glm::fmat4x4 translate = Utils::TransformationTransition(glm::fvec3(centerX, centerY, 0));

			glm::fmat4x4 transformationMatrix = mesh.getWorldTransformation() * mesh.getObjectTransformation();



			for (int j = 0; j < mesh.GetFacesCount(); j++)
			{
				Face face = faces[j];

				int index0 = face.GetVertexIndex(0) - 1;
				glm::vec3 v0 = mesh.GetVertexAtIndex(index0);
				glm::fvec4 newv0 = Utils::Euclidean2Homogeneous(v0);

				newv0 = translate * transformationMatrix * scale * newv0;
				v0 = Utils::Homogeneous2Euclidean(newv0);
				


				int index1 = face.GetVertexIndex(1) - 1;
				glm::vec3 v1 = mesh.GetVertexAtIndex(index1);
				glm::fvec4 newv1 = Utils::Euclidean2Homogeneous(v1);

				newv1 = translate * transformationMatrix * scale * newv1;
				v1 = Utils::Homogeneous2Euclidean(newv1);
				


				int index2 = face.GetVertexIndex(2) - 1;
				glm::vec3 v2 = mesh.GetVertexAtIndex(index2);
				glm::fvec4 newv2 = Utils::Euclidean2Homogeneous(v2);

				newv2 = translate * transformationMatrix * scale * newv2;
				v2 = Utils::Homogeneous2Euclidean(newv2);
				

				DrawTriangle(v0, v1, v2, glm::vec3(1, 0, 0));

			}
		}
	}
	
	

}

int Renderer::GetViewportWidth() const
{
	return viewport_width_;
}

int Renderer::GetViewportHeight() const
{
	return viewport_height_;
}