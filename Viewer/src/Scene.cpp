#include "Scene.h"
#include "MeshModel.h"
#include <string>
#define ORIGIN AmbientLight(glm::vec3(0, 0, 0))

Scene::Scene() : CurrCam(0), CurrPoint(0), SizePoint(0), SizeParallel(0), CurrParallel(0), SizeCam(0), activeModelIndex(0), gridCounter(0), 
shadingType(0), kernelM(5), kernelN(5), gaussianRadius(3), gaussianBlur(false), bloom(false), gaussianMaskSize(5), bloomThresh(0.7f)
{
	Ambient = (std::make_shared<AmbientLight>(AmbientLight(ORIGIN)));
    buildGaussian();
}

Scene::~Scene() {}

void Scene::buildGaussian() {
    switch (gaussianMaskSize) {
        case 3: {
            int std = 1;
            if (gaussianRadius > 0) {
                std = gaussianRadius;
            }
            Trans::buildGaussianKernel3x3(gaussianKernel3x3, kernelM, kernelN, std);        break;
        }
        case 5: {
            int std = 3;
            if (gaussianRadius > 0) {
                std = gaussianRadius;
            }
            Trans::buildGaussianKernel5x5(gaussianKernel5x5, kernelM, kernelN, std);        break;
        }
        case 10: {
            int std = 5;
            if (gaussianRadius > 0) {
                std = gaussianRadius;
            }
            Trans::buildGaussianKernel10x10(gaussianKernel10x10, kernelM, kernelN, std);    break;
        }
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

glm::vec3 Scene::getLightPointVertices(int indexModel, int indexVertex) {
	return PointLights[indexModel]->GetVerticeByIndex(indexVertex);
}

glm::vec3 Scene::getModelVertices(int indexModel, int indexVertex) const {
	return models[indexModel]->GetVerticeByIndex(indexVertex);
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

/*
glm::vec3 Scene::GetModelMassCenter(std::shared_ptr<MeshModel> model) {
	cout << "Into: GetModelMassCenter" << endl;
	float x_avg = 0.0f, y_avg = 0.0f, z_avg = 0.0f;
	vector<float> estPointsx, estPointsy, estPointsz;
	std::vector<Face> faces = model->GetFaces();
	for (int i = 0; i < faces.size(); i++) {
		int v0 = faces.at(i).GetVertexIndex(0) - 1;
		int v1 = faces.at(i).GetVertexIndex(1) - 1;
		int v2 = faces.at(i).GetVertexIndex(2) - 1;
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
	return glm::vec3(x_avg, y_avg, z_avg);
}
*/

void Scene::SetFocusOnCurrentModel() {
	std::shared_ptr<Camera> camera = GetCamera(CurrCam);
	std::shared_ptr<MeshModel> model = GetModel(activeModelIndex);

	glm::vec4 _at = model->GetWorldTransformation() * glm::vec4(model->BoundMiddle.x, model->BoundMiddle.y, model->BoundMiddle.z, 1);
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

void Scene::WholeWorldTransfer(glm::mat4x4& Tcm,glm::mat4x4& Tc) {
	for (int i = 0; i < GetModelCount(); i++) {
		std::shared_ptr<MeshModel> model = GetModel(i);
		glm::vec3 mass = model->GetWorldTransformation() * glm::vec4(model->BoundMiddle.x, model->BoundMiddle.y, model->BoundMiddle.z, 1.0f);
		model->UpdateworldTransform(Trans::get2InitAxis4x4(mass, Tcm * Tc));
	}
	for (int i = 0; i < GetCameraCount(); i++) {
		std::shared_ptr<Camera> camera = GetCamera(i);
		if (i != SizeCam) {
			glm::vec3 mass = camera->GetWorldTransformation() * glm::vec4(camera->BoundMiddle.x, camera->BoundMiddle.y, camera->BoundMiddle.z, 1.0f);
			camera->UpdateworldTransform(Trans::get2InitAxis4x4(mass, Tcm * Tc));
		}
	}
}