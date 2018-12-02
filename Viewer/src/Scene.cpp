#include "Scene.h"
#include "MeshModel.h"
#include <string>

Scene::Scene() : currentActiveCamera(0),activeCameraIndex(0),activeModelIndex(0), gridCounter(0) {}

void Scene::AddModel(const std::shared_ptr<MeshModel>& model) { models.push_back(model); }
const int Scene::GetModelCount() const { return models.size(); }

void Scene::AddCamera(std::shared_ptr<MeshModel> model, int windowHeight , glm::vec3 eye)
{
	if ((eye.x == 0) && (eye.y == 0) && (eye.z == 0)) {
		int Radius = 800;
		int RadiusPow2 = pow(Radius, 2);
		int x = (rand() % (2 * (Radius - 1))) - (Radius - 1);
		int x_pow = pow(x, 2);
		int help = sqrt(RadiusPow2 - x_pow);
		int y = (rand() % (2 * help)) - (help);
		int y_pow = pow(y, 2);
		int z = sqrt(RadiusPow2 - x_pow - y_pow);
		//cout << "(" << x << " , " << y << " , " << z << ")" << endl;

		/*int x = (rand() % windowHeight) - windowHeight / 2;
		int y = (rand() % windowHeight) - windowHeight / 2;
		int z = (rand() % windowHeight) - windowHeight / 2;*/
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
	glm::vec3 mass = model->GetWorldTransformation() * glm::vec4(model->BoundMiddle.x, model->BoundMiddle.y, model->BoundMiddle.z,1.0f);//GetModelMassCenter(model);
	Camera c(model, eye4, at4, up4,mass);

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


