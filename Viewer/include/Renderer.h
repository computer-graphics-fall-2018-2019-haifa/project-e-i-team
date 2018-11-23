#pragma once
#include "Scene.h"
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

#define NORMAL_LENGTH 10

/*
 * Renderer class.
 */
class Renderer
{
private:
	float *colorBuffer;
	float *zBuffer;
	int viewportWidth;
	int viewportHeight;
	int viewportX;
	int viewportY;

	void putPixel(int x, int y, const glm::vec3& color);
	void createBuffers(int viewportWidth, int viewportHeight);
	//void BresenhamLine(float p1, float p2, float q1, float q2);
	
	// Elias Function Implementation:
	void Renderer::NaiveAlg(float p1, float p2, float q1, float q2, const glm::vec3& color);
	// Elias Function Implementation:
	void Renderer::DrawLine(float p1, float p2, float q1, float q2, const glm::vec3& color);
	// Elias Function Implementation:
	void Renderer::BresenhamAlg(float p1, float p2, float q1, float q2, bool switch_print, bool NegX, bool NegY, const glm::vec3& color);
	// Elias Function Implementation:
	void Renderer::showGridObject(Scene scene, std::vector<Face>::iterator face, std::vector<glm::vec3> vNormals, int k, const ImGuiIO& io);
	
	
	// Itay Function Implementations: START
	void Renderer::showMeshObject(Scene scene, std::vector<Face>::iterator face, std::vector<glm::vec3> vNormal,int k, const ImGuiIO& io);
	glm::vec3 Renderer::GetEstimatedFaceNormal(glm::vec3 vbase,glm::vec3 vec0, glm::vec3 vec1, glm::vec3 vec2, float fNlength);
	double Renderer::maxValue(double v0, double v1, double v2);
	double Renderer::minValue(double v0, double v1, double v2);
	glm::vec3 Renderer::normalizeNormal(glm::vec3 v, glm::vec3 n, float length);
	glm::vec4 Renderer::normalizeNormal(glm::vec4 v, glm::vec4 n, float length);
	void Renderer::showAllMeshModels(const Scene &scene, const ImGuiIO& io);
	// Itay Function Implementations: END

	GLuint glScreenTex;
	GLuint glScreenVtc;

	void createOpenGLBuffer();
	void initOpenGLRendering();

public:
	Renderer(int viewportWidth, int viewportHeight, int viewportX = 0, int viewportY = 0);
	~Renderer();

	void Render(const Scene& scene, const ImGuiIO& io);
	void SwapBuffers();
	void ClearColorBuffer(const glm::vec3& color);
	void SetViewport(int viewportWidth, int viewportHeight, int viewportX = 0, int viewportY = 0);

	// Add more methods/functionality as needed...
};
