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
	normals(normals)
{
	resetModel();
}

MeshModel::MeshModel(std::shared_ptr<MeshModel> model) :
	modelName(model->GetModelName()),
	vertices(model->GetVertices()),
	faces(model->GetFaces()),
	normals(model->GetNormals())
{
	resetModel();
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

//const glm::vec3& MeshModel::GetColor() const
//{
//	return color;
//}