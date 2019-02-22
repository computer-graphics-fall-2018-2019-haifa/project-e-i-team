#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <memory>
#include "MeshModel.h"
#include "AmbientLight.h"
#include "PointLight.h"
#include "Camera.h"
#include "Parallellight.h"

class Scene {
private:
	std::vector<std::shared_ptr<MeshModel>> models;
	std::vector<std::shared_ptr<PointLight>> Pointlights;
	std::vector<std::shared_ptr<Parallellight>> Parallellights;
	AmbientLight ambientLight;
	std::vector<Camera> cameras;

	int activeCameraIndex;
	int activeModelIndex;
	int activePointLightIndex;
	int activeParallelLightIndex;
	

public:
	Scene();

	void AddModel(const std::shared_ptr<MeshModel>& model);
	int GetModelCount() const;
	std::shared_ptr<MeshModel> GetModel(int index) const;


	/*Parallellight& Scene::GetActiveParallel();
	PointLight& Scene::GetActivePoint();*/
	void Scene::SetActiveParralelIndex(int index);
	void Scene::SetActivePointIndex(int index);
	const int Scene::GetActivePointIndex() const;
	const int Scene::GetActiveParallelIndex() const;
	const int Scene::GetActivePointLightIndex() const;
	const int Scene::GetActiveParallelLightIndex() const;
	void AddCamera(const Camera& camera);
	int GetCameraCount() const;
	Camera& GetCamera(int index);
	const Camera& GetCamera(int index) const;

	void AddPointLight(const std::shared_ptr<PointLight>& light);
	int GetPointLightCount() const;
	std::shared_ptr<PointLight> GetPointLight(int index) const;
	

	AmbientLight& GetAmbientLight();

	const Camera& GetActiveCamera() const;
	Camera& GetActiveCamera();

	void SetActiveCameraIndex(int index);
	const int GetActiveCameraIndex() const;

	const std::shared_ptr<MeshModel>& GetActiveModel() const;

	void SetActiveModelIndex(int index);
	const int GetActiveModelIndex() const;



	void Scene::AddParallelLight(const std::shared_ptr<Parallellight>& Parallellight);
	int Scene::GetParallelLightCount() const;
	std::shared_ptr<Parallellight> Scene::GetParallelLight(int index) const;
	const std::vector<std::shared_ptr<PointLight>>& Scene::GetActivePointLights() const;
	const std::vector<std::shared_ptr<Parallellight>>& Scene::GetActiveParallelLights() const;
};