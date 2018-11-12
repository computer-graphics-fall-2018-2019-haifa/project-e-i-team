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

	int activeCameraIndex;
	int activeModelIndex;

public:
	Scene();

	void AddModel(const std::shared_ptr<MeshModel>& model);
	const int GetModelCount() const;

	void AddCamera(const Camera& camera);
	const int GetCameraCount() const;

	void SetActiveCameraIndex(int index);
	const int GetActiveCameraIndex() const;

	void SetActiveModelIndex(int index);
	const int GetActiveModelIndex() const;

	//Elias implementation:
	std::vector<Face> getModelFaces(int index) const{
		return models[index]->GetFaces();
	}

	std::vector<glm::vec3> getModelNormals(int indexModel) const {
		return models[indexModel]->GetNormals();
	}

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