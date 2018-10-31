#pragma once
#define _USE_MATH_DEFINES

#include "ImguiMenus.h"
#include "MeshModel.h"
#include "Utils.h"
#include <cmath>
#include <memory>
#include <stdio.h>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <nfd.h>
#include <random>

float slideValue = 0.0;
bool showDemoWindow = false;
bool showAnotherWindow = false;
bool showSimpleWindow = true;
bool DRAW_LINE_NEEDED = false;
static int drawLineCounter = 0;
static bool DrawLine = false;

glm::vec4 clearColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);

const glm::vec4& GetClearColor()
{
	return clearColor;
}

void DrawImguiMenus(ImGuiIO& io, Scene& scene)
{
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (showDemoWindow)
	{
		ImGui::ShowDemoWindow(&showDemoWindow);
	}

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	if(showSimpleWindow)
	{
		static float f = 0.0f;
		static int counter = 0;
		ImGui::Begin("Wireframe Viewer");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("The First And Largest Assignment");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("GUI Settings Window", &showDemoWindow);      // Edit bools storing our window open/close state

		// ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		// ImGui::ColorEdit3("clear color", (float*)&clearColor); // Edit 3 floats representing a color

		// if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			// counter++;
		// ImGui::SameLine();
		// ImGui::Text("counter = %d", counter);
		
		
		if (ImGui::Button("Draw Line")) {
			DrawLine = true;
			showDemoWindow = false;
			// showAnotherWindow = false;
			showSimpleWindow = false;
			DRAW_LINE_NEEDED = true;
		}
		

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		if (ImGui::Button("Termination")) {
			exit(0);
		}
		ImGui::End();
	}

	if(DRAW_LINE_NEEDED)
	{
		ImGui::Begin("Draw Simple Line:", &DRAW_LINE_NEEDED);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Point1:");
		float p1, q1, p2, q2;
		if (DrawLine == true) {

			ImGui::Text("Mouse pos: (%g, %g)", io.MousePos.x, io.MousePos.y);
			if (ImGui::IsMouseClicked(0) && drawLineCounter == 0)
			{
				p1 = io.MousePos.x;
				q1 = io.MousePos.y;
				printf("point1: (%f, %f)\n", io.MousePos.x, io.MousePos.y);
				drawLineCounter++;
			}
			else if (ImGui::IsMouseClicked(0) && drawLineCounter == 1)
			{
				p2 = io.MousePos.x;
				q2 = io.MousePos.y;
				printf("point2: (%f, %f)\n", io.MousePos.x, io.MousePos.y);
				drawLineCounter++;
			}
			if (drawLineCounter == 1) {
				ImGui::Text("point1: (%g, %g)\n", io.MousePos.x, io.MousePos.y);
			}
			if (drawLineCounter == 2) {
				drawLineCounter = 0;
				DrawLine = false;
				showSimpleWindow = true;
				// DL = false;
			}
		}



		if (ImGui::Button("Cancel"))
		{
			DRAW_LINE_NEEDED = false;
		}
		ImGui::End();
	}





	// 3. Show another simple window.
	/*
	if (showAnotherWindow)
	{
		ImGui::Begin("Another Window", &showAnotherWindow);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
		{
			showAnotherWindow = false;
		}
		ImGui::End();
	}
	*/

	// 4. Demonstrate creating a fullscreen menu bar and populating it.
	{
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoFocusOnAppearing;
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Load Model...", "CTRL+O"))
				{
					nfdchar_t *outPath = NULL;
					nfdresult_t result = NFD_OpenDialog("obj;png,jpg", NULL, &outPath);
					if (result == NFD_OKAY) {
						scene.AddModel(std::make_shared<MeshModel>(Utils::LoadMeshModel(outPath)));
						free(outPath);
					}
					else if (result == NFD_CANCEL) {
					}
					else {
					}

				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}
}