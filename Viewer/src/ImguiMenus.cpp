#pragma once
#define _USE_MATH_DEFINES

#include "Trans.h"
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
#include <iostream>
#include <list>
#include <conio.h>
#include <string>

static glm::vec4 backgroundColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);
bool showAboutUsWindow = false;
bool showTransWindow = false;
bool showDemoWindow = false;
bool showSimpleWindow = true;

//bool showAnotherWindow = false;
//bool DL = false;
//static int drawLineCounter = 0;
//static bool DrawLine = false;

const glm::vec4& GetClearColor(){
	return backgroundColor;
}

const char* getLoadedModels(Scene scene) {
	int length = scene.GetModelCount();
	if (length == 0) {
		char* empty = new char[1]{'\0'};
		return empty;
	}
	string cStr = "";
	for (size_t i = 0; i < length; i++) {
		cStr += scene.GetModel(i)->GetModelName();
		cStr += '\0';
	}
	cStr += '\0';
	int listLength = cStr.length();
	char* comboList = new char[listLength];
	if (listLength == 1) { comboList[0] = cStr.at(0); }
	for (size_t i = 0; i < listLength; i++) {
		comboList[i] = cStr.at(i);
	}
	return comboList;
}



//Elias emplementation:
const char* getCamerasNames(int length) {
	string cStr = "";
	for (size_t i = 0; i < length; i++) {

		cStr += "Camera ";
		if (i < 10) {
			cStr += '0' + i;
		}
		else {
			cStr += '0' + int(i / 10);
			cStr += '0' + int(i % 10);
		}
		cStr += '\0';
	}
	cStr += '\0';
	int listLength = cStr.length();

	char* comboList = new char[listLength];
	for (size_t i = 0; i < listLength; i++) {
		comboList[i] = cStr.at(i);
	}
	return comboList;
}


void handleKeyboardInputs(std::shared_ptr<MeshModel> model) {
	if(ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_PageUp))) {
		if(model->fScale < MAX_SCALE_FACTOR){
			model->fScale += SCALE_OBJ_FACTOR;
		}
	}
	else if(ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_PageDown))){
		if(model->fScale > MIN_SCALE_FACTOR){
			model->fScale -= SCALE_OBJ_FACTOR;
		}
	}
	else if(ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_LeftArrow))){
		if (model->fTranslatex > MIN_TRANSLATION_LENGTH) {
			model->fTranslatex -= XTRANS_FACTOR;
		}
	}
	else if(ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_RightArrow))){
		if(model->fTranslatex < MAX_TRANSLATION_LENGTH){
			model->fTranslatex += XTRANS_FACTOR;
		}
	}
	else if(ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_UpArrow))){
		if(model->fTranslatey < MAX_TRANSLATION_LENGTH){
			model->fTranslatey += YTRANS_FACTOR;
		}
	}
	else if(ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_DownArrow))){
		if(model->fTranslatey > MIN_TRANSLATION_LENGTH){
			model->fTranslatey -= YTRANS_FACTOR;
		}
	}
}


