#pragma once
#include <imgui/imgui.h>
#include "Scene.h"

void DrawImguiMenus(ImGuiIO& io, Scene& scene,int winWidth,int winHeight);
const glm::vec4& GetClearColor();
