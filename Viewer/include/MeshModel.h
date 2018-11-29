#pragma once
#include <glm/glm.hpp>
#include <string>
#include <iostream>
#include <memory>
#include "Face.h"
#include <stdio.h>
#include <time.h>
#include "Trans.h"

using namespace std;

#define MESH_MODEL_TYPE 0
#define CAMERA_MODEL_TYPE 1

#define POS_DOUBLE_PI 2.1f*M_PI

#define MAX_NORMAL_LENGTH 100.0f
#define MIN_NORMAL_LENGTH 1.0f

#define MAX_SCALE_FACTOR 100000.0f
#define MIN_SCALE_FACTOR 1.0f
#define SCALE_OBJ_FACTOR 35.0f
#define CAMERA_BASIC_SIZE 10000000000.0f // it is as big as we can achieve for this moment

#define MAX_TRANSLATION_LENGTH 100000.0f
#define MIN_TRANSLATION_LENGTH -100000.0f

// smooth moving:
#define XTRANS_FACTOR 50.0f
#define YTRANS_FACTOR 50.0f
#define BLACK_COLOR_LINE glm::vec4(0.0f, 0.0f, 0.0f,1.0f)

static glm::vec4 VERTEX_NORMAL_COLOR = glm::vec4(1.0f, 0.0f, 0.0f, 1.00f);
static glm::vec4 FACE_NORMAL_COLOR = glm::vec4(0.8f, 0.0f, 0.5f, 1.00f);

/*
getRandColor() success depend on srand(time(NULL)) in the top on main function
*/
static glm::vec3* getRandColor() {
	int r = rand() % 256;
	int g = rand() % 256;
	int b = rand() % 256;
	glm::vec3* color = &glm::vec3(r, g, b);
	//std::cout << "(R=" << r << ",G=" << g << ",B=" << b << ")" << std::endl;
	return color;
}

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
	std::string modelName;
public:
	glm::vec3 BoundMin;
	glm::vec3 BoundMax;
	glm::vec3 color;
	glm::vec3 BoundingBoxColor;
	bool showFaceNormals;
	bool showVertexNormals;
	bool showBoundingBox;
	glm::vec4 fNcolor;
	glm::vec4 vNcolor;
	float fScale, fRotatex, fRotatey, fRotatez;
	float fTranslatex, fTranslatey, fTranslatez;
	float fNlength, vNlength;

	MeshModel(){}
	MeshModel(const std::vector<Face>& faces, const std::vector<glm::vec3>& vertices,const std::vector<glm::vec3>& normals, glm::vec3 BoundMin, glm::vec3 BoundMax, const std::string& modelName = "");
	MeshModel(std::shared_ptr<MeshModel> model, float defsize = CAMERA_BASIC_SIZE);
	virtual ~MeshModel();

	void SetWorldTransformation(const glm::mat4x4& worldTransform);
	const glm::mat4x4& GetWorldTransformation() const;

	void SetColor(const glm::vec4& color);
	std::vector<glm::vec3> GetNormals() { return normals; }
	std::vector<Face> GetFaces() { return faces; }
	std::vector<glm::vec3> GetVertices() { return vertices; }
	std::string GetModelName() { return modelName; }
	void UpdateworldTransform(glm::mat4x4 T) { worldTransform = T * worldTransform; }

	void resetModel(float fScaleDef = SCALE_OBJ_FACTOR,
					glm::vec4 vcolorDef = VERTEX_NORMAL_COLOR,
					glm::vec4 fcolorDef = FACE_NORMAL_COLOR,
					glm::vec3* modelColor = getRandColor(),
					float vertexNlength = MAX_NORMAL_LENGTH,
					float faceNlength = MAX_NORMAL_LENGTH) {
		worldTransform = Trans::getScale4x4(fScaleDef);
		showFaceNormals = false;
		showVertexNormals = false;
		showBoundingBox = false;
		fNcolor = fcolorDef;
		vNcolor = vcolorDef;
		BoundingBoxColor = *modelColor;
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
	}

	glm::vec3 GetVerticeByIndex(int index) { return vertices[index]; }
	void SetFaceNormalLength(float length) { fNlength = length; }
	void SetFaceNormalColor(glm::vec4 color) { fNcolor = color; }
	float GetFaceNormalLength() { return fNlength; }
	glm::vec4 GetFaceNormalColor() { return fNcolor; }
	bool GetFaceNormalView() { return showFaceNormals; }
	void SetFaceNormalView(bool NeedShowNormals) { showFaceNormals = NeedShowNormals; }
	void SetVertexNormalView(bool NeedShowNormals) { showVertexNormals = NeedShowNormals; }
	bool GetVertexNormalView() { return showVertexNormals; }
	void SetVertexNormalLength(float length) { vNlength = length; }
	void SetVertexNormalColor(glm::vec4 color) { vNcolor = color; } 
	float GetVertexNormalLength() { return vNlength; }
	glm::vec4 GetVertexNormalColor() { return vNcolor; }
};