void buildTransformationsWindow(ImGuiIO& io,Scene scene) {
	string path_camera = "C:\\Users\\user\\Documents\\GitHub\\project - e - i - team\\Data\\camera.obj";

	ImGui::Begin("Scene Menu", &showTransWindow);
	ImVec4 textColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	ImGui::TextColored(textColor,"Transformations window:");
	if (ImGui::CollapsingHeader("Cameras")) {
		
		static int camera_current = 0;
		static int number_of_cameras = 1;
		const char* cameras = getCamerasNames(number_of_cameras);
		if (ImGui::Button("Add camera")) {
			number_of_cameras++;
		}

		ImGui::Combo("Active Camera", &camera_current, cameras, IM_ARRAYSIZE(cameras));
		static int Transform_type = 0;
		ImGui::RadioButton("Perspective", &Transform_type, 0);
		ImGui::RadioButton("Orthographic", &Transform_type, 1);
		
		static float ffovy = 1.0f;
		static float fnear = 1.0f;
		static float ffar = 1.0f;

		ImGui::SliderFloat("Fovy", &ffovy, 0.0f, 3.142f);
		ImGui::SliderFloat("Near", &fnear, 1.0f, 10.0f);
		ImGui::SliderFloat("Far", &ffar, 1.0f, 10.0f);
	}
	if (ImGui::CollapsingHeader("Models")) {
		const char* items = getLoadedModels(scene);
		static int modelIndex = 0;
		ImGui::Combo("Model Name", &modelIndex, items, IM_ARRAYSIZE(items));
		std::shared_ptr<MeshModel> m = scene.GetModel(modelIndex);
		
		if (m != nullptr) {
			// determine the parameters initialize if required from the user: [changing scale graph online]
			handleKeyboardInputs(m);
			// each field is belonging to each mesh model object due to this issue, 
			// we need them public and to referenced always the app is running:
			ImGui::SliderFloat("Scale Object", &(m->fScale), 1.0f, 150.0f);
			glm::mat4x4 scaling = Trans::getScale4x4(m->fScale);
			ImGui::SliderFloat("Rotate By X [-2PI,2PI]", &(m->fRotatex), -2.0f*M_PI, 2.0f*M_PI);
			glm::mat4x4 xRotateMat = Trans::getxRotate4x4(m->fRotatex);
			ImGui::SliderFloat("Translate By X", &(m->fTranslatex), -10.0f, 10.0f);
			glm::mat4x4 xTranslateMat = Trans::getTranslate4x4(0.0f, m->fTranslatex, 0.0f);
			ImGui::SliderFloat("Rotate By Y [-2PI,2PI]", &(m->fRotatey), -2.0f*M_PI, 2.0f*M_PI);
			glm::mat4x4 yRotateMat = Trans::getyRotate4x4(m->fRotatey);
			ImGui::SliderFloat("Translate By Y", &(m->fTranslatey), -10.0f, 10.0f);
			glm::mat4x4 yTranslateMat = Trans::getTranslate4x4(0.0f, m->fTranslatey, 0.0f);
			ImGui::SliderFloat("Rotate By Z [-2PI,2PI]", &(m->fRotatez), -2.0f*M_PI, 2.0f*M_PI);
			glm::mat4x4 zRotateMat = Trans::getzRotate4x4(m->fRotatez);
			ImGui::SliderFloat("Translate By Z", &(m->fTranslatez), -10.0f, 10.0f);
			glm::mat4x4 zTranslateMat = Trans::getTranslate4x4(0.0f, 0.0f, m->fTranslatez);
			ImGui::Checkbox("Show Face Normals", &(m->showFaceNormals));
			ImGui::ColorEdit3("Face Normal Color", (float*)&(m->fNcolor));
			ImGui::SliderFloat("Face Normal Length", &(m->fNlength), 1.0f, 2*NORMAL_LENGTH);
			ImGui::Checkbox("Show Vectex Normals", &(m->showVertexNormals));
			ImGui::ColorEdit3("Vertex Normal Color", (float*)&(m->vNcolor));
			ImGui::SliderFloat("Vertex Normal Length", &(m->vNlength), 1.0f, 2*NORMAL_LENGTH);

			// transformations to the space:
			glm::mat4x4 resetPosition = Trans::getTranslate4x4(0.0f, 0.0f, 0.0f);
			glm::mat4x4 nextPosition = Trans::getTranslate4x4(m->fTranslatex, m->fTranslatey, m->fTranslatez);
			m->SetWorldTransformation(nextPosition*zRotateMat*yRotateMat*xRotateMat*scaling*resetPosition);
		}
	}

	ImGui::Text("");
	if (ImGui::Button("Dismiss")) {
		showTransWindow = false;
	}
	ImGui::End();
}

void buildAboutUsWindow() {
	ImGui::Begin("About Us", &showAboutUsWindow);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
	ImVec4 textColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	ImGui::TextColored(textColor,"Student Names: Itay Guy And Elias Jadon");
	ImGui::TextColored(textColor,"Task Count: 1");
	ImGui::TextColored(textColor,"Title: Cameras VS. Viewers");
	ImGui::TextColored(textColor,"Course Semester: Winter 2018");
	ImGui::TextColored(textColor,"Lecturer: Dr. Roi Poranne");
	ImGui::TextColored(textColor,"Institute: University Of Haifa");
	if (ImGui::Button("Close")){ showAboutUsWindow = false; }
	ImGui::End();
}

