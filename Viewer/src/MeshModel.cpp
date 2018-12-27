#include "MeshModel.h"
#include "Utils.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

MeshModel::MeshModel(const std::vector<Face>& faces, const std::vector<glm::vec3>& vertices,const std::vector<glm::vec3>& normals, glm::vec3 BoundMin, glm::vec3 BoundMax, glm::vec3 BoundMiddle, const std::string& modelName) :
	modelName(modelName),
	vertices(vertices),
	faces(faces),
	normals(normals),
	BoundMin(BoundMin),
	BoundMax(BoundMax),
	BoundMiddle(BoundMiddle),
	Kd(1.0f), Ks(1.0f), alpha(45.0f), diffuseColor(0.5f, 0.5f, 0.5f), specularColor(0.5f,0.5f,0.5f),
	lightType(AMBIENT), estfNormal(0,0,0)
{
	resetModel();
}

MeshModel::MeshModel(std::shared_ptr<MeshModel> model, float defsize, bool showFNormals, bool showVNormals) :
	modelName(model->GetModelName()),
	vertices(model->GetVertices()),
	faces(model->GetFaces()),
	normals(model->GetNormals()),
	Kd(1.0f), Ks(1.0f), alpha(45.0f), diffuseColor(0.5f, 0.5f, 0.5f), specularColor(0.5f, 0.5f, 0.5f),
	lightType(AMBIENT), estfNormal(0, 0, 0)
{
	resetModel(defsize, showFNormals, showVNormals);
}
MeshModel::~MeshModel() {}

glm::vec3 MeshModel::GetModelLocationAfterTrans() {
	glm::vec4 BoundMiddle4(BoundMiddle.x, BoundMiddle.y, BoundMiddle.z, 1);
	glm::vec4 location = GetWorldTransformation() * BoundMiddle4;
	return glm::vec3(location.x, location.y, location.z);
}

std::vector<glm::vec3> MeshModel::GetNormals() {
	return normals;
}

std::vector<Face> MeshModel::GetFaces() {
	return faces;
}

std::vector<glm::vec3> MeshModel::GetVertices() {
	return vertices;
}

std::string MeshModel::GetModelName() {
	return modelName;
}

void MeshModel::UpdateworldTransform(glm::mat4x4 T) {
	worldTransform = T * worldTransform;
}

void MeshModel::UpdateLeftworldTransform(glm::mat4x4 T) {
	worldTransform = worldTransform * T;
}

void MeshModel::resetModel(float fScaleDef,
	bool showFNormals,
	bool showVNormals,
	bool isPointLight,
	glm::vec4 vcolorDef,
	glm::vec4 fcolorDef,
	glm::vec3* modelColor,
	float vertexNlength,
	float faceNlength) {
	worldTransform = Trans::getScale4x4(fScaleDef);
	if (modelName.find("Light Source") != std::string::npos) {
		showFaceNormals = false;
		showVertexNormals = false;
	} else {
		showFaceNormals = showFNormals;
		showVertexNormals = showVNormals;
	}
	showBoundingBox = false;
	fNcolor = fcolorDef;
	vNcolor = vcolorDef;
	BoundingBoxColor = BOUNDING_BOX_COLOR;
	color = *modelColor;
	vNlength = vertexNlength;
	fNlength = faceNlength;
	fScale = fScaleDef;
	fRotatex = 0.0f;
	fRotatey = 0.0f;
	fRotatez = 0.0f;
	fTranslatex = 0.0f;
	fTranslatey = 0.0f;
	fTranslatez = 0.0f;
	wfScale = fScaleDef;
	wfRotatex = 0.0f;
	wfRotatey = 0.0f;
	wfRotatez = 0.0f;
}

glm::vec3 MeshModel::GetVerticeByIndex(int index) {
	return vertices[index];
}

void MeshModel::SetFaceNormalLength(float length) {
	fNlength = length;
}

void MeshModel::SetFaceNormalColor(glm::vec4 color) {
	fNcolor = color;
}

float MeshModel::GetFaceNormalLength() {
	return fNlength;
}

glm::vec4 MeshModel::GetFaceNormalColor() {
	return fNcolor;
}

bool MeshModel::GetFaceNormalView() {
	return showFaceNormals;
}

void MeshModel::SetFaceNormalView(bool NeedShowNormals) {
	showFaceNormals = NeedShowNormals;
}

void MeshModel::SetVertexNormalView(bool NeedShowNormals) {
	showVertexNormals = NeedShowNormals;
}

bool MeshModel::GetVertexNormalView() {
	return showVertexNormals;
}

void MeshModel::SetVertexNormalLength(float length) {
	vNlength = length;
}

void MeshModel::SetVertexNormalColor(glm::vec4 color) {
	vNcolor = color;
}

float MeshModel::GetVertexNormalLength() {
	return vNlength;
}

glm::vec4 MeshModel::GetVertexNormalColor() {
	return vNcolor;
}

void MeshModel::SetWorldTransformation(const glm::mat4x4& worldTransform) { 
	this->worldTransform = worldTransform; 
}

const glm::mat4x4& MeshModel::GetWorldTransformation() const { 
	return worldTransform; 
}

void MeshModel::SetColor(const glm::vec4& color) { 
	this->color = color; 
}