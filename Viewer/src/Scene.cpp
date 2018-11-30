#include "Scene.h"
#include "MeshModel.h"
#include <string>

Scene::Scene() : currentActiveCamera(0),activeCameraIndex(0),activeModelIndex(0), gridCounter(0) {}

void Scene::AddModel(const std::shared_ptr<MeshModel>& model) { models.push_back(model); }
const int Scene::GetModelCount() const { return models.size(); }

void Scene::AddCamera(std::shared_ptr<MeshModel> model, int windowHeight , glm::vec3 eye)
{
	
	if ((eye.x == 0) && (eye.y == 0) && (eye.z == 0)) {
		int x = (rand() % windowHeight) - windowHeight / 2;
		int y = (rand() % windowHeight) - windowHeight / 2;
		int z = (rand() % windowHeight) - windowHeight / 2;
		eye = glm::vec3(x, y, z);	
	}
	

	glm::vec3 at = glm::vec3(0, 0, 0);
	glm::vec3 help_up = glm::vec3(0, 1, 0);
	glm::vec3 vec_eye_at = at - eye;

	help_up = glm::cross(vec_eye_at, help_up);
	glm::vec3 up = glm::cross(vec_eye_at, help_up) + eye;


	glm::vec4 up4 = glm::vec4(up.x, up.y, up.z, 1);
	glm::vec4 eye4 = glm::vec4(eye.x, eye.y, eye.z, 1);
	glm::vec4 at4 = glm::vec4(at.x, at.y, at.z, 1);

	/*glm::vec3 at = glm::vec3(0, 0, 0);
	glm::vec3 rand = glm::vec3(3, 2, 1);
	glm::vec3 vec_eye_at = at - eye;
	glm::vec3 vector_eye_rand = rand - eye;
	glm::vec3 up = glm::cross(vec_eye_at, vector_eye_rand) + eye;
	glm::vec4 up4 = glm::vec4(up.x, up.y, up.z, 1);
	glm::vec4 eye4 = glm::vec4(eye.x, eye.y, eye.z, 1);
	glm::vec4 at4 = glm::vec4(at.x, at.y, at.z, 1);*/

	Camera c(model, eye4, at4, up4);

	cameras.push_back(std::make_shared<Camera>(c));
	this->activeCameraIndex++;
}

const int Scene::GetCameraCount() const { return cameras.size(); }
void Scene::SetActiveCameraIndex(int index) {
	if (index >= 0 && index < cameras.size()) { activeCameraIndex = index; }
}
std::shared_ptr<Camera> Scene::GetCamera(int index) {
	if (index  < 0 || index >= cameras.size()) { return NULL; }
	return cameras[index];
}
const int Scene::GetActiveCameraIndex() const { return activeCameraIndex; }
void Scene::SetActiveModelIndex(int index) {
	if (index >= 0 && index < models.size()) { activeModelIndex = index; }
}
const int Scene::GetActiveModelIndex() const { return activeModelIndex; }


