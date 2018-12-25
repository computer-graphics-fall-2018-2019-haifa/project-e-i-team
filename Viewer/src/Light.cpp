#pragma once
#include "MeshModel.h"
#include <memory>
#include <glm/glm.hpp>
#include "Light.h"



Light::Light(float La, float Ld, float Ls) :
La(0.2f), Ld(0.2f), Ls(0.2f)
{}

Light::~Light(){}