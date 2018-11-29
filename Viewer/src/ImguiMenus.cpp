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
#include "Renderer.h"

static glm::vec4 backgroundColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);
bool showAboutUsWindow = false;
bool showTransWindow = true;
bool showDemoWindow = false;
bool showSimpleWindow = false;

const glm::vec4& GetClearColor(){
	return backgroundColor;
}

const char* getModelNames(Scene* scene,int modelType=MESH_MODEL_TYPE) {
	int length = 0;
	if (modelType == MESH_MODEL_TYPE) {
		length = scene->GetModelCount();
	} else { /* CAMERA_MODEL_TYPE */
		length = scene->GetCameraCount();
	}
	if (length == 0) {
		char* empty = new char[1]{ '\0' };
		return empty;
	}
	std::string cStr = "";
	for (size_t i = 0; i < length; i++) {
		if (modelType == MESH_MODEL_TYPE) {
			std::string modelName = scene->GetModel(i)->GetModelName();
			cStr += modelName;
			cStr += '\0';
		} else { /* CAMERA_MODEL_TYPE */
			std::string cam(scene->GetCamera(i)->GetModelName());
			cStr += cam.substr(0, cam.length() - sizeof(".obj") + 1);
			cStr += " ";
			cStr += i;
			cStr += '\0';
		}
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

glm::mat4x4 handleKeyboardInputs(std::shared_ptr<MeshModel> model) {
	glm::mat4x4 Tm(1);

	if(ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_PageUp))) {
		float fsc = model->fScale;
		model->fScale += SCALE_OBJ_FACTOR;
		Tm = Trans::getScale4x4(model->fScale / fsc);
		
	}
	else if(ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_PageDown))){
		float fsc = model->fScale;
		model->fScale -= SCALE_OBJ_FACTOR;
		Tm = Trans::getScale4x4(model->fScale / fsc);
	}
	else if(ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_LeftArrow))){
		float ftx = model->fTranslatex;
		model->fTranslatex -= XTRANS_FACTOR;
		Tm = Trans::getTranslate4x4(model->fTranslatex - ftx, 0, 0);
	}
	else if(ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_RightArrow))){
		float ftx = model->fTranslatex;
		model->fTranslatex += XTRANS_FACTOR;
		Tm = Trans::getTranslate4x4(model->fTranslatex - ftx, 0, 0);
	}
	else if(ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_UpArrow))){
		float fty = model->fTranslatey;
		model->fTranslatey += YTRANS_FACTOR;
		Tm = Trans::getTranslate4x4(0, model->fTranslatey - fty, 0);
	}
	else if(ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_DownArrow))){
		float fty = model->fTranslatey;
		model->fTranslatey -= YTRANS_FACTOR;
		Tm = Trans::getTranslate4x4(0, model->fTranslatey - fty, 0);
	}
	return Tm;
}

