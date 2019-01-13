#define _USE_MATH_DEFINES

#include "Trans.h"
#include "PointLight.h"
#include "Renderer.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>



PointLight::PointLight(std::shared_ptr<MeshModel> model) :
	MeshModel(model), Ld(1.0f), Ls(1.0f), Center(BoundMiddle)
{
	int x = 0, y = 0, z = 0;
	Center = glm::vec3(x, y, z);
	showFaceNormals = false; showVertexNormals = false;
	UpdateworldTransform(Trans::getScale4x4(0.05f));
	UpdateworldTransform(Trans::getyRotate4x4(0.005f));
	UpdateLeftworldTransform(Trans::getTranslate4x4(-1.0f, -1.0f, 1.0f));
	UpdateworldTransform(Trans::getTranslate4x4(x, y, z));
}

glm::vec3 PointLight::GetLocationAfterTrans() {
    glm::vec4 Center4(Center, 1.0f);
    glm::vec4 location = GetWorldTransformation() * Center4;
    return (glm::vec3(location) / location.w);
}

glm::vec3 PointLight::GetLocationAfterTrans(glm::mat4x4 camTrans) {
	glm::vec4 Center4(Center, 1.0f);
	glm::vec4 location = camTrans * GetWorldTransformation() * Center4;
	return (glm::vec3(location) / location.w);
}

PointLight::~PointLight() {}