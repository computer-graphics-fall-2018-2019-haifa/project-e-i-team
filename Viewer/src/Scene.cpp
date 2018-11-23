#include "Scene.h"
#include "MeshModel.h"
#include <string>

Scene::Scene() : currentActiveCamera(0),activeCameraIndex(0),activeModelIndex(0), gridCounter(0)
{
}

void Scene::AddModel(const std::shared_ptr<MeshModel>& model)
{
	models.push_back(model);
}

const int Scene::GetModelCount() const
{
	return models.size();
}

void Scene::AddCamera(std::shared_ptr<MeshModel> model)
{
	cout << "hi" << endl;
	
	//1.
	//previous camera position, is that aim to be the correct form?!?
	//Camera c = Camera(glm::vec4(200, 200, 0, 1), glm::vec4(0, 0, 0, 1), glm::vec4(200, 200, 200, 1));

	//2.
	//picking camera random position on space:
	Camera c = Camera(model,glm::vec4(rand() % 201, rand() % 201, rand() % 201, 1),
					  glm::vec4(rand() % 201, rand() % 201, rand() % 201, 1),
					  glm::vec4(rand() % 201, rand() % 201, rand() % 201, 1));
	

	//3.
	//TODO: need to really adding new camera which is beloging to "Camera i" string at Cameras section
	//all of this with out putting camera at the View side (transformations)! [part of the excercise]

	cameras.push_back(c);
	this->activeCameraIndex++;
}

const int Scene::GetCameraCount() const
{
	return cameras.size();
}

void Scene::SetActiveCameraIndex(int index)
{
	// implementation suggestion...
	if (index >= 0 && index < cameras.size())
	{
		activeCameraIndex = index;
	}
}

Camera* Scene::GetCamera(int index) {
	if (index  < 0 || index >= cameras.size()) {
		return NULL;
	}
	return &cameras[index];
}

const int Scene::GetActiveCameraIndex() const
{
	return activeCameraIndex;
}

void Scene::SetActiveModelIndex(int index)
{
	// implementation suggestion...
	if (index >= 0 && index < models.size())
	{
		activeModelIndex = index;
	}
}

const int Scene::GetActiveModelIndex() const
{
	return activeModelIndex;
}


