#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <memory>
#include "MeshModel.h"
#include "Camera.h"
#include "PointLight.h"
#include "ParallelLight.h"
#include "AmbientLight.h"

#define PHONG			0
#define GOURAUD			1
#define FLAT			2

#define POINT_LIGHT		0
#define PARALLEL_LIGHT	1
#define AMBIENT_LIGHT	2

/*
 * Scene class.
 * This class holds all the scene information (models, cameras, lights, etc..)
 */
class Scene {
private:
	std::vector<std::shared_ptr<MeshModel>> models;
	std::vector< std::shared_ptr<Camera>> cameras;
	std::vector< std::shared_ptr<PointLight>> PointLights;
	std::vector< std::shared_ptr<ParallelLight>> ParallelLights;
	std::shared_ptr<AmbientLight> Ambient;
public:
    float gaussianKernel3x3[3][3] = {  
                                    {0, 0, 0},
                                    {0, 0, 0},
                                    {0, 0, 0}
                                    };
    float gaussianKernel5x5[5][5] = {
                                    {0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0}
                                    };
    float gaussianKernel10x10[10][10] = {
                                        {0, 0, 0, 0, 0},
                                        {0, 0, 0, 0, 0},
                                        {0, 0, 0, 0, 0},
                                        {0, 0, 0, 0, 0},
                                        {0, 0, 0, 0, 0},
                                        {0, 0, 0, 0, 0},
                                        {0, 0, 0, 0, 0},
                                        {0, 0, 0, 0, 0},
                                        {0, 0, 0, 0, 0},
                                        {0, 0, 0, 0, 0}
                                        };
	int CurrCam, SizeCam;
	int CurrPoint, SizePoint;
	int CurrParallel, SizeParallel;
	int activeModelIndex, gridCounter;
	int shadingType,gaussianRadius,gaussianMaskSize;
    int bloom, gaussianBlur;
    int kernelM, kernelN;
    float bloomThresh;
	
	Scene();
    ~Scene();
	void AddModel(const std::shared_ptr<MeshModel>& model);
	const int GetModelCount() const;
	void Scene::AddPointLight(std::shared_ptr<MeshModel> model, int windowHeight, int windowWidth);
	void Scene::AddParallelLight(glm::vec3 DirectionVector=glm::vec3(1,0,0));
	void Scene::AddCamera(std::shared_ptr<MeshModel> model, int windowHeight, int windowWidth, glm::vec3 eye = glm::vec3(0, 0, 0));
	const int GetCameraCount() const;
	const int GetPointLightCount() const;
	const int GetParallelLightCount() const;
	std::shared_ptr<Camera> GetCamera(int index);
	std::shared_ptr<PointLight> GetPointLight(int index);
	void SetActiveCameraIndex(int index);
	const int GetActiveCameraIndex() const;
	void SetActiveModelIndex(int index);
	const int GetActiveModelIndex() const;
	std::vector<Face> Scene::getModelfaces(int indexModel) const;
	std::vector<glm::vec3> Scene::getModelNormals(int indexModel) const;
	std::vector<Face> Scene::getCamerafaces(int indexModel);
	std::vector<Face> Scene::getPointLightfaces(int indexModel);
	std::vector<glm::vec3> Scene::getCameraNormals(int indexModel);
	std::vector<glm::vec3> Scene::getPointLightNormals(int indexModel);
	glm::vec3 Scene::getCameraVertices(int indexModel, int indexVertex);
	glm::vec3 Scene::getLightPointVertices(int indexModel, int indexVertex);
	glm::vec3 Scene::getModelVertices(int indexModel, int indexVertex) const;
	std::shared_ptr<MeshModel> Scene::GetModel(int index) const;
	std::shared_ptr<ParallelLight> Scene::GetParallelLight(int index) const;
	std::shared_ptr<PointLight> Scene::GetPointLight(int index) const;
	std::shared_ptr<AmbientLight> Scene::GetAmbient() const;
	int Scene::modelName2Index(std::string name);
    // glm::vec3 Scene::GetModelMassCenter(std::shared_ptr<MeshModel> model); // BUG - do not use it - required a series test before changing to use this general-mass function
	void Scene::SetFocusOnCurrentModel();
	void WholeWorldTransfer(glm::mat4x4& Tcm, glm::mat4x4& Tc);
    void Scene::buildGaussian();
};