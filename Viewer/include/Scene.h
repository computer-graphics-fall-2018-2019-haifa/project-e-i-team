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
	std::vector< std::shared_ptr<Camera>> cameras;
public:
	
	int currentActiveCamera;
	int activeCameraIndex;
	int activeModelIndex;
	int gridCounter;
	Scene();

	void AddModel(const std::shared_ptr<MeshModel>& model);
	const int GetModelCount() const;

	void Scene::AddCamera(std::shared_ptr<MeshModel> model, int windowHeight, glm::vec3 eye=glm::vec3(0,0,0));

	const int GetCameraCount() const;

	std::shared_ptr<Camera> GetCamera(int index);

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

	std::vector<Face> getCamerafaces(int indexModel) {
		return cameras.at(indexModel)->GetFaces();
	}

	std::vector<glm::vec3> getCameraNormals(int indexModel) {
		return cameras.at(indexModel)->GetNormals();
	}

	glm::vec3 getCameraVertices(int indexModel, int indexVertex) {
		return cameras.at(indexModel)->GetVerticeByIndex(indexVertex);
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

	glm::vec3 GetModelMassCenter(std::shared_ptr<MeshModel> model) {
		float x_avg = 0.0f, y_avg = 0.0f, z_avg = 0.0f;
		vector<float> estPointsx, estPointsy, estPointsz;
		std::vector<Face> faces = model->GetFaces();
		for (int i = 0; i < faces.size(); i++) {
			int v0 = faces[i].GetVertexIndex(0) - 1;
			int v1 = faces[i].GetVertexIndex(1) - 1;
			int v2 = faces[i].GetVertexIndex(2) - 1;
			glm::vec3 dd = getModelVertices(activeModelIndex, v0);
			glm::vec3 vec0 = glm::vec3(
				getModelVertices(activeModelIndex, v0).x,
				getModelVertices(activeModelIndex, v0).y,
				getModelVertices(activeModelIndex, v0).z
			);
			glm::vec3 vec0t = model->GetWorldTransformation() * glm::vec4(vec0.x, vec0.y, vec0.z, 1.0f);
			glm::vec3 vec1 = glm::vec3(
				getModelVertices(activeModelIndex, v1).x,
				getModelVertices(activeModelIndex, v1).y,
				getModelVertices(activeModelIndex, v1).z
			);
			glm::vec3 vec1t = model->GetWorldTransformation() * glm::vec4(vec1.x, vec1.y, vec1.z, 1.0f);
			glm::vec3 vec2 = glm::vec3(
				getModelVertices(activeModelIndex, v2).x,
				getModelVertices(activeModelIndex, v2).y,
				getModelVertices(activeModelIndex, v2).z
			);
			glm::vec3 vec2t = model->GetWorldTransformation() * glm::vec4(vec2.x, vec2.y, vec2.z, 1.0f);
			estPointsx.push_back(vec0t.x);
			estPointsx.push_back(vec1t.x);
			estPointsx.push_back(vec2t.x);
			estPointsy.push_back(vec0t.y);
			estPointsy.push_back(vec1t.y);
			estPointsy.push_back(vec2t.y);
			estPointsz.push_back(vec0t.z);
			estPointsz.push_back(vec1t.z);
			estPointsz.push_back(vec2t.z);
		}
		for (int i = 0; i < estPointsx.size(); i++) {
			x_avg += estPointsx.at(i);
		}
		if (estPointsx.size() > 0) {
			x_avg = x_avg / estPointsx.size();
		}
		for (int i = 0; i < estPointsy.size(); i++) {
			y_avg += estPointsy.at(i);
		}
		if (estPointsy.size() > 0) {
			y_avg = y_avg / estPointsy.size();
		}
		for (int i = 0; i < estPointsz.size(); i++) {
			z_avg += estPointsz.at(i);
		}
		if (estPointsz.size() > 0) {
			z_avg = z_avg / estPointsz.size();
		}
		//cout << "massCenter => (" << x_avg << "," << y_avg << "," << z_avg << ")" << endl;
		return glm::vec3(x_avg, y_avg, z_avg);
	}
};