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

void buildAboutUsWindow() {
	showAboutUsWindow = true;
	ImGui::Begin("About Us", &showAboutUsWindow);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
	ImVec4 textColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	ImGui::TextColored(textColor, "Student Names: Itay Guy And Elias Jadon");
	ImGui::TextColored(textColor, "Task Count: 1");
	ImGui::TextColored(textColor, "Title: Cameras VS. Viewers");
	ImGui::TextColored(textColor, "Course Semester: Winter 2018");
	ImGui::TextColored(textColor, "Lecturer: Dr. Roi Poranne");
	ImGui::TextColored(textColor, "Institute: University Of Haifa");
	if (ImGui::Button("Close")) { showAboutUsWindow = false; }
	ImGui::End();
}

const char* getModelNames(Scene* scene) {
	int length = 0;
	length = scene->GetModelCount();
	if (length == 0) {
		char* empty = new char[1]{ '\0' };
		return empty;
	}
	std::string cStr = "";
	for (size_t i = 0; i < length; i++) {
		std::string modelName = scene->GetModel(i)->GetModelName();
		cStr += modelName;
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
		if((fsc + SCALE_OBJ_FACTOR) != 0 && (fsc + SCALE_OBJ_FACTOR) > 0) {
			model->fScale += SCALE_OBJ_FACTOR;
			Tm = Trans::getScale4x4(model->fScale / fsc);
		}
	}
	else if(ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_PageDown))){
		float fsc = model->fScale;
		if((fsc - SCALE_OBJ_FACTOR) != 0 && (fsc - SCALE_OBJ_FACTOR) > 0) {
			model->fScale -= SCALE_OBJ_FACTOR;
			Tm = Trans::getScale4x4(model->fScale / fsc);
		}
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
	ImGui::Begin("Task 1 - Cameras VS. Models", &showTransWindow);
	
	ImVec4 textColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	ImGui::TextColored(textColor, "Scene Menu:");
	
	ImGui::ColorEdit3("Background Color", (float*)&backgroundColor); // Edit 3 floats representing a color
	
	glm::mat4x4 Tc(1), Tcm(1), Tcx(1), Tcy(1), Tcz(1);
	
	if (ImGui::CollapsingHeader("Cameras")) {
		if (ImGui::Button("Add camera")) {
			std::string path = Get_Root_Project_Dir("Data\\camera.obj");
			scene->AddCamera(std::make_shared<MeshModel>(Utils::LoadMeshModel(path)), frameBufferHeight, frameBufferWidth);
		}
		const char* cameras = getCamerasNames(scene->activeCameraIndex);
		ImGui::Combo("Active Camera", &(scene->currentActiveCamera), cameras, IM_ARRAYSIZE(cameras));
		std::shared_ptr<Camera> currentCam = scene->GetCamera(scene->currentActiveCamera);
		if (currentCam != NULL) {
			Tcm = handleKeyboardInputs(currentCam);

			ImGui::RadioButton("Orthographic", &(currentCam->transType), 0);
			ImGui::RadioButton("Perspective", &(currentCam->transType), 1);
			if (ImGui::CollapsingHeader("Projection Parameters")) {
				std::string fName = !currentCam->transType ? "Height" : "Fovy";
				ImGui::SliderFloat(fName.c_str(), &(currentCam->ffovy), FFOVY_DEF, 50.0f);
				ImGui::SliderFloat("Near", &(currentCam->fnear), FNEAR_DEF, FNEAR_DEF + 10.0f);
				ImGui::SliderFloat("Far", &(currentCam->ffar), FFAR_DEF, FFAR_DEF + 10.0f);
				//TODO at morning by ELIAS SPACER
				ImGui::SliderFloat("Left", &(currentCam->left), -BOX_BOUNDERY_RANGE, BOX_BOUNDERY_RANGE);
				ImGui::SliderFloat("Right", &(currentCam->right), -BOX_BOUNDERY_RANGE, BOX_BOUNDERY_RANGE);
				ImGui::SliderFloat("Up", &(currentCam->top), -BOX_BOUNDERY_RANGE, BOX_BOUNDERY_RANGE);
				ImGui::SliderFloat("Bottom", &(currentCam->bottom), -BOX_BOUNDERY_RANGE, BOX_BOUNDERY_RANGE);
			}
			// rotation the whole world again the stable camera:
			if (ImGui::CollapsingHeader("Camera World-Associated Transformations")) {
				float frx = currentCam->worldfRotatex, diff = 0.0f;
				ImGui::SliderFloat("Rotation By X-CW", &(currentCam->worldfRotatex), -2.0f*M_PI, 2.0f*M_PI);
				diff = currentCam->worldfRotatex - frx;
				if (diff != 0.0f) { Tc = Trans::getxRotate4x4(diff); }

				float fry = currentCam->worldfRotatey;
				ImGui::SliderFloat("Rotation By Y-CW", &(currentCam->worldfRotatey), -2.0f*M_PI, 2.0f*M_PI);
				diff = currentCam->worldfRotatey - fry;
				if (diff != 0.0f) { Tc = Trans::getyRotate4x4(diff); }

				float frz = currentCam->worldfRotatez;
				ImGui::SliderFloat("Rotation By Z-CW", &(currentCam->worldfRotatez), -2.0f*M_PI, 2.0f*M_PI);
				diff = currentCam->worldfRotatez - frz;
				if (diff != 0.0f) { Tc = Trans::getzRotate4x4(diff); }

			}
			if (ImGui::CollapsingHeader("Camera Local-Associated Transformations")) {
				float frx = currentCam->lrotatex, diff = 0.0f;
				//ImGui::SliderFloat("Rotation By X-CL", &(currentCam->lrotatex), -2.0f*M_PI, 2.0f*M_PI);
				//diff = currentCam->lrotatex - frx;
				//if (diff != 0.0f) { currentCam->pitch(diff); }

				//float fry = currentCam->lrotatey;
				//ImGui::SliderFloat("Rotation By Y-CL", &(currentCam->lrotatey), -2.0f*M_PI, 2.0f*M_PI);
				//diff = currentCam->lrotatey - fry;
				//if (diff != 0.0f) { currentCam->yaw(diff); }

				float frz = currentCam->lrotatez;
				ImGui::SliderFloat("Rotation By Z-CL", &(currentCam->lrotatez), -2.0f*M_PI, 2.0f*M_PI);
				diff = currentCam->lrotatez - frz;
				if (diff != 0.0f) { currentCam->roll(diff); }

			}
			float aspectratio = frameBufferHeight ? float(frameBufferWidth) / float(frameBufferHeight) : 0.0f;
			if (!currentCam->transType) {
				currentCam->SetOrthographicProjection(
					currentCam->ffovy, aspectratio, currentCam->fnear, currentCam->ffar,
					currentCam->left, currentCam->right, currentCam->top, currentCam->bottom, frameBufferWidth
				);
			}
			else {
				currentCam->SetPerspectiveProjection(
					currentCam->ffovy, aspectratio, currentCam->fnear, currentCam->ffar,
					currentCam->left, currentCam->right, currentCam->top, currentCam->bottom, frameBufferWidth
				);
			}

			// transform whole the world space using Tc:
			for (int i = 0; i < scene->GetModelCount(); i++) {
				std::shared_ptr<MeshModel> model = scene->GetModel(i);
				glm::vec3 mass = model->GetWorldTransformation() * glm::vec4(model->BoundMiddle.x, model->BoundMiddle.y, model->BoundMiddle.z,1.0f);
				model->UpdateworldTransform(Trans::get2InitAxis4x4(mass, Tcm * Tc));
			}
			for (int i = 0; i < scene->GetCameraCount(); i++) {
				std::shared_ptr<Camera> camera = scene->GetCamera(i);
				if (i != scene->activeCameraIndex) {
					glm::vec3 mass = camera->GetWorldTransformation() * glm::vec4(camera->BoundMiddle.x, camera->BoundMiddle.y, camera->BoundMiddle.z, 1.0f);
					camera->UpdateworldTransform(Trans::get2InitAxis4x4(mass, Tcm * Tc));
				}
			}
			if (ImGui::Button("Focus On Current Model")) {
				std::shared_ptr<Camera> camera = scene->GetCamera(scene->currentActiveCamera);
				std::shared_ptr<MeshModel> model = scene->GetModel(scene->activeModelIndex);

				glm::vec4 _at = model->GetWorldTransformation() * glm::vec4(model->BoundMiddle.x, model->BoundMiddle.y, model->BoundMiddle.z,1);
				glm::vec3 at(_at.x, _at.y, _at.z);

				glm::vec3 help_up = glm::vec3(0, 1, 0);
				glm::vec3 vec_eye_at = at - camera->origin_eye;

				help_up = glm::cross(vec_eye_at, help_up);
				glm::vec3 up = glm::cross(vec_eye_at, help_up) + camera->origin_eye;


				glm::vec4 up4 = glm::vec4(up.x, up.y, up.z, 1);
				glm::vec4 eye4 = glm::vec4(camera->origin_eye.x, camera->origin_eye.y, camera->origin_eye.z, 1);
				glm::vec4 at4 = glm::vec4(at.x, at.y, at.z, 1);
				
				camera->SetCameraLookAt(eye4, at4, up4);
			}
		}
	}
	/*
	determine the parameters initialize if required from the user: [changing scale graph online]
	each field is belonging to each mesh model object due to this issue,
	we need them public and to referenced always the app is running:
	as response to y scrolling value we control the zoom in and zoom out world models:
	*/
	if (ImGui::CollapsingHeader("Models")) {
		static int count = 0;
		const char* items = getModelNames(scene);
		ImGui::Combo("Name", &(scene->activeModelIndex), items, IM_ARRAYSIZE(items));
		std::shared_ptr<MeshModel> currentModel = scene->GetModel(scene->activeModelIndex);
		if (currentModel != nullptr) {
			glm::mat4x4 Tm(1),Tci(1);
			if (ImGui::CollapsingHeader("Model World-Associated-M Transformations")) {
				Tci = handleKeyboardInputs(currentModel);

				float fsc = currentModel->wfScale;
				ImGui::SliderFloat("Model Scale-MW", &(currentModel->wfScale), MIN_SCALE_FACTOR, MAX_SCALE_FACTOR);
				if (currentModel->wfScale >= 0 && fsc != currentModel->wfScale) { Tci = Trans::getScale4x4(currentModel->wfScale / fsc); }

				float frx = currentModel->wfRotatex, diff = 0.0f;
				ImGui::SliderFloat("Rotation By X-MW", &(currentModel->wfRotatex), -2.0f*M_PI, 2.0f*M_PI);
				diff = currentModel->wfRotatex - frx;
				if (diff != 0.0f) { Tci = Trans::getxRotate4x4(diff); }

				float fry = currentModel->wfRotatey;
				ImGui::SliderFloat("Rotation By Y-MW", &(currentModel->wfRotatey), -2.0f*M_PI, 2.0f*M_PI);
				diff = currentModel->wfRotatey - fry;
				if (diff != 0.0f) { Tci = Trans::getyRotate4x4(diff); }

				float frz = currentModel->wfRotatez;
				ImGui::SliderFloat("Rotation By Z-MW", &(currentModel->wfRotatez), -2.0f*M_PI, 2.0f*M_PI);
				diff = currentModel->wfRotatez - frz;
				if (diff != 0.0f) { Tci = Trans::getzRotate4x4(diff); }

				// transform whole the world space using Tci:
				for (int i = 0; i < scene->GetModelCount(); i++) {
					std::shared_ptr<MeshModel> model = scene->GetModel(i);
					if (i != scene->activeModelIndex) {
						glm::vec3 mass = model->GetWorldTransformation() * glm::vec4(model->BoundMiddle.x, model->BoundMiddle.y, model->BoundMiddle.z, 1.0f);
						model->UpdateworldTransform(Trans::get2InitAxis4x4(mass, Tci));
					}
				}
				for (int i = 0; i < scene->GetCameraCount(); i++) {
					std::shared_ptr<Camera> camera = scene->GetCamera(i);
					if (i != scene->activeCameraIndex) {
						glm::vec3 mass = camera->GetWorldTransformation() * glm::vec4(camera->BoundMiddle.x, camera->BoundMiddle.y, camera->BoundMiddle.z, 1.0f);
						camera->UpdateworldTransform(Trans::get2InitAxis4x4(mass, Tci));
					}
				}
			}
			if (ImGui::CollapsingHeader("Model Local-Associated Transformations")) {
				Tm = handleKeyboardInputs(currentModel);

				float fsc = currentModel->fScale;
				ImGui::SliderFloat("Model Scale-ML", &(currentModel->fScale), MIN_SCALE_FACTOR, MAX_SCALE_FACTOR);
				if (currentModel->fScale >= 0 && fsc != currentModel->fScale) { Tm = Trans::getScale4x4(currentModel->fScale / fsc); }
				float diff = 0.0f;
				float frx = currentModel->fRotatex;
				ImGui::SliderFloat("Rotate By X-ML", &(currentModel->fRotatex), -2.0f*M_PI, 2.0f*M_PI);
				diff = currentModel->fRotatex - frx;
				if (diff != 0.0f) { Tm = Trans::getxRotate4x4(diff); }

				float ftx = currentModel->fTranslatex;
				ImGui::SliderFloat("Translate By X-ML", &(currentModel->fTranslatex), MIN_TRANSLATION_LENGTH, MAX_TRANSLATION_LENGTH);
				if (ftx != currentModel->fTranslatex) { Tm = Trans::getTranslate4x4(currentModel->fTranslatex - ftx, 0, 0); }

				float fry = currentModel->fRotatey;
				ImGui::SliderFloat("Rotate By Y-ML", &(currentModel->fRotatey), -2.0f*M_PI, 2.0f*M_PI);
				diff = currentModel->fRotatey - fry;
				if (diff != 0.0f) { Tm = Trans::getyRotate4x4(diff); }

				float fty = currentModel->fTranslatey;
				ImGui::SliderFloat("Translate By Y-ML", &(currentModel->fTranslatey), MIN_TRANSLATION_LENGTH, MAX_TRANSLATION_LENGTH);
				if (fty != currentModel->fTranslatey) { Tm = Trans::getTranslate4x4(0, currentModel->fTranslatey - fty, 0); }

				float frz = currentModel->fRotatez;
				ImGui::SliderFloat("Rotate By Z-ML", &(currentModel->fRotatez), -2.0f*M_PI, 2.0f*M_PI);
				diff = currentModel->fRotatez - frz;
				if (diff != 0.0f) { Tm = Trans::getzRotate4x4(diff); }

				float ftz = currentModel->fTranslatez;
				ImGui::SliderFloat("Translate By Z-ML", &(currentModel->fTranslatez), MIN_TRANSLATION_LENGTH, MAX_TRANSLATION_LENGTH);
				if (ftz != currentModel->fTranslatez) { Tm = Trans::getTranslate4x4(0, 0, currentModel->fTranslatez - ftz); }

				glm::vec3 mass = currentModel->GetWorldTransformation() * glm::vec4(currentModel->BoundMiddle.x, currentModel->BoundMiddle.y, currentModel->BoundMiddle.z, 1.0f);
				currentModel->UpdateworldTransform(Trans::get2InitAxis4x4(mass, Tm));
			}

			if (ImGui::CollapsingHeader("Properties")) {
				ImGui::ColorEdit3("Model Color", (float*)&(currentModel->color)); // Edit 3 floats representing a color
				ImGui::Checkbox("Face Normals", &(currentModel->showFaceNormals));
				ImGui::ColorEdit3("Face Normal Color", (float*)&(currentModel->fNcolor));
				ImGui::SliderFloat("Face Normal Length", &(currentModel->fNlength), MIN_NORMAL_LENGTH, MAX_NORMAL_LENGTH);
				ImGui::Checkbox("Vectex Normals", &(currentModel->showVertexNormals));
				ImGui::ColorEdit3("Vertex Normal Color", (float*)&(currentModel->vNcolor));
				ImGui::SliderFloat("Vertex Normal Length", &(currentModel->vNlength), MIN_NORMAL_LENGTH, MAX_NORMAL_LENGTH);
				ImGui::Checkbox("Bounding Box", &(currentModel->showBoundingBox));
				ImGui::ColorEdit3("Bounding Box Color", (float*)&(currentModel->BoundingBoxColor));
			}
		}
	}
	ImGui::Text("");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("");
	if (ImGui::Button("About us")) {
		buildAboutUsWindow();
	}
	ImGui::End();
}

