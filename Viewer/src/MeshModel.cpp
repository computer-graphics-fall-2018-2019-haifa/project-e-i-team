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
	BoundMiddle(BoundMiddle)
{
	resetModel();
}

MeshModel::MeshModel(std::shared_ptr<MeshModel> model, float defsize, bool showFNormals, bool showVNormals) :
	modelName(model->GetModelName()),
	vertices(model->GetVertices()),
	faces(model->GetFaces()),
	normals(model->GetNormals())
{
	resetModel(defsize, showFNormals, showVNormals);
}

MeshModel::~MeshModel() {}
void MeshModel::SetWorldTransformation(const glm::mat4x4& worldTransform) { this->worldTransform = worldTransform; }
const glm::mat4x4& MeshModel::GetWorldTransformation() const { return worldTransform; }
void MeshModel::SetColor(const glm::vec4& color) { this->color = color; }