#pragma once
#include <imgui/imgui.h>
#include "Scene.h"


void DrawImguiMenus(ImGuiIO& io, Scene& scene, int y_scroll_offset, const int frameBufferWidth, const int frameBufferHeight);
const glm::vec4& GetClearColor();
