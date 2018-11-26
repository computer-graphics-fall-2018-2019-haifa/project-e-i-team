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
	//1.
	//previous camera position, is that aim to be the correct form?!?
	//Camera c = Camera(glm::vec4(200, 200, 0, 1), glm::vec4(0, 0, 0, 1), glm::vec4(200, 200, 200, 1));

	//2.
	//picking camera random position on space:
	cout << "activeCameraIndex = " << activeCameraIndex << endl;
	Camera c(model, glm::vec4(-1, -1, 0, 1), glm::vec4(0, 0, 0, 1), glm::vec4(-1, -1, 1, 1));
	if (activeCameraIndex == 0) {
		c = Camera(model, glm::vec4(1, 1, 0, 1), glm::vec4(0, 0, 0, 1), glm::vec4(1, 1, 1, 1)); // eye,at,up
	}
	
	c = Camera(model, glm::vec4(1, 1, 0, 1), glm::vec4(0, 0, 0, 1), glm::vec4(1, 1, 1, 1)); // eye,at,up
	cameras.push_back(std::make_shared<Camera>(c));
	this->activeCameraIndex++;

	//3.
	//TODO: need to really adding new camera which is beloging to "Camera i" string at Cameras section
	//all of this with out putting camera at the View side (transformations)! [part of the excercise]
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

std::shared_ptr<Camera> Scene::GetCamera(int index) {
	if (index  < 0 || index >= cameras.size()) {
		return NULL;
	}
	return cameras[index];
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


