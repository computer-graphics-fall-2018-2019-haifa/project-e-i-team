#define _USE_MATH_DEFINES
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>

#include <nfd.h>
#include <glm/glm.hpp>
#include <string>
#include "MeshModel.h"
#include <direct.h>
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <sstream>
#include <stdlib.h>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Renderer.h"
#include "Scene.h"
#include "Camera.h"
#include "ImguiMenus.h"
#include "AmbientLight.h"
#include "PointLight.h"
#include "Utils.h"

#define PROJECT_NAME "project-e-i-team"

#define Get_Root_Project_Dir(X) std::string(_getcwd(NULL, 0)).substr(0, std::string(_getcwd(NULL, 0)).find(PROJECT_NAME) + sizeof(PROJECT_NAME)) + X;


#define CAMERA_SECTION          0
#define MODEL_SECTION           1
#define POINT_LIGHT_SECTION     2
#define PARALLEL_LIGHT_SECTION  3
#define AMBIENT_LIGHT_SECTION   4
#define BLUR_SECTION            5


double zoomFactor = 1;
int windowWidth = 1280;
int windowHeight = 720;
char* windowTitle = "OpenGL-Task3";
glm::vec4 clearColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);
bool zoomChanged = false;
std::shared_ptr<Scene> scene;

ImGuiIO* imgui;
GLFWwindow* window;
GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name);
ImGuiIO& SetupImgui(GLFWwindow* window);
bool Setup(int windowWidth, int windowHeight, const char* windowName);
void Cleanup();

static void GlfwErrorCallback(int error, const char* description);

void glfw_OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset);
void glfw_OnFramebufferSize(GLFWwindow* window, int width, int height);

float GetAspectRatio();
void DrawImguiMenus();
void HandleImguiInput();
void loadScene();

int main(int argc, char **argv)
{

	if (!Setup(windowWidth, windowHeight, windowTitle))
	{
		std::cerr << "Setup failed" << std::endl;
		return -1;
	}

	scene = std::make_shared<Scene>();

    // loading customed scene:
    loadScene();
	
    Renderer renderer;
	renderer.LoadShaders();
	renderer.LoadTextures();

	while (!glfwWindowShouldClose(window))
	{
		// Poll and process events
		glfwPollEvents();

		// Imgui stuff
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		DrawImguiMenus();
		ImGui::Render();
		HandleImguiInput();

		// Clear the screen and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render scene
		renderer.Render(scene);

		// Imgui stuff
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Swap front and back buffers
		glfwSwapBuffers(window);
	}

	glfwTerminate();
    return 0;
}

