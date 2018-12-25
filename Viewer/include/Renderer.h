#pragma once
#include "Scene.h"
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

#define BLACK_COLOR_LINE glm::vec3(0, 0, 0)

/*
 * Renderer class.
 */
class Renderer
{
private:
	float *colorBuffer;
	float *zBuffer;
	int viewportWidth,viewportHeight;
	int viewportX,viewportY;

	void putPixel(int x, int y,float depth, const glm::vec3& color);
	void putZ(int i, int j, const float depth);
	float Renderer::Distance(glm::vec2 v1, glm::vec2 v2);
	void createBuffers(int viewportWidth, int viewportHeight);	
	void Renderer::NaiveAlg(float p1, float p2, float q1, float q2, const glm::vec3& color);
	void Renderer::DrawLine(glm::vec4& v1, glm::vec4& v2, const glm::vec3& color);
	void Renderer::BresenhamAlg(float p1, float p2, float q1, float q2, bool switch_print, bool NegX, bool NegY, const glm::vec3& color);
	void Renderer::RenderBoundingBox(Scene& scene, const ImGuiIO& io, int k, bool isCameraModel = false);
	void Renderer::showMeshObject(Scene& scene, std::vector<Face>::iterator face, std::vector<glm::vec3> vNormal,int k, const ImGuiIO& io,bool isCameraModel=false,bool isGrid = false, bool isPointLight=false);
	void Renderer::drawAmbientLight(glm::vec4& base, glm::vec3 color);
	void Renderer::drawParallelLight(glm::vec4& from, glm::vec4& to, glm::vec3 color);
	glm::vec3 Renderer::GetEstimatedFaceNormal(glm::vec3 vbase,glm::vec3 vec0, glm::vec3 vec1, glm::vec3 vec2, float fNlength);
	void Renderer::showAllMeshModels(Scene &scene, const ImGuiIO& io);
	void Renderer::printTriangle(glm::vec4& a, glm::vec4& b, glm::vec4& c, glm::vec3& color0, glm::vec3& color1, glm::vec3& color2);
	glm::vec2 Renderer::CalculateW12(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 p);
	
	GLuint glScreenTex;
	GLuint glScreenVtc;
	void createOpenGLBuffer();
	void initOpenGLRendering();
public:
	static glm::vec3 Renderer::normalizeVector(glm::vec3 v, glm::vec3 n, float length) { return length * glm::normalize(v + n) + v; }
	static glm::vec4 Renderer::normalizeVector(glm::vec4 v, glm::vec4 n, float length) { return length * glm::normalize(v + n) + v; }
	Renderer(int viewportWidth, int viewportHeight, int viewportX = 0, int viewportY = 0);
	~Renderer();
	void Render(Scene& scene, const ImGuiIO& io);
	void SwapBuffers();
	void ClearColorBuffer(const glm::vec3& color, const float depth);
	void SetViewport(int viewportWidth, int viewportHeight, int viewportX = 0, int viewportY = 0);
	/*
	K - fraction of  light  reflected from surface
	L - light intensity
	V - Viewer/Camera pinhole
	N - vertex's normal
	S - source light
	alpha - shininess coefficient
	method - from the macro you could pick: AMBIENT,DIFFUSE,SPECULAR
	*/
	glm::vec3& Renderer::estColor(float K, float L, glm::vec3& V, glm::vec3& N, glm::vec3& S, glm::vec3& colorA, glm::vec3& colorD, glm::vec3& colorS, int method, float alpha = 0.0f); // color material
	std::vector<glm::vec3>* Renderer::estTriangle(Scene& scene, std::shared_ptr<MeshModel> model, glm::vec3& n0, glm::vec3& n1, glm::vec3& n2, int method);
	glm::vec3& Renderer::interpolate_baricentric_coordinate(glm::vec4& p, glm::vec4& a, glm::vec4& b, glm::vec4& c, glm::vec3 color0, glm::vec3 color1, glm::vec3 color2);
	// uniform and simply mesh-model rendering without any ray technique:
	void Renderer::printTriangle(glm::vec4& a, glm::vec4& b, glm::vec4& c, glm::vec3& color);
	//float getTriangleArea(glm::vec2& a, glm::vec2& b, glm::vec2& c);
};
