#define _USE_MATH_DEFINES

#include "Renderer.h"
#include "InitShader.h"
#include "MeshModel.h"
#include <imgui/imgui.h>
#include <vector>
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <fenv.h>       /* fegetround, FE_* */
#include <math.h>       /* nearbyint */
#include "Trans.h"

using namespace std;

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)

static float p1 = -50, q1 = 50;

static float p2 = -50, q2 = -100;

static glm::vec3 lastVect0, lastVect1, lastVect2;
static glm::vec3 lastVn0, lastVn1, lastVn2;


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


// Elias Function Implementation:
void Renderer::NaiveAlg(float p1, float p2, float q1, float q2, const glm::vec3& color)
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
		putPixel(x, 720-y, color);
	}
}

// Elias Function Implementation:
void Renderer::DrawLine(float p1, float p2, float q1, float q2, const glm::vec3& color) {
	float a = (q1 - q2) / (p1 - p2);

	if (a >= 0 && a <= 1) {
		if (p1 < p2) {
			BresenhamAlg(p1, p2, q1, q2, false, false, false, color);
		}
		else {
			BresenhamAlg(p2, p1, q2, q1, false, false, false, color);
		}
	}
	else if (a > 1) {
		if (q1 < q2) {
			BresenhamAlg(q1, q2, p1, p2, true, false, false, color);
		}
		else {
			BresenhamAlg(q2, q1, p2, p1, true, false, false, color);
		}
	}
	else if (a < 0 && a >= -1) {
		if (p1 < p2) {
			BresenhamAlg(p1, p2, q1, q2 + 2*(q1-q2), false, true, false, color);
		}
		else {
			BresenhamAlg(p2, p1, q2, q1 + 2 * (q2 - q1), false, true, false, color);
		}
	}
	else if (a < -1) {
		if (q1 < q2) {
			BresenhamAlg(q1, q2, p1, p2+2*(p1-p2), true, true, false, color);
		}
		else {
			BresenhamAlg(q1, q2 + 2 * (q1 - q2), p1, p2, true, false, true, color);
		}
	}

}

// Elias Function Implementation:
void Renderer::BresenhamAlg(float p1, float p2, float q1, float q2, bool switch_print, bool NegX, bool NegY, const glm::vec3& color) {
	float x, y, e;
	float delta_p = p2 - p1;
	float delta_q = q2 - q1;
	
	x = p1;
	y = q1;
	e = -delta_p;

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
		
		if (switch_print) {
			if (NegY) {
				putPixel((viewportWidth / 2) + y , (viewportHeight / 2) - x + 2*p1, color);
			}
			else {
				putPixel((viewportWidth / 2) + y, (viewportHeight / 2) + x,color);
			}
		}
		else {
			putPixel((viewportWidth / 2) + x, (viewportHeight / 2) + y, color);
		}
		
		x++;
		e = e + 2 * delta_q;
	}
}


double Renderer::maxValue(double v0, double v1, double v2) {
	if (v0 > v1) {
		if (v0 > v2) {
			return v0;
		}
		else if (v2 > v1) {
			return v2;
		}
		else {
			v1;
		}
	}
	else if (v2 > v1) {
		return v2;
	}
	return v1;
}

double Renderer::minValue(double v0, double v1, double v2) {
	if (v0 < v1) {
		if (v0 < v2) {
			return v0;
		}
		else if (v2 < v1) {
			return v2;
		}
		else {
			v1;
		}
	}
	else if (v2 < v1) {
		return v2;
	}
	return v1;
}