void DrawImguiMenus(ImGuiIO& io, Scene& scene){
	static int grid_counter = 0;

	if (grid_counter == 0) {
		MeshModel k = Utils::LoadGridModel();
		scene.AddModel(std::make_shared<MeshModel>(k));
		grid_counter++;
	}
	

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (showDemoWindow){
		ImGui::ShowDemoWindow(&showDemoWindow);
	}

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	if(showSimpleWindow){
		
		ImGui::Begin("Task 1 - Cameras VS. Viewers");					// Create a window called "Task 1 - Cameras VS. Views" and append into it.

		//ImGui::Text("This is some useful text.");						// Display some text (you can use a format strings too)
		//ImGui::Checkbox("Demo Window", &showDemoWindow);				// Edit bools storing our window open/close state
		//ImGui::Checkbox("Another Window", &showAnotherWindow);

		ImGui::Checkbox("Transformations Window", &showTransWindow);
		ImGui::ColorEdit3("Background Color", (float*)&backgroundColor); // Edit 3 floats representing a color
		
		//if (ImGui::Button("Draw Line")) {
		//	DrawLine = true;
		//	showDemoWindow = false;
		//	showAnotherWindow = false;
		//	showSimpleWindow = false;
		//	DL = true;
		//}
		
		ImGui::Checkbox("About Us", &showAboutUsWindow);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// Show transformations window:
	if (showTransWindow) {
		// Itay's implementation:
		buildTransformationsWindow(io, scene);
	}

	// Show about us window:
	if (showAboutUsWindow) {
		// Itay's implementation:
		buildAboutUsWindow();
	}

	//if (DL){
	//	ImGui::Begin("Draw Line:", &DL);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
	//	ImGui::Text("Point1:");
	//	float p1, q1, p2, q2;
	//	if (DrawLine == true) {
	//		ImGui::Text("Mouse pos: (%g, %g)", io.MousePos.x, io.MousePos.y);
	//		if (ImGui::IsMouseClicked(0) && drawLineCounter == 0){
	//			p1 = io.MousePos.x;
	//			q1 = io.MousePos.y;
	//			printf("point1: (%f, %f)\n", io.MousePos.x, io.MousePos.y);
	//			drawLineCounter++;
	//		}
	//		else if (ImGui::IsMouseClicked(0) && drawLineCounter == 1){
	//			p2 = io.MousePos.x;
	//			q2 = io.MousePos.y;
	//			printf("point2: (%f, %f)\n", io.MousePos.x, io.MousePos.y);
	//			drawLineCounter++;
	//		}
	//		if (drawLineCounter == 1) {
	//			ImGui::Text("point1: (%g, %g)\n", io.MousePos.x, io.MousePos.y);
	//		}
	//		if (drawLineCounter == 2) {
	//			drawLineCounter = 0;
	//			DrawLine = false;
	//			showSimpleWindow = true;
	//			//DL = false;
	//		}
	//	}



	//	if (ImGui::Button("Cancel")){
	//		DL = false;
	//	}
	//	ImGui::End();
	//}

	// Show another simple window:
	//if (showAnotherWindow)
	//{
	//	ImGui::Begin("Another Window", &showAnotherWindow);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
	//	ImGui::Text("Hello from another window!");
	//	if (ImGui::Button("Dismiss"))
	//	{
	//		showAnotherWindow = false;
	//	}
	//	ImGui::End();
	//}

	// Demonstrate creating a fullscreen menu bar and populating it:
	{
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoFocusOnAppearing;
		if (ImGui::BeginMainMenuBar()){
			if (ImGui::BeginMenu("File")){
				if (ImGui::MenuItem("Load Model...", "CTRL+O")){
					nfdchar_t *outPath = NULL;
					nfdresult_t result = NFD_OpenDialog("obj;png,jpg", NULL, &outPath);
					if (result == NFD_OKAY) {
						MeshModel k = Utils::LoadMeshModel(outPath);
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
