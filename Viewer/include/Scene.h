#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <memory>
#include "MeshModel.h"
#include "Camera.h"

/*
 * Scene class.
 * This class holds all the scene information (models, cameras, lights, etc..)
 */
class Scene {
private:
	std::vector<std::shared_ptr<MeshModel>> models;
	std::vector<Camera> cameras;
public:
	int currentActiveCamera;
	int activeCameraIndex;
	int activeModelIndex;
	int gridCounter;
	Scene();

	void AddModel(const std::shared_ptr<MeshModel>& model);
	const int GetModelCount() const;

	void Scene::AddCamera(MeshModel* model);

	const int GetCameraCount() const;

	Camera* GetCamera(int index);

	void SetActiveCameraIndex(int index);
	const int GetActiveCameraIndex() const;

	void SetActiveModelIndex(int index);
	const int GetActiveModelIndex() const;

	//Elias implementation:
	std::vector<Face> getModelfaces(int indexModel) const{
		return models[indexModel]->GetFaces();
	}

	std::vector<glm::vec3> getModelNormals(int indexModel) const {
		return models[indexModel]->GetNormals();
	}

	//std::vector<Face> getCamerafaces(int indexCamera) const {
	//	return cameras.at(indexCamera).GetFaces();
	//}

	//std::vector<glm::vec3> getCameraNormals(int indexCamera) const {
	//	return cameras.at(indexCamera).GetNormals();
	//}

	glm::vec3 getModelVertices(int indexModel, int indexVertex) const{
		return models[indexModel]->GetVerticeByIndex(indexVertex);
	}

	std::shared_ptr<MeshModel> GetModel(int index) const {
		if (models.size() == 0) {
			return nullptr;
		}
		return models[index];
	}

	int modelName2Index(std::string name) {
		for (size_t i = 0; i < models.size(); i++){
			if (models[i]->GetModelName().compare(name) == 0) {
				return i;
			}
		}
		return -1;
	}
	// Add more methods as needed...

};