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
	bool showFaceNormals;
	bool showVertexNormals;
	glm::vec4 fNcolor;
	glm::vec4 vNcolor;
	int fNlength;
	int vNlength;

public:
	MeshModel(const std::vector<Face>& faces, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::string& modelName = "");
	virtual ~MeshModel();

	void SetWorldTransformation(const glm::mat4x4& worldTransform);
	const glm::mat4x4& GetWorldTransformation() const;

	const glm::vec4& GetColor() const;
	void SetColor(const glm::vec4& color);

	const std::string& GetModelName();

	std::vector<glm::vec3> GetNormals() {
		return normals;
	}

	//Elias emplementation:
	glm::vec3 GetVerticeByIndex(int index) {
		return vertices[index];
	}
	//Elias emplementation:
	std::vector<Face> GetFaces() {
		return faces;
	}

	void SetFaceNormalLength(float length) {
		fNlength = length;
	}

	void SetFaceNormalColor(glm::vec4 color) {
		fNcolor = color;
	}

	float GetFaceNormalLength() {
		return fNlength;
	}

	glm::vec4 GetFaceNormalColor() {
		return fNcolor;
	}

	bool GetFaceNormalView() {
		return showFaceNormals;
	}

	void SetFaceNormalView(bool NeedShowNormals) {
		showFaceNormals = NeedShowNormals;
	}

	void SetVertexNormalView(bool NeedShowNormals) {
		showVertexNormals = NeedShowNormals;
	}

	bool GetVertexNormalView() {
		return showVertexNormals;
	}

	void SetVertexNormalLength(float length) {
		vNlength = length;
	}

	void SetVertexNormalColor(glm::vec4 color) {
		vNcolor = color;
	}

	float GetVertexNormalLength() {
		return vNlength;
	}

	glm::vec4 GetVertexNormalColor() {
		return vNcolor;
	}

	// Add more methods/functionality as needed...
};
