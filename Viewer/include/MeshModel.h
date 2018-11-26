#pragma once
#include <glm/glm.hpp>
#include <string>
#include <iostream>
#include <memory>
#include "Face.h"

using namespace std;

#define MESH_MODEL_TYPE 0
#define CAMERA_MODEL_TYPE 1

#define FSCALE_DEF 80.0f

static glm::vec4 VERTEX_NORMAL_COLOR = glm::vec4(1.0f, 0.0f, 0.0f, 1.00f);
static glm::vec4 FACE_NORMAL_COLOR = glm::vec4(0.8f, 0.0f, 0.5f, 1.00f);

#define NORMAL_LENGTH 40.0f

#define MAX_SCALE_FACTOR 2000.0f
#define MIN_SCALE_FACTOR 50.0f
#define SCALE_OBJ_FACTOR 4.0f

#define MAX_TRANSLATION_LENGTH 20.f
#define MIN_TRANSLATION_LENGTH -20.f

// smooth moving:
#define XTRANS_FACTOR 0.5f
#define YTRANS_FACTOR 0.5f

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
	glm::mat4x4 allWorldTransform;
	glm::vec4 color;
	std::string modelName;
public:
	bool showFaceNormals;
	bool showVertexNormals;
	glm::vec4 fNcolor;
	glm::vec4 vNcolor;
	float fScale, fRotatex, fRotatey, fRotatez;
	float fTranslatex, fTranslatey, fTranslatez;
	float fNlength, vNlength;

	MeshModel(){}
	MeshModel(const std::vector<Face>& faces, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::string& modelName = "");
	MeshModel(std::shared_ptr<MeshModel> model);
	virtual ~MeshModel();

	void SetAllWorldTransformation(float fAllScale) {
		if (fAllScale < 0) {
			fAllScale = 1 / -fAllScale;
		}
		allWorldTransform[0][0] * fAllScale; allWorldTransform[1][1] * fAllScale; allWorldTransform[2][2] * fAllScale;
	}

	glm::mat4x4 GetAllWorldTransformation() {
		return allWorldTransform;
	}

	void SetWorldTransformation(const glm::mat4x4& worldTransform);
	const glm::mat4x4& GetWorldTransformation() const;

	const glm::vec4& GetColor() const;
	void SetColor(const glm::vec4& color);

	std::vector<glm::vec3> GetNormals() {
		return normals;
	}

	std::vector<Face> GetFaces() {
		return faces;
	}

	std::vector<glm::vec3> GetVertices() {
		return vertices;
	}

	std::string GetModelName() {
		return modelName;
	}

	void resetModel(float fScaleDef = FSCALE_DEF,
					glm::vec4 vcolorDef = VERTEX_NORMAL_COLOR,
					glm::vec4 fcolorDef = FACE_NORMAL_COLOR,
					float vertexNlength = NORMAL_LENGTH,
					float faceNlength = NORMAL_LENGTH) {
		worldTransform = glm::mat4x4(1);
		allWorldTransform = glm::mat4x4(1);
		showFaceNormals = false;
		showVertexNormals = false;
		fNcolor = fcolorDef;
		vNcolor = vcolorDef;
		vNlength = vertexNlength;
		fNlength = faceNlength;
		fScale = fScaleDef;
		fRotatex = 0.0f;
		fRotatey = 0.0f;
		fRotatez = 0.0f;
		fTranslatex = 0.0f;
		fTranslatey = 0.0f;
		fTranslatez = 0.0f;
	}

	//Elias emplementation:
	glm::vec3 GetVerticeByIndex(int index) {
		return vertices[index];
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