void loadGrid(Scene& scene) {
	MeshModel k = Utils::LoadGridModel();
	scene.AddModel(std::make_shared<MeshModel>(k));
	glm::vec4 blackColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	scene.GetModel(0)->resetModel(1.0f, false, false,blackColor, blackColor, &glm::vec3(blackColor.x, blackColor.y, blackColor.z),0.0f, 0.0f);
	scene.gridCounter++;
}

void DrawImguiMenus(ImGuiIO& io, Scene& scene,int y_scroll_offset, const int frameBufferWidth, const int frameBufferHeight){
	if (scene.gridCounter == 0) { 
		loadGrid(scene); 
		
		std::string path = Get_Root_Project_Dir("Data\\camera.obj");
		scene.AddCamera(std::make_shared<MeshModel>(Utils::LoadMeshModel(path)), frameBufferHeight,frameBufferWidth, glm::vec3(0, 500, 500));
		
	}

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	// if (showDemoWindow){ ImGui::ShowDemoWindow(&showDemoWindow); }
	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	//if(showSimpleWindow){
	//	ImGui::Begin("Task 1 - Cameras View VS. Models View");					// Create a window called "Task 1 - Cameras VS. Views" and append into it.
	//	ImGui::Checkbox("Camera Projections & Transformations", &showTransWindow);
	//	ImGui::Checkbox("About Us", &showAboutUsWindow);
	//	ImGui::End();
	//}

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
				if (ImGui::MenuItem("Browse..", "CTRL+O")){
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
			//TODO: finish this and make it no closing while it opens!!!
			//if (ImGui::BeginMenu("Help")) {
			//	if (ImGui::MenuItem("Show Demo Menu", "")) { ImGui::ShowDemoWindow(&showDemoWindow); }
			//	if (ImGui::MenuItem("About Us..", "")) { buildAboutUsWindow(); }
			//	ImGui::EndMenu();
			//}
			ImGui::EndMainMenuBar();
		}
	}
}