void loadScene() {
    //load 1 camera:
    glm::vec3 eye = glm::vec3(0, 0, 10);
    glm::vec3 at = glm::vec3(0, 0, 0);
    glm::vec3 up = glm::vec3(0, 1, 0);
    Camera camera = Camera(eye, at, up, GetAspectRatio());
    camera.SetFovy(0.9);
    scene->AddCamera(camera);

    //load 1 demo object:
    std::string demo = Get_Root_Project_Dir("Data\\obj_examples\\demo.obj");
    scene->AddModel(Utils::LoadMeshModel(demo));
    std::shared_ptr<MeshModel> model = scene->GetModel(0);
    model->ScaleModel(0.7f);
    model->TranslateModel(glm::vec3(2, 0, 0));
    model->SetColor(glm::vec3(0.6, 0.5, 0.7));
    model->_Ka = 0.7;
    model->_Kd = 0.7;
    model->_Ks = 0.7;
    model->_exp = 0.7;
    model->_diffuseColor = glm::vec3(1, 0.4, 0.4);
    model->_specuralColor = glm::vec3(1, 0.4, 0.4);
    model->_isTexNeeded = false;
    model->_isWireMode = false;
    model->_isToonShading = false;
    model->_isNoisyMode = false;

    //load 1 cow using phong illumination:
    std::string cow = Get_Root_Project_Dir("Data\\obj_examples\\cow.obj");
    scene->AddModel(Utils::LoadMeshModel(cow));
    model = scene->GetModel(1);
    model->TranslateModel(glm::vec3(-2, 0, 0));
    model->SetColor(glm::vec3(0.4, 0.4, 0.4));
    model->_Ka = 0.7;
    model->_Kd = 0.7;
    model->_Ks = 0.7;
    model->_exp = 0.7;
    model->_diffuseColor = glm::vec3(0.2, 0.4, 1);
    model->_specuralColor = glm::vec3(0.2, 0.4, 1);
    model->_isTexNeeded = false;
    model->_isWireMode = false;
    model->_isToonShading = false;
    model->_isNoisyMode = false;

    //load 1 cow using phong illumination + toon shading:
    std::string cowToon = Get_Root_Project_Dir("Data\\obj_examples\\cow.obj");
    scene->AddModel(Utils::LoadMeshModel(cowToon));
    model = scene->GetModel(2);
    model->TranslateModel(glm::vec3(-2, -3, 0));
    model->SetColor(glm::vec3(0.4, 0.4, 0.4));
    model->_Ka = 0.7;
    model->_Kd = 0.7;
    model->_Ks = 0.7;
    model->_exp = 0.7;
    model->_diffuseColor = glm::vec3(0.2, 0.4, 1);
    model->_specuralColor = glm::vec3(0.2, 0.4, 1);
    model->_toonRange = 45;
    model->_isTexNeeded = false;
    model->_isWireMode = false;
    model->_isToonShading = true;
    model->_isNoisyMode = false;

    //load crate object to display the texture:
    std::string crate = Get_Root_Project_Dir("Data\\obj_examples\\crate.obj");
    scene->AddModel(Utils::LoadMeshModel(crate));
    model = scene->GetModel(3);
    model->TranslateModel(glm::vec3(2, -3, 0));
    model->SetColor(glm::vec3(0.4, 0.4, 0.4));
    model->_Ka = 0.7;
    model->_Kd = 0.7;
    model->_Ks = 0.7;
    model->_exp = 0.7;
    model->_diffuseColor = glm::vec3(0.2, 0.4, 1);
    model->_specuralColor = glm::vec3(0.2, 0.4, 1);
    model->_isTexNeeded = true;
    model->_isWireMode = false;
    model->_isToonShading = false;
    model->_isNoisyMode = false;

    //Choose color randomly
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0, 1);

    //load 1 point light:
    scene->AddPointLight(std::make_shared<PointLight>(glm::vec3(0, 0, 0), glm::vec3(dist(mt), dist(mt), dist(mt))));
    std::shared_ptr<PointLight> pointLight = scene->GetPointLight(0);
    pointLight->ScaleModel(0.1f);
    pointLight->TranslateModel(glm::vec3(0, -2, 2));
    pointLight->SetColor(glm::vec3(0.9, 1.0, 0.0));
}

static void GlfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

bool Setup(int windowWidth, int windowHeight, const char* windowName)
{
	GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, windowName);
	if (!window)
	{
		std::cerr << "Window setup failed" << std::endl;
		return false;
	}

	imgui = &SetupImgui(window);

	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	glEnable(GL_DEPTH_TEST);

	return true;
}

GLFWwindow* SetupGlfwWindow(int windowWidth, int windowHeight, const char* windowName)
{
	// Intialize GLFW
	if (!glfwInit())
	{
		// An error occured
		std::cerr << "GLFW initialization failed" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// forward compatible with newer versions of OpenGL as they become available but not backward compatible (it will not run on devices that do not support OpenGL 3.3
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);	

	// Create an OpenGL 3.3 core, forward compatible context window
	window = glfwCreateWindow(windowWidth, windowHeight, windowName, NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	// Make the window's context the current one
	glfwMakeContextCurrent(window);

	// Setup window events callbacks
	glfwSetFramebufferSizeCallback(window, glfw_OnFramebufferSize);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		// An error occured
		std::cerr << "GLAD initialization failed" << std::endl;
		return false;
	}

	return window;
}

ImGuiIO& SetupImgui(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	// Setup style
	ImGui::StyleColorsDark();

	glfwSetScrollCallback(window, glfw_OnMouseScroll);

	return io;
}

