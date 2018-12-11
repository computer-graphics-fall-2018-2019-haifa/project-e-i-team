#pragma once
#include <imgui/imgui.h>
#include "Scene.h"

void DrawImguiMenus(ImGuiIO& io, Scene& scene, int y_scroll_offset, const int frameBufferWidth, const int frameBufferHeight);
const glm::vec4& GetClearColor();
void buildAboutUsWindow();
const char* getModelNames(Scene* scene);
const char* getCamerasNames(int length);
glm::mat4x4 handleKeyboardInputs(std::shared_ptr<MeshModel> model);
void buildProjectionsSection(std::shared_ptr<Camera> currentCam);
void buildCameraWorldTransformationsSection(glm::mat4x4& Tc, std::shared_ptr<Camera> currentCam);
void buildCameraLocalTransformationsSection(std::shared_ptr<Camera> currentCam);
void buildModelWorldTransformationsSection(glm::mat4x4& Tci, std::shared_ptr<MeshModel> currentModel);
void buildModelLocalTransformationsSection(glm::mat4x4& Tm, std::shared_ptr<MeshModel> currentModel);
void buildPropertiesSection(std::shared_ptr<MeshModel> currentModel);
void buildTransformationsWindow(ImGuiIO& io, Scene* scene, int y_scroll_offset, const int frameBufferWidth, const int frameBufferHeight);
void loadGrid(Scene& scene);
