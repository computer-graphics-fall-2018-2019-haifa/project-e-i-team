#pragma once
#include "ShaderProgram.h"
#include "Scene.h"
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include "Texture2D.h"

#define BLACK_COLOR_LINE glm::vec3(0, 0, 0)

/*
 * Renderer class.
 */
class Renderer
{
private:
	void Renderer::showAllMeshModels(Scene &scene, const ImGuiIO& io);

    ShaderProgram lightShader;
    ShaderProgram colorShader;
    Texture2D texture1;
public:
    Renderer::Renderer();
    Renderer::~Renderer();
	void Renderer::Render(Scene& scene, const ImGuiIO& io);
    void Renderer::LoadShaders();
    void Renderer::LoadTextures();
};