void Renderer::showMeshObject(Scene scene, std::vector<Face>::iterator face, std::vector<glm::vec3> vNormals,int k, const ImGuiIO& io) {
	int v0 = face->GetVertexIndex(0) - 1;
	int v1 = face->GetVertexIndex(1) - 1;
	int v2 = face->GetVertexIndex(2) - 1;

	// v0,v1,v2 => 1,13,4

	float x0 = scene.getModelVertices(k, v0).x;
	float y0 = scene.getModelVertices(k, v0).y;
	float z0 = scene.getModelVertices(k, v0).z;
	glm::vec4 vec0(x0, y0, z0, 1);
	// => (x0,y0,zo)

	float x1 = scene.getModelVertices(k, v1).x;
	float y1 = scene.getModelVertices(k, v1).y;
	float z1 = scene.getModelVertices(k, v1).z;
	glm::vec4 vec1(x1, y1, z1, 1);
	// => (x1,y1,z1)

	float x2 = scene.getModelVertices(k, v2).x;
	float y2 = scene.getModelVertices(k, v2).y;
	float z2 = scene.getModelVertices(k, v2).z;
	glm::vec4 vec2(x2, y2, z2, 1);
	// => (x2,y2,z2)

	std::shared_ptr<MeshModel> model = scene.GetModel(k);
	glm::vec4 vect0 = model->GetWorldTransformation()*vec0;
	glm::vec4 vect1 = model->GetWorldTransformation()*vec1;
	glm::vec4 vect2 = model->GetWorldTransformation()*vec2;
	glm::vec3 n0 = vNormals.at(0);
	glm::vec4 nt0 = model->GetWorldTransformation()*glm::vec4(n0.x,n0.y,n0.z,1);
	n0 = glm::vec3(nt0.x, nt0.y, nt0.z);
	glm::vec3 n1 = vNormals.at(1);
	glm::vec4 nt1 = model->GetWorldTransformation()*glm::vec4(n1.x, n1.y, n1.z, 1);
	n1 = glm::vec3(nt1.x, nt1.y, nt1.z);
	glm::vec3 n2 = vNormals.at(2);
	glm::vec4 nt2 = model->GetWorldTransformation()*glm::vec4(n2.x, n2.y, n2.z, 1);
	n2 = glm::vec3(nt2.x, nt2.y, nt2.z);
	

	// height = 720 & width = 1280 on my laptop constantly
	cout << "max vect_x = " << maxValue(vect0.x, vect1.x, vect2.x) << endl
	<< "min vect_x = " << minValue(vect0.x, vect1.x, vect2.x) << endl
	<< "max vect_y = " << maxValue(vect0.y, vect1.y, vect2.y) << endl
	<< "min vect_y = " << minValue(vect0.y, vect1.y, vect2.y) << endl;

	if (maxValue(vect0.x, vect1.x, vect2.x) > (viewportWidth / 2) ||
		minValue(vect0.x, vect1.x, vect2.x) < (-viewportWidth / 2) ||
		maxValue(vect0.y, vect1.y, vect2.y) > (viewportHeight / 2) ||
		minValue(vect0.y, vect1.y, vect2.y) < (-viewportHeight / 2)) {
		return;
	}
	
	lastVect0 = vect0;
	lastVn0 = n0;
	lastVect1 = vect1;
	lastVn1 = n1;
	lastVect2 = vect2;
	lastVn2 = n2;

	// it is at the valid scope:
	glm::vec3 color = glm::vec3(0, 0, 0);
	DrawLine(lastVect0.x, lastVect1.x, lastVect0.y, lastVect1.y, color);
	DrawLine(lastVect0.x, lastVect2.x, lastVect0.y, lastVect2.y, color);
	DrawLine(lastVect1.x, lastVect2.x, lastVect1.y, lastVect2.y, color);

	if (model->GetFaceNormalView()) {
		glm::vec3 basePoint((lastVect0.x + lastVect1.x + lastVect2.x) / 3, (lastVect0.y + lastVect1.y + lastVect2.y) / 3, (lastVect0.z + lastVect1.z + lastVect2.z) / 3);
		glm::vec3 estfNormal = GetEstimatedFaceNormal(basePoint, lastVect0, lastVect1, lastVect2);
		DrawLine(basePoint.x, estfNormal.x, basePoint.y, estfNormal.y, glm::vec3(200, 0, 0));
	}

	if (model->GetVertexNormalView()) {
		DrawLine(lastVect0.x, lastVn0.x, lastVect0.y, lastVn0.y, glm::vec3(0, 200, 0));
		DrawLine(lastVect1.x, lastVn1.x, lastVect1.y, lastVn1.y, glm::vec3(0, 200, 0));
		DrawLine(lastVect2.x, lastVn2.x, lastVect2.y, lastVn2.y, glm::vec3(0, 200, 0));
	}
}

glm::vec3 Renderer::GetEstimatedFaceNormal(glm::vec3 basePoint,glm::vec3 vec0, glm::vec3 vec1, glm::vec3 vec2) {
	glm::vec3 u0 = vec1 - vec0;
	glm::vec3 u1 = vec2 - vec0;
	glm::vec3 v = glm::cross(u0, u1) + basePoint;
	return glm::vec3(v.x, v.y, v.z);
}

void Renderer::Render(const Scene& scene, const ImGuiIO& io)
{
	// Get mouse position:
	p2 = io.MousePos.x - (viewportWidth/2);
	q2 = (viewportHeight/2) - io.MousePos.y;

	int modelsCount = scene.GetModelCount();

	if (scene.GetModelCount() > 0) {
		for (int k = 0; k < modelsCount; k++) {
			std::vector<Face> faces = scene.getModelfaces(k);
			std::vector<glm::vec3> vNormals = scene.getModelNormals(k);
			for (auto face = faces.begin(); face != faces.end(); ++face) {
				showMeshObject(scene, face, vNormals,k,io);
			}
		}
	}

	// Draw X and Y axis lines:
	DrawLine(-(viewportWidth/2), (viewportWidth / 2), 0, 0, glm::vec3(0, 0, 0));
	DrawLine(0, 0, (viewportHeight / 2), -(viewportHeight / 2), glm::vec3(0, 0, 0));
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