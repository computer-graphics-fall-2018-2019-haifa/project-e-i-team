#define _USE_MATH_DEFINES

#include "Renderer.h"
#include "InitShader.h"
#include "MeshModel.h"
#include <imgui/imgui.h>
#include <vector>
#include <cmath>
#include <stdio.h>
#include <fenv.h>       /* fegetround, FE_* */
#include <math.h>       /* nearbyint */

using namespace std;

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)

static float p1 = 0, q1 = 0;

static float p2 = -50, q2 = -100;

Renderer::Renderer(int viewportWidth, int viewportHeight, int viewportX, int viewportY) :
	colorBuffer(nullptr),
	zBuffer(nullptr)
{
	initOpenGLRendering();
	SetViewport(viewportWidth, viewportHeight, viewportX, viewportY);
}

Renderer::~Renderer()
{
	if (colorBuffer)
	{
		delete[] colorBuffer;
	}
}

void Renderer::putPixel(int i, int j, const glm::vec3& color)
{
	if (i < 0) return; if (i >= viewportWidth) return;
	if (j < 0) return; if (j >= viewportHeight) return;
	colorBuffer[INDEX(viewportWidth, i, j, 0)] = color.x;
	colorBuffer[INDEX(viewportWidth, i, j, 1)] = color.y;
	colorBuffer[INDEX(viewportWidth, i, j, 2)] = color.z;
}

void Renderer::createBuffers(int viewportWidth, int viewportHeight)
{
	if (colorBuffer)
	{
		delete[] colorBuffer;
	}

	colorBuffer = new float[3* viewportWidth * viewportHeight];
	for (int x = 0; x < viewportWidth; x++)
	{
		for (int y = 0; y < viewportHeight; y++)
		{
			putPixel(x, y, glm::vec3(0.0f, 0.0f, 0.0f));
		}
	}
}

void Renderer::ClearColorBuffer(const glm::vec3& color)
{
	for (int i = 0; i < viewportWidth; i++)
	{
		for (int j = 0; j < viewportHeight; j++)
		{
			putPixel(i, j, color);
		}
	}
}

void Renderer::SetViewport(int viewportWidth, int viewportHeight, int viewportX, int viewportY)
{
	this->viewportX = viewportX;
	this->viewportY = viewportY;
	this->viewportWidth = viewportWidth;
	this->viewportHeight = viewportHeight;
	createBuffers(viewportWidth, viewportHeight);
	createOpenGLBuffer();
}
void Renderer::NaiveAlg(float p1, float p2, float q1, float q2)
{
	float delta_p = p2 - p1;
	float delta_q = q2 - q1;
	float m = delta_q / delta_p;
	float c = q1 - m * p1;
	
	float x,to,y;
	if (p1 < p2) {
		x = p1;
		to = p2;
	}
	else {
		x = p2;
		to = p1;
	}
	for (; x <= to; x++) {
		y = round(m*x + c);
		putPixel(x, 720-y, glm::vec3(0, 0, 0));
	}
}
void Renderer::DrawLine(float p1, float p2, float q1, float q2) {
	float a = (q1 - q2) / (p1 - p2);
	printf("a = %f\n", a);


	if (a >= 0 && a <= 1) {
		if (p1 < p2) {
			BresenhamAlg(p1, p2, q1, q2, false, false, false,0);
		}
		else {
			BresenhamAlg(p2, p1, q2, q1, false, false, false,0);
		}
	}
	else if (a > 1) {
		if (q1 < q2) {
			BresenhamAlg(q1, q2, p1, p2, true, false, false,0);
		}
		else {
			BresenhamAlg(q2, q1, p2, p1, true, false, false,0);
		}
	}
	else if (a < 0 && a >= -1) {
		if (p1 < p2) {
			BresenhamAlg(p1, p2, q1, q2 + 2*(q1-q2), false, true, false,0);
		}
		else {
			BresenhamAlg(p2, p1, q2, q1 + 2 * (q2 - q1), false, true, false,0);
		}
	}
	else if (a < -1) {
		if (q1 < q2) {
			BresenhamAlg(q1, q2, p1, p2+2*(p1-p2), true, true, false, 0);
			//BresenhamAlg(q2 - 2* (q2-q1), q1, p2, p1, true, false, true,p1);//q2
		}
		else {
			//BresenhamAlg(q2, q1, p2 - 2*(p2-p1), p1, true, false, true, 0);
			BresenhamAlg(q1, q2 + 2 * (q1 - q2), p1, p2, true, false, true,p1);//q1			
		}
	}

}

void Renderer::BresenhamAlg(float p1, float p2, float q1, float q2, bool switch_print, bool NegX, bool NegY,float P) {
	float x, y, e;
	float delta_p = p2 - p1;
	float delta_q = q2 - q1;
	
	x = p1;
	y = q1;

	
	e = -delta_p;
	float fthf = 2 * (p1 - p2);
	while (x <= p2) {
		if (e > 0) {

			if (NegX) {
				y--;
			}
			else {
				y++;
			}
			e = e - 2 * delta_p;
		}
		//printf("xy = (%f,%F)\n", y,x);
		if (switch_print) {
			if (NegY) {
				putPixel((viewportWidth / 2) + y , (viewportHeight / 2) - x + 2*p1, glm::vec3(0, 0, 0));
			}
			else {
				putPixel((viewportWidth / 2) + y, (viewportHeight / 2) + x, glm::vec3(0, 0, 0));
			}
		}
		else {
			putPixel((viewportWidth / 2) + x, (viewportHeight / 2) + y, glm::vec3(0, 0, 0));
		}
		
		fthf -= 2;
		x++;
		
		e = e + 2 * delta_q;
	}
}