void HandleImguiInput()
{
	if (!imgui->WantCaptureKeyboard)
	{
		if (imgui->KeysDown[49]) // 1
		{
			scene->GetActiveModel()->RotateXModel(M_PI / 20);
		}

		if (imgui->KeysDown[50]) // 2
		{
			scene->GetActiveModel()->RotateXModel(-M_PI / 20);
		}

		if (imgui->KeysDown[51]) // 3
		{
			scene->GetActiveModel()->RotateYModel(M_PI / 20);
		}

		if (imgui->KeysDown[52]) // 4
		{
			scene->GetActiveModel()->RotateYModel(-M_PI / 20);
		}

		if (imgui->KeysDown[53]) // 5
		{
			scene->GetActiveModel()->RotateZModel(M_PI / 20);
		}

		if (imgui->KeysDown[54]) // 6
		{
			scene->GetActiveModel()->RotateZModel(-M_PI / 20);
		}

		if (imgui->KeysDown[45]) // -
		{
			scene->GetActiveModel()->ScaleModel(1 / 1.1);
		}

		if (imgui->KeysDown[61]) // +
		{
			scene->GetActiveModel()->ScaleModel(1.1);
		}

		if (imgui->KeysDown[65]) // a
		{
			scene->GetActiveModel()->TranslateModel(glm::vec3(-0.2, 0, 0));
		}

		if (imgui->KeysDown[68]) // d
		{
			scene->GetActiveModel()->TranslateModel(glm::vec3(0.2, 0, 0));
		}

		if (imgui->KeysDown[83]) // s
		{
			scene->GetActiveModel()->TranslateModel(glm::vec3(0, 0, 0.2));
		}

		if (imgui->KeysDown[87]) // w
		{
			scene->GetActiveModel()->TranslateModel(glm::vec3(0, 0, -0.2));
		}
	}

	if (!imgui->WantCaptureMouse)
	{
		if (zoomChanged)
		{
			scene->GetActiveCamera().Zoom(zoomFactor);
			zoomChanged = false;
		}
	}
}

void Cleanup()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

//-----------------------------------------------------------------------------
// Is called when the window is resized
//-----------------------------------------------------------------------------
void glfw_OnFramebufferSize(GLFWwindow* window, int width, int height)
{
	windowWidth = width;
	windowHeight = height;
	glViewport(0, 0, windowWidth, windowHeight);
	scene->GetActiveCamera().SetAspectRatio(GetAspectRatio());
}

void glfw_OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	zoomFactor = glm::pow(1.1, -yoffset);
	zoomChanged = true;
}

float GetAspectRatio()
{
	return static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
}

