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
	int scaleFactor;

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
	std::vector<Face> getModelFaces(int index) {
		return models[index]->GetFaces();
	}

	glm::vec3 getModelVertices(int indexModel, int indexVertex) {
		return models[indexModel]->GetVerticeByIndex(indexVertex);
	}

	std::shared_ptr<MeshModel> GetModel(int index) const {
		return models[index];
	}

	// Add more methods as needed...

};