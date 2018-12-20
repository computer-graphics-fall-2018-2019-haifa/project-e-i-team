#define _USE_MATH_DEFINES

#include "Trans.h"
#include "PointLight.h"
#include "Renderer.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>



PointLight::PointLight(std::shared_ptr<MeshModel> model) :
	MeshModel(model)
{
	/*int Radius = 800;
	int RadiusPow2 = pow(Radius, 2);
	int x = (rand() % (2 * (Radius - 1))) - (Radius - 1);
	int x_pow = pow(x, 2);
	int help = sqrt(RadiusPow2 - x_pow);
	int y = (rand() % (2 * help)) - (help);
	int y_pow = pow(y, 2);
	int z = sqrt(RadiusPow2 - x_pow - y_pow);
	cout << "(" << x << " , " << y << " , " << z << ")" << endl;*/
	int x = 0, y = 0, z = 0;

	Center = glm::vec3(x, y, z);
	
	showFaceNormals = false; showVertexNormals = false;

	UpdateworldTransform(Trans::getScale4x4(0.05f));
	UpdateworldTransform(Trans::getyRotate4x4(0.005f));
	UpdateLeftworldTransform(Trans::getTranslate4x4(-1.0f, -1.0f, 1.0f));
	UpdateworldTransform(Trans::getTranslate4x4(x, y, z));
	
	//UpdateLeftworldTransform(Trans::getTranslate4x4(float(-BoundMiddle.x), float(-BoundMiddle.y), float(-BoundMiddle.z)));
	//UpdateworldTransform(Trans::getScale4x4(10));
	//UpdateworldTransform(Trans::getTranslate4x4(x + BoundMiddle.x, y + BoundMiddle.y, z + BoundMiddle.z));
	
	
}

PointLight::~PointLight() {}