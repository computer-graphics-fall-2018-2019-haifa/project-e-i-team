#include "MeshModel.h"
#include "Utils.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

MeshModel::MeshModel(const std::vector<Face>& faces, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::string& modelName) :
	modelName(modelName),
	vertices(vertices),
	faces(faces),
	normals(normals),
	worldTransform(glm::mat4x4(1)),
	showFaceNormals(false),
	showVertexNormals(false),
	fNcolor(FACE_NORMAL_COLOR),
	vNcolor(VERTEX_NORMAL_COLOR),
	vNlength(NORMAL_LENGTH),
	fNlength(NORMAL_LENGTH),
	fScale(80.0f),
	fRotatex(0.0f),
	fRotatey(0.0f),
	fRotatez(0.0f),
	fTranslatex(0.0f),
	fTranslatey(0.0f),
	fTranslatez(0.0f)
{

}

MeshModel::~MeshModel()
{

}

void MeshModel::SetWorldTransformation(const glm::mat4x4& worldTransform)
{
	this->worldTransform = worldTransform;
}

const glm::mat4x4& MeshModel::GetWorldTransformation() const
{
	return worldTransform;
}

void MeshModel::SetColor(const glm::vec4& color)
{
	this->color = color;
}

const glm::vec4& MeshModel::GetColor() const
{
	return color;
}

const std::string& MeshModel::GetModelName()
{
	return modelName;
}