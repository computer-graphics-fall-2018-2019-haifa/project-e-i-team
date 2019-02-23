#include "PointLight.h"
#include <algorithm>

#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>

#include <nfd.h>

#include <stdio.h>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <sstream>
#include <stdlib.h>



PointLight::PointLight(const glm::vec3& position, const glm::vec3& color):
	position(position),Light(color)
{
}


PointLight::~PointLight()
{
}

glm::vec3& PointLight::GetPosition()
{
	return position;
}


glm::vec3 PointLight::GetPositionAfterTrans() {
	glm::vec4 Pos(position, 1.0f);
	Pos = GetWorldTransformation() * GetModelTransformation() * Pos;
	return (glm::vec3(Pos) / Pos.w);
}

