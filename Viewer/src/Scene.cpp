#include "Utils.h"
#include "Scene.h"
#include "MeshModel.h"
#include <string>
#define ORIGIN AmbientLight(glm::vec3(0, 0, 0))

Scene::Scene() : Debug_mode(false), illuminationMode(true), needCreative(false),CurrCam(0), CurrPoint(0), SizePoint(0), SizeParallel(0), CurrParallel(0), SizeCam(0), activeModelIndex(0), gridCounter(0),
shadingType(1), kernelM(5), kernelN(5), gaussianRadius(3.0f), gaussianBlur(false), bloom(false), gaussianMaskSize(1), bloomThresh(0.7f)
{
	Ambient = (std::make_shared<AmbientLight>(AmbientLight(ORIGIN)));
    buildGaussian();
}

Scene::~Scene() {}

void Scene::buildGaussian() {
    switch (gaussianMaskSize) {
        case 0: {
            int std = 1;
            if (gaussianRadius > 0) {
                std = gaussianRadius;
            }
            Trans::buildGaussianKernel3x3(gaussianKernel3x3, kernelM, kernelN, std);        break;
        }
        case 1: {
            int std = 3;
            if (gaussianRadius > 0) {
                std = gaussianRadius;
            }
            Trans::buildGaussianKernel5x5(gaussianKernel5x5, kernelM, kernelN, std);        break;
        }
        //case 3: {
        //    int std = 5;
        //    if (gaussianRadius > 0) {
        //        std = gaussianRadius;
        //    }
        //    Trans::buildGaussianKernel10x10(gaussianKernel10x10, kernelM, kernelN, std);    break;
        //}
        default:    Trans::buildGaussianKernel5x5(gaussianKernel5x5, kernelM, kernelN, 3);  break;
    }
}

void Scene::AddModel(const std::shared_ptr<MeshModel>& model) { models.push_back(model); }
const int Scene::GetModelCount() const { return models.size(); }


void Scene::AddCamera(std::shared_ptr<MeshModel> model, int windowHeight, int windowWidth, glm::vec3 eye)
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
		eye = glm::vec3(x, y, z);	
	}
	glm::vec3 at = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 help_up = glm::vec3(0.0f, -1.0f, 0.0f);
	glm::vec3 vec_eye_at = at - eye;
	help_up = glm::cross(vec_eye_at, help_up);
	glm::vec3 up = glm::cross(vec_eye_at, help_up) + eye;
	glm::vec4 up4 = glm::vec4(up.x, up.y, up.z, 1.0f);
	glm::vec4 eye4 = glm::vec4(eye.x, eye.y, eye.z, 1.0f);
	glm::vec4 at4 = glm::vec4(at.x, at.y, at.z, 1.0f);
	glm::vec3 mass = model->GetWorldTransformation() * glm::vec4(model->BoundMiddle.x, model->BoundMiddle.y, model->BoundMiddle.z,1.0f);
	Camera c(model, eye4, at4, up4,mass);

	cameras.push_back(std::make_shared<Camera>(c));
	this->SizeCam++;
}


void Scene::AddPointLight(std::shared_ptr<MeshModel> model, int windowHeight, int windowWidth)
{
	PointLight p(model);
	PointLights.push_back(std::make_shared<PointLight>(p));
	this->SizePoint++;
}

void Scene::AddParallelLight(glm::vec3 DirectionVector)
{
	ParallelLight p(DirectionVector);
	ParallelLights.push_back(std::make_shared<ParallelLight>(p));
	this->SizeParallel++;
}

std::vector<Face> Scene::getModelfaces(int indexModel) const {
	return models[indexModel]->GetFaces();
}

std::vector<glm::vec3> Scene::getModelNormals(int indexModel) const {
	return models[indexModel]->GetNormals();
}

std::vector<Face> Scene::getCamerafaces(int indexModel) {
	return cameras.at(indexModel)->GetFaces();
}

std::vector<Face> Scene::getPointLightfaces(int indexModel) {
	return PointLights.at(indexModel)->GetFaces();
}

glm::vec3 Scene::getCameraVertices(int indexModel, int indexVertex) {
	return cameras.at(indexModel)->GetVerticeByIndex(indexVertex);
}

