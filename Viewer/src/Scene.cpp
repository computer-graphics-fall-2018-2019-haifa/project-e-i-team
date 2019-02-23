#include "Scene.h"
#include "MeshModel.h"
#include <string>

Scene::Scene() :
	activeCameraIndex(0),
	activeModelIndex(0),
	activePointLightIndex(0),
    activeParallelLightIndex(0),
	ambientLight(glm::vec3(0,0,0), glm::vec3(1,1,1)) {}

void Scene::AddModel(const std::shared_ptr<MeshModel>& model) {
	models.push_back(model);
}

void Scene::AddCamera(const Camera& camera) {
	cameras.push_back(camera);
}

int Scene::GetModelCount() const {
    return models.size();
}

int Scene::GetCameraCount() const {
	return cameras.size();
}

std::shared_ptr<MeshModel> Scene::GetModel(int index) const {
	return models[index];
}

Camera& Scene::GetCamera(int index) {
	return cameras[index];
}

const Camera& Scene::GetCamera(int index) const {
	return cameras[index];
}

const Camera& Scene::GetActiveCamera() const {
	return cameras[activeCameraIndex];
}

Camera& Scene::GetActiveCamera() {
	return cameras[activeCameraIndex];
}

void Scene::SetActiveCameraIndex(int index) {
	if (index >= 0 && index < cameras.size()) {
		activeCameraIndex = index;
	}
}

void Scene::SetActivePointIndex(int index) {
	if (index >= 0 && index < Pointlights.size()) {
		activePointLightIndex = index;
	}
}

void Scene::SetActiveParralelIndex(int index) {
	if (index >= 0 && index < Parallellights.size()) {
		activeParallelLightIndex = index;
	}
}

const int Scene::GetActiveCameraIndex() const {
	return activeCameraIndex;
}

const int Scene::GetActivePointIndex() const {
	return activePointLightIndex;
}

const int Scene::GetActiveParallelIndex() const {
	return activeParallelLightIndex;
}

const int Scene::GetActivePointLightIndex() const {
	return activePointLightIndex;
}

const int Scene::GetActiveParallelLightIndex() const {
	return activeParallelLightIndex;
}

void Scene::SetActiveModelIndex(int index) {
	activeModelIndex = index;
}

const int Scene::GetActiveModelIndex() const {
	return activeModelIndex;
}

const std::shared_ptr<MeshModel>& Scene::GetActiveModel() const {
	return models[activeModelIndex];
}

void Scene::AddPointLight(const std::shared_ptr<PointLight>& Pointlight) {
	Pointlights.push_back(Pointlight);
}

void Scene::AddParallelLight(const std::shared_ptr<Parallellight>& Parallellight) {
	Parallellights.push_back(Parallellight);
}

int Scene::GetPointLightCount() const {
	return Pointlights.size();
}

int Scene::GetParallelLightCount() const {
	return Parallellights.size();
}

std::shared_ptr<PointLight> Scene::GetPointLight(int index) const {
	return Pointlights[index];
}

std::shared_ptr<Parallellight> Scene::GetParallelLight(int index) const {
	return Parallellights[index];
}

const std::vector<std::shared_ptr<PointLight>>& Scene::GetActivePointLights() const {
	return Pointlights;
}

const std::vector<std::shared_ptr<Parallellight>>& Scene::GetActiveParallelLights() const {
	return Parallellights;
}

AmbientLight& Scene::GetAmbientLight() {
	return ambientLight;
}