void DrawImguiMenus()
{
	ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_Once);
	{
		ImGui::Begin("Scene Menu");
		if (ImGui::ColorEdit3("Clear Color", (float*)&clearColor))
		{
			glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
		}

		if (ImGui::CollapsingHeader("Ambient Light"))
		{
			glm::vec3 modelColor = scene->GetAmbientLight().GetColor();
			if (ImGui::ColorEdit3("Ambient Light Color", (float*)&modelColor))
			{
				//scene->GetAmbientLight().SetColor(modelColor);
				scene->GetAmbientLight().SetColor(modelColor);
			}
		}

		if (ImGui::CollapsingHeader("Point Light"))
		{
			if (ImGui::Button("Add Point Light"))
			{
				//Choose color randomly
				std::random_device rd;
				std::mt19937 mt(rd());
				std::uniform_real_distribution<double> dist(0, 1);
				scene->AddPointLight(std::make_shared<PointLight>(glm::vec3(0, 0, 0), glm::vec3(dist(mt), dist(mt), dist(mt))));
			}

			const char** items;
			std::vector<std::string> pointStrings;
			items = new const char*[scene->GetPointLightCount()];
			for (int i = 0; i < scene->GetPointLightCount(); i++)
			{
				std::ostringstream s;
				s << "Point Light " << i;
				std::string mystring = s.str();
				pointStrings.push_back(mystring);
			}

			for (int i = 0; i < scene->GetPointLightCount(); i++)
			{
				items[i] = pointStrings[i].c_str();
			}

			int currentPoint = scene->GetActivePointLightIndex();
			ImGui::Combo("Active Point Light", &currentPoint, items, scene->GetPointLightCount());

			if (currentPoint != scene->GetActivePointLightIndex())
			{
				scene->SetActivePointIndex(currentPoint);
				//scene->GetPointLight(currentPoint).SetAspectRatio(GetAspectRatio());
			}

			delete items;
			
			glm::vec3 modelColor = scene->GetPointLight(currentPoint)->GetColor();
			if (ImGui::ColorEdit3("Point Light Color", (float*)&modelColor))
			{
				scene->GetPointLight(currentPoint)->SetColor(modelColor);
			}

			float T = 1;
			ImGui::SliderFloat("ptLocal Scale", &(T), 0.95 , 1.05);
			scene->GetPointLight(currentPoint)->ScaleModel(T);

			T = 1;
			ImGui::SliderFloat("ptWorld Scale", &(T), 0.95, 1.05);
			scene->GetPointLight(currentPoint)->ScaleWorld(T);
			
				
			T = 0;
			ImGui::SliderFloat("ptLocal Rotate (X)", &(T), -M_PI/40, M_PI/40);
			scene->GetPointLight(currentPoint)->RotateXModel(T);

			T = 0;
			ImGui::SliderFloat("ptLocal Rotate (Y)", &(T), -M_PI / 40, M_PI / 40);
			scene->GetPointLight(currentPoint)->RotateYModel(T);

			T = 0;
			ImGui::SliderFloat("ptLocal Rotate (Z)", &(T), -M_PI / 40, M_PI / 40);
			scene->GetPointLight(currentPoint)->RotateZModel(T);

			T = 0;
			ImGui::SliderFloat("ptWorld Rotate (X)", &(T), -M_PI / 40, M_PI / 40);
			scene->GetPointLight(currentPoint)->RotateXWorld(T);

			T = 0;
			ImGui::SliderFloat("ptWorld Rotate (Y)", &(T), -M_PI / 40, M_PI / 40);
			scene->GetPointLight(currentPoint)->RotateYWorld(T);

			T = 0;
			ImGui::SliderFloat("ptWorld Rotate (Z)", &(T), -M_PI / 40, M_PI / 40);
			scene->GetPointLight(currentPoint)->RotateZWorld(T);

			T = 0;
			ImGui::SliderFloat("ptWorld Translate (X)", &(T), -0.5, 0.5);
			scene->GetPointLight(currentPoint)->TranslateWorld(glm::vec3(T,0,0));

			T = 0;
			ImGui::SliderFloat("ptWorld Translate (Y)", &(T), -0.5, 0.5);
			scene->GetPointLight(currentPoint)->TranslateWorld(glm::vec3(0, T, 0));

			T = 0;
			ImGui::SliderFloat("ptWorld Translate (Z)", &(T), -0.5, 0.5);
			scene->GetPointLight(currentPoint)->TranslateWorld(glm::vec3(0, 0, T));
		}

		if (ImGui::CollapsingHeader("Parallel Light"))
		{
			if (ImGui::Button("Add Parallel Light"))
			{
				//Choose color randomly
				std::random_device rd;
				std::mt19937 mt(rd());
				std::uniform_real_distribution<double> dist(0, 1);
				scene->AddParallelLight(std::make_shared<Parallellight>(glm::vec3(0, 0, 0), glm::vec3(5, 0, 0), glm::vec3(dist(mt), dist(mt), dist(mt))));
			}

			const char** items;
			std::vector<std::string> ParallelStrings;
			items = new const char*[scene->GetParallelLightCount()];
			for (int i = 0; i < scene->GetParallelLightCount(); i++)
			{
				std::ostringstream s;
				s << "Parallel Light " << i;
				std::string mystring = s.str();
				ParallelStrings.push_back(mystring);
			}

			for (int i = 0; i < scene->GetParallelLightCount(); i++)
			{
				items[i] = ParallelStrings[i].c_str();
			}

			int currentParallel = scene->GetActiveParallelLightIndex();
			ImGui::Combo("Active Parallel Light", &currentParallel, items, scene->GetParallelLightCount());

			if (currentParallel != scene->GetActiveParallelLightIndex())
			{
				scene->SetActiveParralelIndex(currentParallel);
			}

			delete items;

			glm::vec3 modelColor = scene->GetParallelLight(currentParallel)->GetColor();
			if (ImGui::ColorEdit3("Parallel Light Color", (float*)&modelColor))
			{
				scene->GetParallelLight(currentParallel)->SetColor(modelColor);
			}

			float T = 1;
			ImGui::SliderFloat("prLocal Scale", &(T), 0.95, 1.05);
			scene->GetParallelLight(currentParallel)->ScaleModel(T);

			T = 1;
			ImGui::SliderFloat("prWorld Scale", &(T), 0.95, 1.05);
			scene->GetParallelLight(currentParallel)->ScaleWorld(T);


			T = 0;
			ImGui::SliderFloat("prLocal Rotate (X)", &(T), -M_PI / 40, M_PI / 40);
			scene->GetParallelLight(currentParallel)->RotateXModel(T);

			T = 0;
			ImGui::SliderFloat("prLocal Rotate (Y)", &(T), -M_PI / 40, M_PI / 40);
			scene->GetParallelLight(currentParallel)->RotateYModel(T);

			T = 0;
			ImGui::SliderFloat("prLocal Rotate (Z)", &(T), -M_PI / 40, M_PI / 40);
			scene->GetParallelLight(currentParallel)->RotateZModel(T);

			T = 0;
			ImGui::SliderFloat("prWorld Rotate (X)", &(T), -M_PI / 40, M_PI / 40);
			scene->GetParallelLight(currentParallel)->RotateXWorld(T);

			T = 0;
			ImGui::SliderFloat("prWorld Rotate (Y)", &(T), -M_PI / 40, M_PI / 40);
			scene->GetParallelLight(currentParallel)->RotateYWorld(T);

			T = 0;
			ImGui::SliderFloat("prWorld Rotate (Z)", &(T), -M_PI / 40, M_PI / 40);
			scene->GetParallelLight(currentParallel)->RotateZWorld(T);

			T = 0;
			ImGui::SliderFloat("prWorld Translate (X)", &(T), -0.5, 0.5);
			scene->GetParallelLight(currentParallel)->TranslateWorld(glm::vec3(T, 0, 0));

			T = 0;
			ImGui::SliderFloat("prWorld Translate (Y)", &(T), -0.5, 0.5);
			scene->GetParallelLight(currentParallel)->TranslateWorld(glm::vec3(0, T, 0));

			T = 0;
			ImGui::SliderFloat("prWorld Translate (Z)", &(T), -0.5, 0.5);
			scene->GetParallelLight(currentParallel)->TranslateWorld(glm::vec3(0, 0, T));
		}
		

		if (ImGui::CollapsingHeader("Cameras"))
		{
			const char** items;
			std::vector<std::string> cameraStrings;
			items = new const char*[scene->GetCameraCount()];
			for (int i = 0; i < scene->GetCameraCount(); i++)
			{
				std::ostringstream s;
				s << "Camera " << i;
				std::string mystring = s.str();
				cameraStrings.push_back(mystring);
			}

			for (int i = 0; i < scene->GetCameraCount(); i++)
			{
				items[i] = cameraStrings[i].c_str();
			}

			int currentCamera = scene->GetActiveCameraIndex();
			ImGui::Combo("Active Camera", &currentCamera, items, scene->GetCameraCount());

			if (currentCamera != scene->GetActiveCameraIndex())
			{
				scene->SetActiveCameraIndex(currentCamera);
				scene->GetActiveCamera().SetAspectRatio(GetAspectRatio());
			}

			delete items;

			int newProjectionType = scene->GetActiveCamera().IsPrespective() ? 0 : 1;
			ImGui::RadioButton("Perspective", &newProjectionType, 0);
			ImGui::RadioButton("Orthographic", &newProjectionType, 1);

			if (newProjectionType == 0)
			{
				scene->GetActiveCamera().SwitchToPrespective();
				if (ImGui::SliderFloat("Fovy", &scene->GetActiveCamera().fovy, 0.0f, M_PI)){}
				if (ImGui::SliderFloat("Near", &scene->GetActiveCamera().zNear, 1.0f, 10.0f)){}
				if (ImGui::SliderFloat("Far", &scene->GetActiveCamera().zFar, 1.0f, 10.0f)){}
			}
			else if (newProjectionType == 1)
			{
				scene->GetActiveCamera().SwitchToOrthographic();
				if (ImGui::SliderFloat("Height", &scene->GetActiveCamera().height, 0.0f, M_PI)) {}
				if (ImGui::SliderFloat("Near", &scene->GetActiveCamera().zNear, 1.0f, 10.0f)) {}
				if (ImGui::SliderFloat("Far", &scene->GetActiveCamera().zFar, 1.0f, 10.0f)) {}
			}
			////////////////////////////////////////////////////////////////
			if (ImGui::Button("Add Camera"))
			{

				std::random_device rd;
				std::mt19937 mt(rd());
				std::uniform_real_distribution<double> dist(0, 2 * M_PI);
				std::uniform_real_distribution<double> dist2(2, 10);
				double angle = dist(mt);
				double radius = dist2(mt);

				glm::vec4 eye = glm::vec4(radius * glm::cos(angle), 0, radius * glm::sin(angle), 1);
				glm::vec4 at = glm::vec4(0, 0, 0, 1);
				glm::vec4 up = glm::vec4(0, 1, 0, 1);
				scene->AddCamera(Camera(eye, at, up, GetAspectRatio())); //-scene->GetActiveCamera().GetEye()
			}

			float T = 1;
			ImGui::SliderFloat("CLocal Scale", &(T), 0.95, 1.05);
			scene->GetActiveCamera().ScaleXModel(T);

			T = 1;
			ImGui::SliderFloat("CWorld Scale", &(T), 0.95, 1.05);
			scene->GetActiveCamera().ScaleWorld(T);


			T = 0;
			ImGui::SliderFloat("CLocal Rotate (X)", &(T), -M_PI / 40, M_PI / 40);
			scene->GetActiveCamera().RotateXModel(T);

			T = 0;
			ImGui::SliderFloat("CLocal Rotate (Y)", &(T), -M_PI / 40, M_PI / 40);
			scene->GetActiveCamera().RotateYModel(T);

			T = 0;
			ImGui::SliderFloat("CLocal Rotate (Z)", &(T), -M_PI / 40, M_PI / 40);
			scene->GetActiveCamera().RotateZModel(T);

			T = 0;
			ImGui::SliderFloat("CWorld Rotate (X)", &(T), -M_PI / 40, M_PI / 40);
			scene->GetActiveCamera().RotateXWorld(T);

			T = 0;
			ImGui::SliderFloat("CWorld Rotate (Y)", &(T), -M_PI / 40, M_PI / 40);
			scene->GetActiveCamera().RotateYWorld(T);

			T = 0;
			ImGui::SliderFloat("CWorld Rotate (Z)", &(T), -M_PI / 40, M_PI / 40);
			scene->GetActiveCamera().RotateZWorld(T);

			T = 0;
			ImGui::SliderFloat("CWorld Translate (X)", &(T), -0.5, 0.5);
			scene->GetActiveCamera().TranslateWorld(glm::vec3(T, 0, 0));

			T = 0;
			ImGui::SliderFloat("CWorld Translate (Y)", &(T), -0.5, 0.5);
			scene->GetActiveCamera().TranslateWorld(glm::vec3(0, T, 0));

			T = 0;
			ImGui::SliderFloat("CWorld Translate (Z)", &(T), -0.5, 0.5);
			scene->GetActiveCamera().TranslateWorld(glm::vec3(0, 0, T));
			/////////////////////////////////////////////////////
		}

		if (ImGui::CollapsingHeader("Models"))
		{
			const char** items;
			std::vector<std::string> modelStrings;
			items = new const char*[scene->GetModelCount()];
			for (int i = 0; i < scene->GetModelCount(); i++)
			{
				std::ostringstream s;
				s << scene->GetModel(i)->GetModelName();
				std::string mystring = s.str();
				modelStrings.push_back(mystring);
			}

			for (int i = 0; i < scene->GetModelCount(); i++)
			{
				items[i] = modelStrings[i].c_str();
			}

			int currentModelIndex = scene->GetActiveModelIndex();
			ImGui::Combo("Active Model", &currentModelIndex, items, scene->GetModelCount());

			if (currentModelIndex != scene->GetActiveModelIndex())
			{
				scene->SetActiveModelIndex(currentModelIndex);
			}

			delete items;

			glm::vec3 modelColor = scene->GetActiveModel()->GetColor();
			if (ImGui::ColorEdit3("Model Color", (float*)&modelColor))
			{
				scene->GetActiveModel()->SetColor(modelColor);
			}


			if (ImGui::ColorEdit3("Diffuse Color", (float*)&(scene->GetActiveModel()->_diffuseColor))){}
			if (ImGui::ColorEdit3("Specular Color", (float*)&(scene->GetActiveModel()->_specuralColor))) {}

            if (ImGui::ColorEdit3("Ambient Color Wire", (float*)&(scene->GetActiveModel()->_ambientColorLines))) {}
            if (ImGui::ColorEdit3("Diffuse Color Wire", (float*)&(scene->GetActiveModel()->_diffuseColorLines))) {}
            if (ImGui::ColorEdit3("Specular Color Wire", (float*)&(scene->GetActiveModel()->_specularColorLines))) {}

			ImGui::SliderFloat("Ka", &(scene->GetActiveModel()->_Ka), 0.0f, 1.0f);
			ImGui::SliderFloat("Kd", &(scene->GetActiveModel()->_Kd), 0.0f, 1.0f);
			ImGui::SliderFloat("Ks", &(scene->GetActiveModel()->_Ks), 0.0f, 1.0f);
			ImGui::SliderFloat("exp", &(scene->GetActiveModel()->_exp), 0.0f, 1.0f);
			
			ImGui::Checkbox("Texture", &(scene->GetActiveModel()->_isTexNeeded));
            ImGui::Checkbox("Wire-Off", &(scene->GetActiveModel()->_isWireMode));
            ImGui::Checkbox("Bump mapping", &(scene->GetActiveModel()->_isNoisyMode));
			ImGui::Checkbox("Toon-Shading", &(scene->GetActiveModel()->_isToonShading));
            ImGui::SliderInt("Toon-Shading Range", &(scene->GetActiveModel()->_toonRange), 1, 256);
            
            std::shared_ptr<MeshModel> meshModel = std::dynamic_pointer_cast<MeshModel>(scene->GetActiveModel());
			if (meshModel)
			{
				float T = 1;
				ImGui::SliderFloat("MLocal Scale", &(T), 0.95, 1.05);
				meshModel->ScaleXModel(T);

				T = 1;
				ImGui::SliderFloat("MWorld Scale", &(T), 0.95, 1.05);
				meshModel->ScaleWorld(T);


				T = 0;
				ImGui::SliderFloat("MLocal Rotate (X)", &(T), -M_PI / 40, M_PI / 40);
				meshModel->RotateXModel(T);

				T = 0;
				ImGui::SliderFloat("MLocal Rotate (Y)", &(T), -M_PI / 40, M_PI / 40);
				meshModel->RotateYModel(T);

				T = 0;
				ImGui::SliderFloat("MLocal Rotate (Z)", &(T), -M_PI / 40, M_PI / 40);
				meshModel->RotateZModel(T);

				T = 0;
				ImGui::SliderFloat("MWorld Rotate (X)", &(T), -M_PI / 40, M_PI / 40);
				meshModel->RotateXWorld(T);

				T = 0;
				ImGui::SliderFloat("MWorld Rotate (Y)", &(T), -M_PI / 40, M_PI / 40);
				meshModel->RotateYWorld(T);

				T = 0;
				ImGui::SliderFloat("MWorld Rotate (Z)", &(T), -M_PI / 40, M_PI / 40);
				meshModel->RotateZWorld(T);

				T = 0;
				ImGui::SliderFloat("MWorld Translate (X)", &(T), -0.5, 0.5);
				meshModel->TranslateWorld(glm::vec3(T, 0, 0));

				T = 0;
				ImGui::SliderFloat("MWorld Translate (Y)", &(T), -0.5, 0.5);
				meshModel->TranslateWorld(glm::vec3(0, T, 0));

				T = 0;
				ImGui::SliderFloat("MWorld Translate (Z)", &(T), -0.5, 0.5);
				meshModel->TranslateWorld(glm::vec3(0, 0, T));
				

			}
		}

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	{
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoFocusOnAppearing;
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open", "CTRL+O"))
				{
					nfdchar_t *outPath = NULL;
					nfdresult_t result = NFD_OpenDialog("obj;png,jpg", NULL, &outPath);
					if (result == NFD_OKAY) {
						scene->AddModel(Utils::LoadMeshModel(outPath));
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
