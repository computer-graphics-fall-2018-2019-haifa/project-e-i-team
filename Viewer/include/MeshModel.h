#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include "MeshModel.h"
#include "Face.h"
#include "Texture2D.h"

#define TOON_RANGE  256

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textureCoords;
};

class MeshModel {
public:
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> textureCoords;

	std::vector<Vertex> modelVertices;

	glm::mat4x4 modelTransform;
	glm::mat4x4 worldTransform;

	std::string modelName;
    
    glm::vec3 _diffuseColor;
    glm::vec3 _specuralColor;
    float _Ka, _Kd, _Ks, _exp;
    int _toonRange;

    bool _isTexNeeded, _isWireMode,_isToonShading, _isNoisyMode;

	GLuint vbo;
	GLuint vao;

    Texture2D _texture;

	glm::vec3 color;
    glm::vec3 _ambientColorLines, _diffuseColorLines, _specularColorLines;
	float WScale, WRotatex, WRotatey, WRotatez, MScale, MRotatex, MRotatey, MRotatez;
	float WTranslatex, WTranslatey, WTranslatez;
	
	MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> textureCoords, const std::string& modelName = "");
	virtual ~MeshModel();

    glm::vec3 GetDiffuseColor();
    glm::vec3 GetSpecuralColor();

    float GetKa();
    float GetKd();
    float GetKs();

    float GetExponent();
    bool IsTextureNeeded();

    bool IsWireOnlyMode();

    bool isToonShadingMode();

	const glm::mat4x4& GetWorldTransformation() const;
	const glm::mat4x4& GetModelTransformation() const;

	void SetWorldTransformation(const glm::mat4x4& worldTransform);
	void SetModelTransformation(const glm::mat4x4& modelTransform);

	const glm::vec3& GetColor() const;
	void SetColor(const glm::vec3& color);

	const std::string& GetModelName();

	const std::vector<Vertex>& GetModelVertices();

	void TranslateModel(const glm::vec3& translationVector);
	void TranslateWorld(const glm::vec3& translationVector);

	void RotateXModel(double angle);
	void RotateYModel(double angle);
	void RotateZModel(double angle);
	void ScaleXModel(double factor);
	void ScaleYModel(double factor);
	void ScaleZModel(double factor);
	void ScaleModel(double factor);

	void RotateXWorld(double angle);
	void RotateYWorld(double angle);
	void RotateZWorld(double angle);
	void ScaleXWorld(double factor);
	void ScaleYWorld(double factor);
	void ScaleZWorld(double factor);
	void ScaleWorld(double factor);

	GLuint GetVAO() const;

    int GetToonShadingRange();

    glm::vec3 GetAmbientColorLines();
    glm::vec3 GetDiffuseColorLines();
    glm::vec3 GetSpecularColorLines();

    bool isNoisyMode();
};