// it is important to use public variable for lite reading and writing values from object's fields
void buildTransformationsWindow(ImGuiIO& io,Scene* scene,int y_scroll_offset, const int frameBufferWidth, const int frameBufferHeight) {
	ImGui::Begin("Scene Menu", &showTransWindow);
	ImVec4 textColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	ImGui::ColorEdit3("Background Color", (float*)&backgroundColor); // Edit 3 floats representing a color
	glm::mat4x4 inv = glm::mat4x4(1);
	if (ImGui::CollapsingHeader("Cameras")) {
		if (ImGui::Button("Add camera")) {
			std::string path = Get_Root_Project_Dir("Data\\camera.obj");
			scene->AddCamera(std::make_shared<MeshModel>(Utils::LoadMeshModel(path)), frameBufferHeight);
		}
		const char* cameras = getCamerasNames(scene->activeCameraIndex);
		ImGui::Combo("Active Camera", &(scene->currentActiveCamera), cameras, IM_ARRAYSIZE(cameras));
		std::shared_ptr<Camera> currentCam = scene->GetCamera(scene->currentActiveCamera);
		if (currentCam != NULL) {	
			
			//glm::mat4x4 Tc(1);

			ImGui::RadioButton("Orthographic", &(currentCam->transType), 0);
			ImGui::RadioButton("Perspective", &(currentCam->transType), 1);
			std::string fName = !currentCam->transType ? "Height" : "Fovy";
			ImGui::SliderFloat(fName.c_str(), &(currentCam->ffovy), MIN_FFOVY, MAX_FFOVY);
			ImGui::SliderFloat("Near", &(currentCam->fnear), MIN_FNEAR, MAX_FNEAR);
			ImGui::SliderFloat("Far", &(currentCam->ffar), MIN_FFAR, MAX_FFAR);
			ImGui::ColorEdit3("Camera Color", (float*)&(currentCam->color));


			/*
			ImGui::TextColored(textColor, "Camera Transformations:");
			float frx = currentCam->fRotatex, fry = currentCam->fRotatey, frz = currentCam->fRotatez;
			ImGui::SliderFloat("Camera Rotation By x [-2PI,+2PI]", &(currentCam->fRotatex), -2.2f*M_PI, 2.2f*M_PI);
			if (frx != currentCam->fRotatex) { Tc = Trans::getxRotate4x4(currentCam->fRotatex - frx); }
			ImGui::SliderFloat("Camera Rotation By y [-2PI,+2PI]", &(currentCam->fRotatey), -2.2f*M_PI, 2.2f*M_PI);
			if (fry != currentCam->fRotatey) { Tc = Trans::getyRotate4x4(currentCam->fRotatey - fry); }
			ImGui::SliderFloat("Camera Rotation By z [-2PI,+2PI]", &(currentCam->fRotatez), -2.2f*M_PI, 2.2f*M_PI);
			if (frz != currentCam->fRotatez) { Tc = Trans::getzRotate4x4(currentCam->fRotatez - frz); }
			ImGui::ColorEdit3("Camera Color", (float*)&(currentCam->color)); // Edit 3 floats representing a color
			*/
			
			float aspectratio = frameBufferHeight ? float(frameBufferWidth) / float(frameBufferHeight) : 0.0f;
			if (!currentCam->transType) { 
				currentCam->SetOrthographicProjection(currentCam->ffovy, aspectratio, currentCam->fnear, currentCam->ffar); 
				inv = glm::inverse(currentCam->GetProjection()); 
			} else { 
				currentCam->SetPerspectiveProjection(currentCam->ffovy, aspectratio, currentCam->fnear, currentCam->ffar);
				inv = glm::inverse(currentCam->Getview());
			}



			if (ImGui::Button("Focus on current model")) {
				std::shared_ptr<MeshModel> currentModel = scene->GetModel(scene->activeModelIndex);
				glm::vec3 at = currentModel->BoundMiddle;
				
				glm::vec3 rand = glm::vec3(3, 2, 1);
				glm::vec3 vec_eye_at = at - currentCam->origin_eye;
				glm::vec3 vector_eye_rand = rand - currentCam->origin_eye;
				glm::vec3 up = glm::cross(vec_eye_at, vector_eye_rand) + currentCam->origin_eye;
				
				glm::vec4 up4 = glm::vec4(up.x, up.y, up.z, 1);
				glm::vec4 at4 = glm::vec4(at.x, at.y, at.z, 1);
				glm::vec4 eye4 = glm::vec4((currentCam->origin_eye).x, (currentCam->origin_eye).y, (currentCam->origin_eye).z, 1);
				
				currentCam->SetCameraLookAt(eye4, at4, up4);
			}
		}
	}
	// transformations to the space:
	/*for (int i = 0; i < scene->GetModelCount(); i++) {
		std::shared_ptr<MeshModel> model = scene->GetModel(i);
		if(i == scene->activeModelIndex) { model->SetWorldTransformation(Tm* model->GetWorldTransformation()); }
		else { model->SetWorldTransformation(inv); }
	}
	for (int i = 0; i < scene->GetCameraCount(); i++) {
		if (i != scene->activeCameraIndex) {
			std::shared_ptr<Camera> camera = scene->GetCamera(i);
			camera->SetWorldTransformation(inv);
		}
	}*/


	
	
	if (ImGui::CollapsingHeader("Models")) {
		const char* items = getModelNames(scene);
		ImGui::Combo("Model Name", &(scene->activeModelIndex), items, IM_ARRAYSIZE(items));
		std::shared_ptr<MeshModel> currentModel = scene->GetModel(scene->activeModelIndex);
		ImGui::ColorEdit3("Model Color", (float*)&(currentModel->color)); // Edit 3 floats representing a color
		if (currentModel != nullptr) {
			glm::mat4x4 Tm(1);
			// determine the parameters initialize if required from the user: [changing scale graph online]
			// each field is belonging to each mesh model object due to this issue, 
			// we need them public and to referenced always the app is running:
			// as response to y scrolling value we control the zoom in and zoom out world models:

			ImGui::TextColored(textColor, "Model Transformations:");

			Tm = handleKeyboardInputs(currentModel);
			float fsc = currentModel->fScale;
			ImGui::SliderFloat("Model Scale", &(currentModel->fScale), MIN_SCALE_FACTOR, MAX_SCALE_FACTOR);
			if (fsc != currentModel->fScale) {Tm = Trans::getScale4x4(currentModel->fScale / fsc);	}
			float frx = currentModel->fRotatex;
			ImGui::SliderFloat("Rotate By X [-2PI,+2PI]", &(currentModel->fRotatex), -2.2f*M_PI, 2.2f*M_PI);
			if (frx != currentModel->fRotatex) { Tm = Trans::getxRotate4x4(currentModel->fRotatex - frx); }
			float ftx = currentModel->fTranslatex;
			ImGui::SliderFloat("Translate By X", &(currentModel->fTranslatex), -1000.0f, 1000.0f);
			if (ftx != currentModel->fTranslatex) { Tm = Trans::getTranslate4x4(currentModel->fTranslatex - ftx, 0, 0); }
			float fry = currentModel->fRotatey;
			ImGui::SliderFloat("Rotate By Y [-2PI,+2PI]", &(currentModel->fRotatey), -2.2f*M_PI, 2.2f*M_PI);
			if (fry != currentModel->fRotatey) { Tm = Trans::getyRotate4x4(currentModel->fRotatey - fry); }
			float fty = currentModel->fTranslatey;
			ImGui::SliderFloat("Translate By Y", &(currentModel->fTranslatey), -1000.0f, 1000.0f);
			if (fty != currentModel->fTranslatey) { Tm = Trans::getTranslate4x4(0, currentModel->fTranslatey - fty, 0); }
			float frz = currentModel->fRotatez;
			ImGui::SliderFloat("Rotate By Z [-2PI,+2PI]", &(currentModel->fRotatez), -2.2f*M_PI, 2.2f*M_PI);
			if (frz != currentModel->fRotatez) { Tm = Trans::getzRotate4x4(currentModel->fRotatez - frz); }
			float ftz = currentModel->fTranslatez;
			ImGui::SliderFloat("Translate By Z", &(currentModel->fTranslatez), -1000.0f, 1000.0f);
			if (ftz != currentModel->fTranslatez){ Tm = Trans::getTranslate4x4(0, 0, currentModel->fTranslatez - ftz); }
	
			ImGui::Checkbox("Show Face Normals", &(currentModel->showFaceNormals));
			ImGui::ColorEdit3("Face Normal Color", (float*)&(currentModel->fNcolor));
			ImGui::SliderFloat("Face Normal Length", &(currentModel->fNlength), 1.0f, 4*NORMAL_LENGTH);
			ImGui::Checkbox("Show Vectex Normals", &(currentModel->showVertexNormals));
			ImGui::ColorEdit3("Vertex Normal Color", (float*)&(currentModel->vNcolor));
			ImGui::SliderFloat("Vertex Normal Length", &(currentModel->vNlength), 1.0f, 4*NORMAL_LENGTH);
			ImGui::Checkbox("Show Bounding Box", &(currentModel->showBoundingBox));
			ImGui::ColorEdit3("Bounding Box Color", (float*)&(currentModel->BoundingBoxColor));

			currentModel->UpdateworldTransform(Tm);
		}
	}

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
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

void loadGrid(Scene& scene) {
	MeshModel k = Utils::LoadGridModel();
	scene.AddModel(std::make_shared<MeshModel>(k));
	glm::vec4 blackColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	scene.GetModel(0)->resetModel(1.0f, blackColor, blackColor, &glm::vec3(blackColor.x, blackColor.y, blackColor.z),0.0f, 0.0f);
	scene.gridCounter++;
}

void DrawImguiMenus(ImGuiIO& io, Scene& scene,int y_scroll_offset, const int frameBufferWidth, const int frameBufferHeight){
	if (scene.gridCounter == 0) { loadGrid(scene); }
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (showDemoWindow){ ImGui::ShowDemoWindow(&showDemoWindow); }
	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	if(showSimpleWindow){
		ImGui::Begin("Task 1 - Cameras View VS. Models View");					// Create a window called "Task 1 - Cameras VS. Views" and append into it.
		ImGui::Checkbox("Camera Projections & Transformations", &showTransWindow);
		
		ImGui::Checkbox("About Us", &showAboutUsWindow);
		ImGui::End();
	}

	// Show transformations window:
	// Itay's implementation:
	if (showTransWindow) { buildTransformationsWindow(io, &scene, y_scroll_offset ,frameBufferWidth, frameBufferHeight); }
	// Show about us window:
	if (showAboutUsWindow) { buildAboutUsWindow(); }

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