glm::vec3 Scene::getCameraNormals(int indexModel, int indexVertex) {
	return cameras.at(indexModel)->GetNormalByIndex(indexVertex);
}

glm::vec3 Scene::getLightPointVertices(int indexModel, int indexVertex) {
	return PointLights[indexModel]->GetVerticeByIndex(indexVertex);
}

glm::vec3 Scene::getLightPointNormals(int indexModel, int indexVertex) {
	return PointLights[indexModel]->GetNormalByIndex(indexVertex);
}

glm::vec3 Scene::getModelVertices(int indexModel, int indexVertex) const {
	return models[indexModel]->GetVerticeByIndex(indexVertex);
}

glm::vec3 Scene::getModelNormals(int indexModel, int indexVertex) const {
	return models[indexModel]->GetNormalByIndex(indexVertex);
}

std::shared_ptr<MeshModel> Scene::GetModel(int index) const {
	if (models.size() == 0) {
		return nullptr;
	}
	return models[index];
}

std::shared_ptr<PointLight> Scene::GetPointLight(int index) const {
	if (PointLights.size() == 0) {
		return nullptr;
	}
	return PointLights[index];
}

std::shared_ptr<AmbientLight> Scene::GetAmbient() const {
	return Ambient;
}

std::shared_ptr<ParallelLight> Scene::GetParallelLight(int index) const {
	if (ParallelLights.size() == 0) {
		return nullptr;
	}
	return ParallelLights[index];
}

int Scene::modelName2Index(std::string name) {
	for (size_t i = 0; i < models.size(); i++) {
		if (models[i]->GetModelName().compare(name) == 0) {
			return i;
		}
	}
	return -1;
}

std::vector<glm::vec3> Scene::getCameraNormals(int indexModel) {
	return cameras.at(indexModel)->GetNormals();
}

std::vector<glm::vec3> Scene::getPointLightNormals(int indexModel) {
	return PointLights.at(indexModel)->GetNormals();
}

const int Scene::GetCameraCount() const { 
	return cameras.size(); 
}

const int Scene::GetPointLightCount() const {
	return PointLights.size();
}

const int Scene::GetParallelLightCount() const {
	return ParallelLights.size();
}

void Scene::SetActiveCameraIndex(int index) {
	if (index >= 0 && index < cameras.size()) { SizeCam = index; }
}

std::shared_ptr<Camera> Scene::GetCamera(int index) {
	if (index  < 0 || index >= cameras.size()) { return NULL; }
	return cameras[index];
}

std::shared_ptr<PointLight> Scene::GetPointLight(int index) {
	if (index < 0 || index >= PointLights.size()) { return NULL; }
	return PointLights[index];
}

const int Scene::GetActiveCameraIndex() const { 
	return SizeCam;
}

void Scene::SetActiveModelIndex(int index) {
	if (index >= 0 && index < models.size()) { activeModelIndex = index; }
}

const int Scene::GetActiveModelIndex() const { 
	return activeModelIndex; 
}

void Scene::SetFocusOnCurrentModel() {
	std::shared_ptr<Camera> camera = GetCamera(CurrCam);
	std::shared_ptr<MeshModel> model = GetModel(activeModelIndex);

	glm::vec4 _at = model->GetWorldTransformation() * glm::vec4(model->BoundMiddle, 1);
	glm::vec3 at(_at.x, _at.y, _at.z);

	glm::vec3 help_up = glm::vec3(0, 1, 0);
	glm::vec3 vec_eye_at = at - camera->origin_eye;

	help_up = glm::cross(vec_eye_at, help_up);
	glm::vec3 up = glm::cross(vec_eye_at, help_up) + camera->origin_eye;

	glm::vec4 up4 = glm::vec4(up.x, up.y, up.z, 1);
	glm::vec4 eye4 = glm::vec4(camera->origin_eye.x, camera->origin_eye.y, camera->origin_eye.z, 1);
	glm::vec4 at4 = glm::vec4(at.x, at.y, at.z, 1);

	camera->SetCameraLookAt(eye4, at4, up4);
}

bool Scene::isIlluminationModeOn() {
    return illuminationMode;
}