// Elias Function Implementation:
void Renderer::BresenhamLine(float p1, float p2, float q1, float q2)
{
	float a = (q1-q2) / (p1-p2);
	float x, y, e;
	float delta_p = p2 - p1;
	float delta_q = q2 - q1;
	float target;

	printf("a = %f\n", a);
	x = p1;
	y = q1;
	target = p2;

	e = -delta_p;
	while (x <= target) {
		if (e > 0) {
			y++;
			e = e - 2 * delta_p;
		}
		printf("xy = (%f,%F)\n", x, y);
		putPixel(x, 720 - y, glm::vec3(0, 0, 0));
		x++;
		e = e + 2 * delta_q;
	}
	
	/*
	if (a >= 0 && a <= 1) {
		if (p1 < p2) {
			x = p1;
			y = q1;
			target = p2;
		}
		else {
			x = p2;
			y = q2;
			target = p1;
			delta_p = -delta_p;
			delta_q = -delta_q;
		}
		
		e = -delta_p;
		while (x <= target) {
			if (e > 0) {
				y++;
				e = e - 2 * delta_p;
			}
			printf("xy = (%f,%F)\n", x, y);
			putPixel(x, 720-y, glm::vec3(0, 0, 0));
			x++;
			e = e + 2 * delta_q;
		}
	}
	else if (a > 1) {
		if (q1 < q2) {
			x = p1;
			y = q1;
			target = q2;
		}
		else {
			x = p2;
			y = q2;
			target = q1;
			delta_p = -delta_p;
			delta_q = -delta_q;
		}

		e = -delta_q;
		while (y <= target) {
			if (e > 0) {
				x++;
				e = e - 2 * delta_q;
			}
			printf("xy = (%f,%F)\n", x, y);
			putPixel(x, 720-y, glm::vec3(0, 0, 0));
			y++;
			e = e + 2 * delta_p;
		}
	}
	*/


	/*if (a <= 0 && a >= -1) {
		if (p1 < p2) {
			x = p1;
			y = q1;
			target = p2;
		}
		else {
			x = p2;
			y = q2;
			target = p1;
			delta_p = -delta_p;
			delta_q = -delta_q;
		}

		e = -delta_p;
		while (x <= target) {
			if (e > 0) {
				y++;
				e = e - 2 * delta_p;
			}
			printf("xy = (%f,%F)\n", x, y);
			putPixel(x, 720 - y, glm::vec3(0, 0, 0));
			x++;
			e = e + 2 * delta_q;
		}
	}
	else if (a < -1) {
		if (q1 < q2) {
			x = p1;
			y = q1;
			target = q2;
		}
		else {
			x = p2;
			y = q2;
			target = q1;
			delta_p = -delta_p;
			delta_q = -delta_q;
		}

		e = -delta_q;
		while (y <= target) {
			if (e > 0) {
				x++;
				e = e - 2 * delta_q;
			}
			printf("xy = (%f,%F)\n", x, y);
			putPixel(x, 720 - y, glm::vec3(0, 0, 0));
			y++;
			e = e + 2 * delta_p;
		}
	}
	*/

}



void Renderer::Render(const Scene& scene, const ImGuiIO& io)
{
	//#############################################
	//## You should override this implementation ##
	//## Here you should render the scene.       ##
	//#############################################


	/*
	// Draw a chess board in the middle of the screen
	for (int i = 0; i < viewportWidth; i++)
	{
		for (int j = 0; j < viewportHeight; j++)
		{
			int mod_i = i / 50;
			int mod_j = j / 50;

			int odd = (mod_i + mod_j) % 2;
			if (odd)
			{
				putPixel(i, j, glm::vec3(0, 1, 0));
			}
			else
			{
				putPixel(i, j, glm::vec3(1, 0, 0));
			}
		}
	}
	*/



	p2 = io.MousePos.x - (viewportWidth/2);
	q2 = (viewportHeight/2) - io.MousePos.y;


	//printf("viewportHeight = %d , viewportWidth = %d\n", viewportHeight , viewportWidth);
	//printf("p1 = (%f,%f) , p2 = (%f,%f)\n", p1, q1, p2, q2);
	DrawLine(p1, p2, q1, q2);
	DrawLine(-(viewportWidth/2), (viewportWidth / 2), 0, 0);
	DrawLine(0, 0, (viewportHeight / 2), -(viewportHeight / 2));
}

//##############################
//##OpenGL stuff. Don't touch.##
//##############################

// Basic tutorial on how opengl works:
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
// don't linger here for now, we will have a few tutorials about opengl later.
void Renderer::initOpenGLRendering()
{
	// Creates a unique identifier for an opengl texture.
	glGenTextures(1, &glScreenTex);

	// Same for vertex array object (VAO). VAO is a set of buffers that describe a renderable object.
	glGenVertexArrays(1, &glScreenVtc);

	GLuint buffer;

	// Makes this VAO the current one.
	glBindVertexArray(glScreenVtc);

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

void Renderer::createOpenGLBuffer()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, glScreenTex);

	// malloc for a texture on the gpu.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, viewportWidth, viewportHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glViewport(0, 0, viewportWidth, viewportHeight);
}

void Renderer::SwapBuffers()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, glScreenTex);

	// memcopy's colorBuffer into the gpu.
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, viewportWidth, viewportHeight, GL_RGB, GL_FLOAT, colorBuffer);

	// Tells opengl to use mipmapping
	glGenerateMipmap(GL_TEXTURE_2D);

	// Make glScreenVtc current VAO
	glBindVertexArray(glScreenVtc);

	// Finally renders the data.
	glDrawArrays(GL_TRIANGLES, 0, 6);
}