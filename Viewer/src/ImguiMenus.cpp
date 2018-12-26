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
static float MaxDepth = 4000.0f;

bool showAboutUsWindow = false;
bool showTransWindow = true;
bool showDemoWindow = false;
bool showSimpleWindow = false;

const glm::vec4& GetClearColor(){
	return backgroundColor;
}

const float GetMaxDepth() {
	return MaxDepth;
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



const char* getLightNames(int size,string str) {
	if (size == 0) {
		char* empty = new char[1]{ '\0' };
		return empty;
	}
	std::string cStr = "";
	for (size_t i = 0; i < size; i++) {
		char num = '0' + i;
		//std::string s("Your name is ");
		//s += num;
		std::string modelName = str;
		modelName += num;
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

void buildProjectionsSection(std::shared_ptr<Camera> currentCam) {
	ImGui::RadioButton("Ordinary", &(currentCam->FrustrumType), 0);
	ImGui::RadioButton("Frustrum", &(currentCam->FrustrumType), 1);
	if (currentCam->FrustrumType) {
		ImGui::SliderFloat("Top", &(currentCam->top), -1, 1);
		ImGui::SliderFloat("Bottom", &(currentCam->bottom), -1, 1);  //-0.001745, -0.087
		ImGui::SliderFloat("Right", &(currentCam->right), -1, 1);
		ImGui::SliderFloat("Left", &(currentCam->left), -1, 1);
		ImGui::SliderFloat("Near", &(currentCam->fnear), 1, 10.0f);
		ImGui::SliderFloat("Far", &(currentCam->ffar), 1, 10.0f);
	}
	else {
		std::string fName = !currentCam->transType ? "Height" : "Fovy";
		ImGui::SliderFloat(fName.c_str(), &(currentCam->ffovy), FFOVY_DEF, 50.0f);
		ImGui::SliderFloat("Near", &(currentCam->fnear), FNEAR_DEF, FNEAR_DEF + 10.0f);
		ImGui::SliderFloat("Far", &(currentCam->ffar), FFAR_DEF, FFAR_DEF + 10.0f);
	}
	if (ImGui::Button("Snap Form To Origin")) {
		currentCam->ffovy = 1;
		currentCam->fnear = -1;
		currentCam->ffar = 1;
		currentCam->top = -1;
		currentCam->bottom = 1;
		currentCam->right = -1;
		currentCam->left = 1;
	}
}

void buildCameraWorldTransformationsSection(glm::mat4x4& Tc,std::shared_ptr<Camera> currentCam) {
	float frx = currentCam->worldfRotatex, diff = 0.0f;
	ImGui::SliderFloat("Camera World Rotate By X", &(currentCam->worldfRotatex), -2.0f*M_PI, 2.0f*M_PI);
	diff = currentCam->worldfRotatex - frx;
	if (diff != 0.0f) { Tc = Trans::getxRotate4x4(diff); }

	float fry = currentCam->worldfRotatey;
	ImGui::SliderFloat("Camera World Rotate By Y", &(currentCam->worldfRotatey), -2.0f*M_PI, 2.0f*M_PI);
	diff = currentCam->worldfRotatey - fry;
	if (diff != 0.0f) { Tc = Trans::getyRotate4x4(diff); }

	float frz = currentCam->worldfRotatez;
	ImGui::SliderFloat("Camera World Rotate By Z", &(currentCam->worldfRotatez), -2.0f*M_PI, 2.0f*M_PI);
	diff = currentCam->worldfRotatez - frz;
	if (diff != 0.0f) { Tc = Trans::getzRotate4x4(diff); }
}

void buildCameraLocalTransformationsSection(std::shared_ptr<Camera> currentCam) {
	float frx = currentCam->lrotatex, diff = 0.0f;
	//ImGui::SliderFloat("Pitch", &(currentCam->lrotatex), -2.0f*M_PI, 2.0f*M_PI);
	//diff = currentCam->lrotatex - frx;
	//if (diff != 0.0f) { currentCam->pitch(diff); }

	//float fry = currentCam->lrotatey;
	//ImGui::SliderFloat("Yaw", &(currentCam->lrotatey), -2.0f*M_PI, 2.0f*M_PI);
	//diff = currentCam->lrotatey - fry;
	//if (diff != 0.0f) { currentCam->yaw(diff); }

	float frz = currentCam->lrotatez;
	ImGui::SliderFloat("Roll", &(currentCam->lrotatez), -2.0f*M_PI, 2.0f*M_PI);
	diff = currentCam->lrotatez - frz;
	if (diff != 0.0f) { currentCam->roll(diff); }
}

void buildLocalTrans(glm::mat4x4& Tci,std::shared_ptr<MeshModel> currentModel) {
	float fsc = currentModel->wfScale;
	ImGui::SliderFloat("Local Scale", &(currentModel->wfScale), MIN_SCALE_FACTOR, MAX_SCALE_FACTOR);
	if (currentModel->wfScale >= 0 && fsc != currentModel->wfScale) { Tci = Trans::getScale4x4(currentModel->wfScale / fsc); }

	float frx = currentModel->wfRotatex, diff = 0.0f;
	ImGui::SliderFloat("Local Rotate By X", &(currentModel->wfRotatex), -2.0f*M_PI, 2.0f*M_PI);
	diff = currentModel->wfRotatex - frx;
	if (diff != 0.0f) { Tci = Trans::getxRotate4x4(diff); }

	float fry = currentModel->wfRotatey;
	ImGui::SliderFloat("Local Rotate By Y", &(currentModel->wfRotatey), -2.0f*M_PI, 2.0f*M_PI);
	diff = currentModel->wfRotatey - fry;
	if (diff != 0.0f) { Tci = Trans::getyRotate4x4(diff); }

	float frz = currentModel->wfRotatez;
	ImGui::SliderFloat("Local Rotate By Z", &(currentModel->wfRotatez), -2.0f*M_PI, 2.0f*M_PI);
	diff = currentModel->wfRotatez - frz;
	if (diff != 0.0f) { Tci = Trans::getzRotate4x4(diff); }
}

void buildWorldTrans(glm::mat4x4& Tm,std::shared_ptr<MeshModel> currentModel) {
	float fsc = currentModel->fScale;
	ImGui::SliderFloat("World Scale", &(currentModel->fScale), MIN_SCALE_FACTOR, MAX_SCALE_FACTOR);
	if (currentModel->fScale >= 0 && fsc != currentModel->fScale) { Tm = Trans::getScale4x4(currentModel->fScale / fsc); }
	float diff = 0.0f;
	float frx = currentModel->fRotatex;
	ImGui::SliderFloat("World Rotate By X", &(currentModel->fRotatex), -2.0f*M_PI, 2.0f*M_PI);
	diff = currentModel->fRotatex - frx;
	if (diff != 0.0f) { Tm = Trans::getxRotate4x4(diff); }

	float ftx = currentModel->fTranslatex;
	ImGui::SliderFloat("World Translate By X", &(currentModel->fTranslatex), MIN_TRANSLATION_LENGTH, MAX_TRANSLATION_LENGTH);
	if (ftx != currentModel->fTranslatex) { Tm = Trans::getTranslate4x4(currentModel->fTranslatex - ftx, 0, 0); }

	float fry = currentModel->fRotatey;
	ImGui::SliderFloat("World Rotate By Y", &(currentModel->fRotatey), -2.0f*M_PI, 2.0f*M_PI);
	diff = currentModel->fRotatey - fry;
	if (diff != 0.0f) { Tm = Trans::getyRotate4x4(diff); }

	float fty = currentModel->fTranslatey;
	ImGui::SliderFloat("World Translate By Y", &(currentModel->fTranslatey), MIN_TRANSLATION_LENGTH, MAX_TRANSLATION_LENGTH);
	if (fty != currentModel->fTranslatey) { Tm = Trans::getTranslate4x4(0, currentModel->fTranslatey - fty, 0); }

	float frz = currentModel->fRotatez;
	ImGui::SliderFloat("World Rotate By Z", &(currentModel->fRotatez), -2.0f*M_PI, 2.0f*M_PI);
	diff = currentModel->fRotatez - frz;
	if (diff != 0.0f) { Tm = Trans::getzRotate4x4(diff); }

	float ftz = currentModel->fTranslatez;
	ImGui::SliderFloat("World Translate By Z", &(currentModel->fTranslatez), MIN_TRANSLATION_LENGTH, MAX_TRANSLATION_LENGTH);
	if (ftz != currentModel->fTranslatez) { Tm = Trans::getTranslate4x4(0, 0, currentModel->fTranslatez - ftz); }
}

void buildLightTranslationsSection(glm::mat4x4& Tm, std::shared_ptr<MeshModel> currentModel) {
	float ftx = currentModel->fTranslatex;
	ImGui::SliderFloat("Light Translate By X", &(currentModel->fTranslatex), MIN_TRANSLATION_LENGTH, MAX_TRANSLATION_LENGTH);
	if (ftx != currentModel->fTranslatex) { Tm = Trans::getTranslate4x4(currentModel->fTranslatex - ftx, 0, 0); }

	float fty = currentModel->fTranslatey;
	ImGui::SliderFloat("Light Translate By Y", &(currentModel->fTranslatey), MIN_TRANSLATION_LENGTH, MAX_TRANSLATION_LENGTH);
	if (fty != currentModel->fTranslatey) { Tm = Trans::getTranslate4x4(0, currentModel->fTranslatey - fty, 0); }

	float ftz = currentModel->fTranslatez;
	ImGui::SliderFloat("Light Translate By Z", &(currentModel->fTranslatez), MIN_TRANSLATION_LENGTH, MAX_TRANSLATION_LENGTH);
	if (ftz != currentModel->fTranslatez) { Tm = Trans::getTranslate4x4(0, 0, currentModel->fTranslatez - ftz); }
}

void buildPropertiesSection(std::shared_ptr<MeshModel> currentModel) {
    ImGui::SliderFloat("Diffuse Reflected Ray Size", &(currentModel->Kd), 0.0f, 1.0f);
    ImGui::ColorEdit3("Diffuse Color", (float*)&(currentModel->diffuseColor));
    ImGui::SliderFloat("Specular Reflected Ray Size", &(currentModel->Ks), 0.0f, 1.0f);
    ImGui::ColorEdit3("Specular Color", (float*)&(currentModel->specularColor));
	ImGui::Checkbox("Face Normals", &(currentModel->showFaceNormals));
	ImGui::ColorEdit3("Face Normal Color", (float*)&(currentModel->fNcolor));
	ImGui::SliderFloat("Face Normal Length", &(currentModel->fNlength), MIN_NORMAL_LENGTH, MAX_NORMAL_LENGTH);
	ImGui::Checkbox("Vectex Normals", &(currentModel->showVertexNormals));
	ImGui::ColorEdit3("Vertex Normal Color", (float*)&(currentModel->vNcolor));
	ImGui::SliderFloat("Vertex Normal Length", &(currentModel->vNlength), MIN_NORMAL_LENGTH, MAX_NORMAL_LENGTH);
	ImGui::Checkbox("Bounding Box", &(currentModel->showBoundingBox));
	ImGui::ColorEdit3("Bounding Box Color", (float*)&(currentModel->BoundingBoxColor));
    ImGui::ColorEdit3("Model Color", (float*)&(currentModel->color));
}

void buildPointLightPropertiesSection(std::shared_ptr<PointLight> currentLight) {
    ImGui::SliderFloat("Diffuse Point Light Intensity", &(currentLight->Ld), 0.0f, 1.0f);
    ImGui::SliderFloat("Specular Point Light Intensity", &(currentLight->Ls), 0.0f, 1.0f);
    ImGui::ColorEdit3("Light Color", (float*)&(currentLight->color));	// Edit 3 floats representing a color
}

void buildParallelLightPropertiesSection(std::shared_ptr<ParallelLight> currentLight) {
    ImGui::SliderFloat("Diffuse Parallel Light Intensity", &(currentLight->Ld), 0.0f, 1.0f);
    ImGui::SliderFloat("Specular Parallel Light Intensity", &(currentLight->Ls), 0.0f, 1.0f);
    ImGui::ColorEdit3("Light Color", (float*)&(currentLight->color));	// Edit 3 floats representing a color
}

// it is important to use public variable for lite reading and writing values from object's fields
// the main UI building function:
void buildTransformationsWindow(ImGuiIO& io,Scene* scene,int y_scroll_offset, const int frameBufferWidth, const int frameBufferHeight) {
	ImGui::Begin("Task 1 - Cameras VS. Models", &showTransWindow);
	ImVec4 textColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	ImGui::ColorEdit3("Background Color", (float*)&backgroundColor); // Edit 3 floats representing a color
    std::shared_ptr<AmbientLight> currentLight = scene->GetAmbient();
    ImGui::ColorEdit3("Ambient Color", (float*)&(currentLight->ambientColor));
    ImGui::SliderFloat("Ambient Reflected Ray", (float*)&(currentLight->Ka),0.0f,1.0f);
    ImGui::SliderFloat("Ambient Intensity", (float*)&(currentLight->La),0.0f,1.0f);
    currentLight->ambientColor = glm::vec3(1,1,1) * currentLight->estAmbientColor(currentLight->Ka, currentLight->La);
    int shader = scene->getShadingType();
    ImGui::Combo("Shader", &shader, "Phongy\0Gouraud\0FLAT\0", 3);
    scene->setShadingType(shader);
	glm::mat4x4 Tc(1), Tcm(1), Tcx(1), Tcy(1), Tcz(1);
	static int type = MODEL_SECTION;
	const char* items[] = { "Cameras", "Models", "Point Source", "Parallel Source"};
	ImGui::Combo("Section", &type, items, IM_ARRAYSIZE(items));
	ImGui::TextColored(textColor, "");
	if (type == CAMERA_SECTION) {
		if (ImGui::Button("Add camera")) {
			std::string path = Get_Root_Project_Dir("Data\\camera.obj");
			scene->AddCamera(std::make_shared<MeshModel>(Utils::LoadMeshModel(path)), frameBufferHeight, frameBufferWidth);
		}
		const char* cameras = getCamerasNames(scene->SizeCam);
		ImGui::Combo("Active Camera", &(scene->CurrCam), cameras, IM_ARRAYSIZE(cameras));
		std::shared_ptr<Camera> currentCam = scene->GetCamera(scene->CurrCam);
		if (currentCam != NULL) {
			Tcm = handleKeyboardInputs(currentCam);
			ImGui::RadioButton("Orthographic", &(currentCam->transType), 0);
			ImGui::RadioButton("Perspective", &(currentCam->transType), 1);
			if (ImGui::CollapsingHeader("Projection Fields")) {
				buildProjectionsSection(currentCam);
			}
            if (ImGui::CollapsingHeader("Local Transformations")) {
                buildCameraLocalTransformationsSection(currentCam);
            }
			if (ImGui::CollapsingHeader("World Transformations")) { // rotation the whole world against the stable camera
				buildCameraWorldTransformationsSection(Tc, currentCam);
			}
			float aspectratio = frameBufferHeight ? float(frameBufferWidth) / float(frameBufferHeight) : 0.0f;
			if (!currentCam->transType) {
				currentCam->SetOrthographicProjection(aspectratio, frameBufferWidth);
			}
			else {
				currentCam->SetPerspectiveProjection(aspectratio, frameBufferWidth);
			}
			scene->WholeWorldTransfer(Tcm, Tc);
			if (ImGui::Button("Focus On Current Model")) {
				scene->SetFocusOnCurrentModel();
			}
		}
	}
	else if (type == MODEL_SECTION) {
		const char* items = getModelNames(scene);
		ImGui::Combo("Active Model", &(scene->activeModelIndex), items, IM_ARRAYSIZE(items));
		std::shared_ptr<MeshModel> currentModel = scene->GetModel(scene->activeModelIndex);
		if (currentModel != nullptr) {
			glm::mat4x4 T(1), Tci(1), Tk(1);
			Tk = handleKeyboardInputs(currentModel);
			currentModel->UpdateworldTransform(Tk);
			if (ImGui::CollapsingHeader("Local Transformations")) {
				buildLocalTrans(Tci, currentModel);
				glm::vec3 location = currentModel->GetModelLocationAfterTrans();
				glm::mat4x4 toZero = Trans::getTranslate4x4(-location.x, -location.y, -location.z);
				glm::mat4x4 BacktoOrigin = Trans::getTranslate4x4(location.x, location.y, location.z);
				currentModel->UpdateworldTransform(BacktoOrigin * Tci * toZero);
			}
			if (ImGui::CollapsingHeader("World Transformations")) {
				buildWorldTrans(T, currentModel);
				currentModel->UpdateworldTransform(T);
			}
			if (ImGui::CollapsingHeader("Model Properties")) {
				buildPropertiesSection(currentModel);
			}
		} 
	} else if (type == POINT_LIGHT_SECTION) {
		if (ImGui::Button("Add Point Light")) {
			std::string path = Get_Root_Project_Dir("Data\\obj_examples\\light_source.obj");
			scene->AddPointLight(std::make_shared<MeshModel>(Utils::LoadMeshModel(path)), frameBufferHeight, frameBufferWidth);
		}
		const char* items = getLightNames(scene->SizePoint, "Point Light ");
		ImGui::Combo("Light Name", &(scene->CurrPoint), items, IM_ARRAYSIZE(items));
		std::shared_ptr<PointLight> currentLight = scene->GetPointLight(scene->CurrPoint);
		if (currentLight != nullptr) {
            currentLight->lightType = POINT_LIGHT;
			glm::mat4x4 T(1), Tci(1), Tk(1);
			Tk = handleKeyboardInputs(currentLight);
			currentLight->UpdateworldTransform(Tk);
			if (ImGui::CollapsingHeader("Local Transformations")) {
				buildLocalTrans(Tci, currentLight);
				glm::vec3 location = currentLight->GetLocationAfterTrans();
				glm::mat4x4 toZero = Trans::getTranslate4x4(-location.x, -location.y, -location.z);
				glm::mat4x4 BacktoOrigin = Trans::getTranslate4x4(location.x, location.y, location.z);
				currentLight->UpdateworldTransform(BacktoOrigin * Tci * toZero);
			}
			if (ImGui::CollapsingHeader("World Transformations")) {
				buildWorldTrans(T, currentLight);
				currentLight->UpdateworldTransform(T);
			}
			if (ImGui::CollapsingHeader("Properties")) {
                buildPointLightPropertiesSection(currentLight);
			}

		}
	} else if (type == PARALLEL_LIGHT_SECTION) {
		if (ImGui::Button("Add Parallel Light")) {
			scene->AddParallelLight();
		}
		const char* items = getLightNames(scene->SizeParallel, "Parallel Light ");
		ImGui::Combo("Light Name", &(scene->CurrParallel), items, IM_ARRAYSIZE(items));
		std::shared_ptr<ParallelLight> currentLight = scene->GetParallelLight(scene->CurrParallel);
		if (currentLight != nullptr) {
            currentLight->lightType = PARALLEL_LIGHT;
			glm::mat4x4 T(1), Tci(1), Tk(1);
			Tk = handleKeyboardInputs(currentLight);
			currentLight->UpdateworldTransform(Tk);
			if (ImGui::CollapsingHeader("Local Transformations")) {
				buildLocalTrans(Tci, currentLight);
				glm::vec3 location = currentLight->GetLocationAfterTrans();
				glm::mat4x4 toZero = Trans::getTranslate4x4(-location.x, -location.y, -location.z);
				glm::mat4x4 BacktoOrigin = Trans::getTranslate4x4(location.x, location.y, location.z);
				currentLight->UpdateworldTransform(BacktoOrigin * Tci * toZero);
			}
			if (ImGui::CollapsingHeader("World Transformations")) {
				buildWorldTrans(T, currentLight);
				currentLight->UpdateworldTransform(T);
			}
			if (ImGui::CollapsingHeader("Properties")) {
                buildParallelLightPropertiesSection(currentLight);
			}
		}
	} else {
        cout << "Error under section picking!" << endl;
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
	scene.GetModel(0)->resetModel(1.0f, false, false,false, blackColor, blackColor, &glm::vec3(blackColor.x, blackColor.y, blackColor.z), 0.0f, 0.0f);
	scene.gridCounter++;
}

void DrawImguiMenus(ImGuiIO& io, Scene& scene,int y_scroll_offset, const int frameBufferWidth, const int frameBufferHeight){
	if (scene.gridCounter == 0) { 
		loadGrid(scene); 
		
		std::string path = Get_Root_Project_Dir("Data\\camera.obj");
		scene.AddCamera(std::make_shared<MeshModel>(Utils::LoadMeshModel(path)), frameBufferHeight,frameBufferWidth, glm::vec3(0, 500, 500));
		
	}

	if (showTransWindow) { 
		buildTransformationsWindow(io, &scene, y_scroll_offset ,frameBufferWidth, frameBufferHeight); 
	}

	if (showAboutUsWindow) { 
		buildAboutUsWindow();
	}

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
						cout << "(" << k.BoundMiddle.x << " , " << k.BoundMiddle.y << " , " << k.BoundMiddle.z << ")" << endl;
						
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
			//if (ImGui::MenuItem("Show Demo Menu", "")) { ImGui::ShowDemoWindow(&showDemoWindow); }
			//	if (ImGui::MenuItem("About Us..", "")) { buildAboutUsWindow(); }
			//	ImGui::EndMenu();
			//}
			ImGui::EndMainMenuBar();
		}
	}
}
