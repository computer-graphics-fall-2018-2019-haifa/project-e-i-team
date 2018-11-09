#pragma once
#include <glm/glm.hpp>
#include <string>
#include <iostream>
#include <memory>
#include "Face.h"

using namespace std;

/*
 * MeshModel class.
 * This class represents a mesh model (with faces and normals informations).
 * You can use Utils::LoadMeshModel to create instances of this class from .obj files.
 */
class MeshModel
{
private:
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	glm::mat4x4 worldTransform;
	glm::vec4 color;
	std::string modelName;

public:
	MeshModel(const std::vector<Face>& faces, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::string& modelName = "");
	virtual ~MeshModel();

	void SetWorldTransformation(const glm::mat4x4& worldTransform);
	const glm::mat4x4& GetWorldTransformation() const;

	const glm::vec4& GetColor() const;
	void SetColor(const glm::vec4& color);

	const std::string& GetModelName();


	//Elias emplementation:
	glm::vec3 GetVerticeByIndex(int index) {
		return vertices[index];
	}
	//Elias emplementation:
	std::vector<Face> GetFaces() {
		return faces;
	}
	// Add more methods/functionality as needed...
};
