#define _USE_MATH_DEFINES

#include "Renderer.h"
#include "InitShader.h"
#include "MeshModel.h"
#include <imgui/imgui.h>
#include <vector>
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <fenv.h>       /* fegetround, FE_* */
#include <math.h>       /* nearbyint */
#include "Trans.h"

using namespace std;

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)

Renderer::Renderer(int viewportWidth, int viewportHeight, int viewportX, int viewportY) :
	colorBuffer(nullptr),
	zBuffer(nullptr)
{
	initOpenGLRendering();
	SetViewport(viewportWidth, viewportHeight, viewportX, viewportY);
}

Renderer::~Renderer()
{
	if (colorBuffer) { delete[] colorBuffer; }
}

void Renderer::putPixel(int i, int j, const glm::vec3& color)
{
	if (i < 0) return; if (i >= viewportWidth) return;
	if (j < 0) return; if (j >= viewportHeight) return;
	
	colorBuffer[INDEX(viewportWidth, i, j, 0)] = color.x;
	colorBuffer[INDEX(viewportWidth, i, j, 1)] = color.y;
	colorBuffer[INDEX(viewportWidth, i, j, 2)] = color.z;
}

void Renderer::createBuffers(int viewportWidth, int viewportHeight)
{
	if (colorBuffer){ delete[] colorBuffer; }
	colorBuffer = new float[3* viewportWidth * viewportHeight];
	for (int x = 0; x < viewportWidth; x++) {
		for (int y = 0; y < viewportHeight; y++) { putPixel(x, y, glm::vec3(0.0f, 0.0f, 0.0f)); }
	}
}

void Renderer::ClearColorBuffer(const glm::vec3& color) {
	
	for (int i = 0; i < viewportWidth; i++) {
		
		for (int j = 0; j < viewportHeight; j++) {
		
			putPixel(i, j, color); 
		}
	}
}

void Renderer::SetViewport(int viewportWidth, int viewportHeight, int viewportX, int viewportY){
	this->viewportX = viewportX;
	this->viewportY = viewportY;
	this->viewportWidth = viewportWidth;
	this->viewportHeight = viewportHeight;
	createBuffers(viewportWidth, viewportHeight);
	createOpenGLBuffer();
}

/*
float Renderer::getTriangleArea(glm::vec2& a, glm::vec2& b, glm::vec2& c) {
	glm::vec2 proj_rot_vec = glm::normalize(-glm::dot(c - a, b - a) * (c - a) * Trans::getxRotate2x2(M_PI / 2.0f));
	glm::vec2 height = (proj_rot_vec - glm::normalize(b));
	float A = glm::length(height) * glm::length(c - a) / 2.0f;
	return A;
}
*/

glm::vec3& Renderer::interpolate_baricentric_coordinate(glm::vec2& p,glm::vec2& a, glm::vec2& b, glm::vec2& c,glm::vec3 color0, glm::vec3 color1, glm::vec3 color2) {
	float Wb = ((a.y - c.y) * (p.x - c.x) + (c.x - a.x) * (p.y - c.y)) / ((a.y - c.y) * (b.x - c.x) + (c.x - a.x) * (b.y - c.y));
	float Wa = ((c.y - b.y) * (p.x - c.x) + (b.x - c.x) * (p.y - c.y)) / ((a.y - c.y) * (b.x - c.x) + (c.x - a.x) * (b.y - c.y));
	float Wc = 1 - Wb - Wa;

	// Wa,Wb,Wc are weight which is depended on a,b,c relatively and provided as weight to the colors from a,b,c to interpolate:
	return glm::vec3(Wa*color0 + Wb * color1 + Wc * color2);
}

void Renderer::printTriangle(glm::vec2& a, glm::vec2& b, glm::vec2& c, glm::vec3& color) {
	float min_x = a.x;
	if (b.x < min_x) min_x = b.x;
	if (c.x < min_x) min_x = c.x;

	float min_y = a.y;
	if (b.y < min_y) min_y = b.y;
	if (c.y < min_y) min_y = c.y;

	float max_x = a.x;
	if (b.x > max_x) max_x = b.x;
	if (c.x > max_x) max_x = c.x;

	float max_y = a.y;
	if (b.y > max_y) max_y = b.y;
	if (c.y > max_y) max_y = c.y;

	for (int x = min_x; x <= max_x; x++) {
		for (int y = min_y; y <= max_y; y++) {
			glm::vec2 p(x, y);
			glm::vec2 w = CalculateW12(a, b, c, p);
			float w1 = w[0];
			float w2 = w[1];

			if ((w1 >= 0) && (w2 >= 0) && ((w1 + w2) <= 1)) {
				putPixel((viewportWidth / 2) + p.x, (viewportHeight / 2) + p.y, color);
			}
		}
	}
}

void Renderer::printTriangle(glm::vec2& a, glm::vec2& b, glm::vec2& c, glm::vec3& color0, glm::vec3& color1, glm::vec3& color2) {
	float min_x = a.x;
	if (b.x < min_x) min_x = b.x;
	if (c.x < min_x) min_x = c.x;

	float min_y = a.y;
	if (b.y < min_y) min_y = b.y;
	if (c.y < min_y) min_y = c.y;

	float max_x = a.x;
	if (b.x > max_x) max_x = b.x;
	if (c.x > max_x) max_x = c.x;

	float max_y = a.y;
	if (b.y > max_y) max_y = b.y;
	if (c.y > max_y) max_y = c.y;

	for (int x = min_x; x <= max_x ; x++) {
		for (int y = min_y; y <= max_y; y++) {
			glm::vec2 p(x, y);
			glm::vec2 w = CalculateW12(a, b, c, p);
			float w1 = w[0];
			float w2 = w[1];

			if ((w1 >= 0) && (w2 >= 0) && ((w1 + w2) <= 1)) {
				// baricentric coordinates for p = (w1,w2) color interpolation:
				glm::vec3 p_color = interpolate_baricentric_coordinate(glm::vec2(w1,w2),a, b, c, color0, color1, color2);
				putPixel((viewportWidth / 2) + p.x, (viewportHeight / 2) + p.y, p_color);
			}
		}
	}
}

glm::vec2 Renderer::CalculateW12(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 p) {
	float w1_up = a.x * (c.y - a.y) + (p.y - a.y) * (c.x - a.x) - p.x * (c.y - a.y);
	float w1_down = (b.y - a.y) * (c.x - a.x) - (b.x - a.x) * (c.y - a.y);
	float w1 = w1_up / w1_down;

	float w2_up = p.y - a.y - w1 * (b.y - a.y);
	float w2_down = c.y - a.y;
	float w2 = w2_up / w2_down;

	return glm::vec2(w1, w2);

}

// huge of complexity than Bresenham algorithm
void Renderer::NaiveAlg(float p1, float p2, float q1, float q2, const glm::vec3& color) {
	float delta_p = p2 - p1;
	float delta_q = q2 - q1;
	float m = delta_q / delta_p;
	float c = q1 - m * p1;
	
	float x,to,y;
	if (p1 < p2) {
		x = p1;
		to = p2;
	} else {
		x = p2;
		to = p1;
	}
	for (; x <= to; x++) {
		y = round(m*x + c);
		putPixel((viewportWidth / 2) + x, (viewportHeight / 2) + y, color);
	}
}

void Renderer::DrawLine(float p1, float p2, float q1, float q2, const glm::vec3& color) {
	float a = (q1 - q2) / (p1 - p2);
	if (a >= 0 && a <= 1) {
		if (p1 < p2) {
			BresenhamAlg(p1, p2, q1, q2, false, false, false, color);
		} else {
			BresenhamAlg(p2, p1, q2, q1, false, false, false, color);
		}
	} else if (a > 1) {
		if (q1 < q2) {
			BresenhamAlg(q1, q2, p1, p2, true, false, false, color);
		} else {
			BresenhamAlg(q2, q1, p2, p1, true, false, false, color);
		}
	} else if (a < 0 && a >= -1) {
		if (p1 < p2) {
			BresenhamAlg(p1, p2, q1, q2 + 2*(q1-q2), false, true, false, color);
		} else {
			BresenhamAlg(p2, p1, q2, q1 + 2 * (q2 - q1), false, true, false, color);
		}
	} else if (a < -1) {
		if (q1 < q2) {
			BresenhamAlg(q1, q2, p1, p2+2*(p1-p2), true, true, false, color);
		} else {
			BresenhamAlg(q1, q2 + 2 * (q1 - q2), p1, p2, true, false, true, color);
		}
	}
}

// fully tested
void Renderer::BresenhamAlg(float p1, float p2, float q1, float q2, bool switch_print, bool NegX, bool NegY, const glm::vec3& color) {
	float x, y, e;
	float delta_p = p2 - p1;
	float delta_q = q2 - q1;
	x = p1;
	y = q1;
	e = -delta_p;

	while (x <= p2) {
		if (e > 0) {
			if (NegX) {
				y--;
			} else {
				y++;
			}
			e = e - 2 * delta_p;
		}

		if (switch_print) {
			if (NegY) {
				putPixel((viewportWidth / 2) + y , (viewportHeight / 2) - x + 2*p1, color);
			} else {
				putPixel((viewportWidth / 2) + y, (viewportHeight / 2) + x,color);
			}
		} else {
			putPixel((viewportWidth / 2) + x, (viewportHeight / 2) + y, color);
		}
		x++;
		e = e + 2 * delta_q;
	}
}

void Renderer::RenderBoundingBox(Scene& scene, const ImGuiIO& io , int k, bool isCameraModel) {
	std::shared_ptr<Camera> active_camera = scene.GetCamera(scene.CurrCam);
	glm::mat4x4 Mc = glm::mat4x4(1);
	glm::mat4x4 Mp = glm::mat4x4(1);

	if (active_camera != NULL) {
		Mc = active_camera->Getview();
		Mp = active_camera->GetProjection();
	}
	std::shared_ptr<MeshModel> model = NULL;
	if (isCameraModel) {
		model = scene.GetCamera(k);
	}
	else {
		model = scene.GetModel(k);
	}
	
	float min_x = (model->BoundMin).x, min_y = (model->BoundMin).y, min_z = (model->BoundMin).z;
	float max_x = (model->BoundMax).x , max_y = (model->BoundMax).y, max_z = (model->BoundMax).z;

	glm::vec4 vec0(min_x, min_y, min_z, 1);
	glm::vec4 vec1(min_x, min_y, max_z, 1);
	glm::vec4 vec2(min_x, max_y, min_z, 1);
	glm::vec4 vec3(min_x, max_y, max_z, 1);
	glm::vec4 vec4(max_x, min_y, min_z, 1);
	glm::vec4 vec5(max_x, min_y, max_z, 1);
	glm::vec4 vec6(max_x, max_y, min_z, 1);
	glm::vec4 vec7(max_x, max_y, max_z, 1);

	glm::mat4x4 seriesTransform = Mp * Mc * model->GetWorldTransformation();
	
	glm::vec4 vect0 = seriesTransform * vec0;
	vect0 = vect0 / vect0.w;
	glm::vec4 vect1 = seriesTransform * vec1;
	vect1 = vect1 / vect1.w;
	glm::vec4 vect2 = seriesTransform * vec2;
	vect2 = vect2 / vect2.w;
	glm::vec4 vect3 = seriesTransform * vec3;
	vect3 = vect3 / vect3.w;
	glm::vec4 vect4 = seriesTransform * vec4;
	vect4 = vect4 / vect4.w;
	glm::vec4 vect5 = seriesTransform * vec5;
	vect5 = vect5 / vect5.w;
	glm::vec4 vect6 = seriesTransform * vec6;
	vect6 = vect6 / vect6.w;
	glm::vec4 vect7 = seriesTransform * vec7;
	vect7 = vect7 / vect7.w;

	DrawLine(vect0.x, vect1.x, vect0.y, vect1.y, model->BoundingBoxColor);
	DrawLine(vect0.x, vect2.x, vect0.y, vect2.y, model->BoundingBoxColor);
	DrawLine(vect0.x, vect4.x, vect0.y, vect4.y, model->BoundingBoxColor);

	DrawLine(vect3.x, vect2.x, vect3.y, vect2.y, model->BoundingBoxColor);
	DrawLine(vect3.x, vect1.x, vect3.y, vect1.y, model->BoundingBoxColor);
	DrawLine(vect3.x, vect7.x, vect3.y, vect7.y, model->BoundingBoxColor);

	DrawLine(vect6.x, vect7.x, vect6.y, vect7.y, model->BoundingBoxColor);
	DrawLine(vect6.x, vect2.x, vect6.y, vect2.y, model->BoundingBoxColor);
	DrawLine(vect6.x, vect4.x, vect6.y, vect4.y, model->BoundingBoxColor);

	DrawLine(vect5.x, vect7.x, vect5.y, vect7.y, model->BoundingBoxColor);
	DrawLine(vect5.x, vect1.x, vect5.y, vect1.y, model->BoundingBoxColor);
	DrawLine(vect5.x, vect4.x, vect5.y, vect4.y, model->BoundingBoxColor);
}

glm::vec3 Renderer::GetEstimatedFaceNormal(glm::vec3 basePoint, glm::vec3 vec0, glm::vec3 vec1, glm::vec3 vec2, float fNlength) {
	glm::vec3 u0 = vec1 - vec0;
	glm::vec3 u1 = vec2 - vec0;
	// return the normal as length of length
	glm::vec3 v = normalizeVector(basePoint, glm::cross(u0, u1), fNlength);
	return v;
}

float estAmbientColor(float K, float L) {
	return K * L;
}

float estDiffuseColor(float K, float L, glm::vec3 N, glm::vec3 S) {
	return K * L * glm::dot(N, S);
}

float estSpecularColor(float K, float L, glm::vec3 V, glm::vec3 N, glm::vec3 S, float alpha) {
	glm::vec3 R = 2.0f * N * glm::dot(N, S) - S;
	return K * L * pow(glm::dot(R, V), alpha);
}

glm::vec3& Renderer::estColor(float K, float L, glm::vec3& V, glm::vec3& N, glm::vec3& S, glm::vec3& colorA, glm::vec3& colorD, glm::vec3& colorS, int method, float alpha) {
	if (method == AMBIENT) {
		return colorA * estAmbientColor(K, L);
	}
	else if (method == DIFFUSE) {
		return colorD * estDiffuseColor(K, L, N, S);
	}
	else if (method == SPECULAR) {
		return colorS * estSpecularColor(K, L, V, N, S, alpha);
	}
	else if(method == PHONG_ILLUMINATION){
		return colorA * estAmbientColor(K, L) + colorD * estDiffuseColor(K, L, N, S) + colorS * estSpecularColor(K, L, V, N, S, alpha);
	}
}

std::vector<glm::vec3>* Renderer::estTriangle(Scene& scene,std::shared_ptr<MeshModel> model,glm::vec3& n0, glm::vec3& n1, glm::vec3& n2,int method) {
	glm::vec3* sourceLight = nullptr;
	if (scene.SizePoint > 0) {
		sourceLight = &scene.GetPointLight(scene.CurrPoint)->Center;
	}
	if(sourceLight == nullptr) sourceLight = &n0;
	glm::vec3 color0 = estColor(
		model->K,
		model->L,
		scene.GetCamera(scene.CurrCam)->origin_eye,
		n0,
		*sourceLight,
		model->lightColorA, model->lightColorD, model->lightColorA,
		method,
		model->alpha
	);
	if (sourceLight == nullptr) sourceLight = &n0;
	glm::vec3 color1 = estColor(
		model->K,
		model->L,
		scene.GetCamera(scene.CurrCam)->origin_eye,
		n1,
		*sourceLight,
		model->lightColorA, model->lightColorD, model->lightColorD,
		method,
		model->alpha
	);
	if (sourceLight == nullptr) sourceLight = &n0;
	glm::vec3 color2 = estColor(
		model->K,
		model->L,
		scene.GetCamera(scene.CurrCam)->origin_eye,
		n2,
		*sourceLight,
		model->lightColorA, model->lightColorD, model->lightColorS,
		method,
		model->alpha
	);
	std::vector<glm::vec3>* v = new std::vector<glm::vec3>;
	v->push_back(color0);
	v->push_back(color1);
	v->push_back(color2);
	return v;
}

void Renderer::showMeshObject(Scene& scene, std::vector<Face>::iterator face, std::vector<glm::vec3> vNormals, int k, const ImGuiIO& io, bool isCameraModel,bool isGrid, bool isPointLight) {
	std::shared_ptr<Camera> active_camera = scene.GetCamera(scene.CurrCam);
	glm::mat4x4 Mc = glm::mat4x4(1);
	glm::mat4x4 Mp = glm::mat4x4(1);
	if (active_camera != NULL) {
		Mc = active_camera->Getview();
		Mp = active_camera->GetProjection();
	}
	int v0 = face->GetVertexIndex(0) - 1;
	int v1 = face->GetVertexIndex(1) - 1;
	int v2 = face->GetVertexIndex(2) - 1;
	int v3;
	isGrid ? v3 = face->GetVertexIndex(3) - 1 : v3 = -1;
	// for instance: v0,v1,v2 => 1,13,4

	glm::vec3 modelVec;
	if (isCameraModel) { modelVec = scene.getCameraVertices(k, v0); }
	else if (isPointLight) { modelVec = scene.getLightPointVertices(k, v0); }
	else { modelVec = scene.getModelVertices(k, v0); }
	float x0 = modelVec.x;
	float y0 = modelVec.y;
	float z0 = modelVec.z;
	glm::vec4 vec0(x0, y0, z0, 1);
	// => (x0,y0,z0)

	if (isCameraModel) { modelVec = scene.getCameraVertices(k, v1); }
	else if (isPointLight) { modelVec = scene.getLightPointVertices(k, v1); }
	else { modelVec = scene.getModelVertices(k, v1); }
	float x1 = modelVec.x;
	float y1 = modelVec.y;
	float z1 = modelVec.z;
	glm::vec4 vec1(x1, y1, z1, 1);
	// => (x1,y1,z1)

	if (isCameraModel) { modelVec = scene.getCameraVertices(k, v2); } 
	else if (isPointLight) { modelVec = scene.getLightPointVertices(k, v2); }
	else { modelVec = scene.getModelVertices(k, v2); }
	float x2 = modelVec.x;
	float y2 = modelVec.y;
	float z2 = modelVec.z;
	glm::vec4 vec2(x2, y2, z2, 1);
	// => (x2,y2,z2)

	float x3 = 0.0f;
	float y3 = 0.0f;
	float z3 = 0.0f;
	if (isGrid) {
		x3 = scene.getModelVertices(k, v3).x;
		y3 = scene.getModelVertices(k, v3).y;
		z3 = scene.getModelVertices(k, v3).z;
	}
	glm::vec4 vec3(x3, y3, z3, 1);
	// => (x3,y3,z3)

	// transform face as world transform view:
	std::shared_ptr<MeshModel> model = NULL;
	if(isCameraModel){
		model = scene.GetCamera(k);
	}
	else if (isPointLight) { model = scene.GetPointLight(k); }
	else {
		model = scene.GetModel(k);
	}
	glm::mat4x4 seriesTransform = Mp * Mc * model->GetWorldTransformation();
	glm::vec4 vect0 = seriesTransform*vec0;
	vect0 = vect0 / vect0.w;
	glm::vec4 vect1 = seriesTransform*vec1;
	vect1 = vect1 / vect1.w;
	glm::vec4 vect2 = seriesTransform*vec2;
	vect2 = vect2 / vect2.w;
	glm::vec4 vect3 = seriesTransform * vec3;
	if (isGrid) {
		vect3 = vect3 / vect3.w;
	}

	/////////////////////////////////////////////////
	//	test section - line:
	//	=====================
	//	glm::vec4 c((model->BoundMiddle).x, (model->BoundMiddle).y, (model->BoundMiddle).z,1);
	//	c = seriesTransform * c;
	//	c = c / c.w;
	//	DrawLine(c.x, 300, c.y, 300, model->BoundingBoxColor);
	/////////////////////////////////////////////////

	float vNlength = model->GetVertexNormalLength();
	// transform and normalize vertex normals:
	glm::vec3 n0 = vNormals.at(0);
	glm::vec4 nt0 = seriesTransform*glm::vec4(n0.x,n0.y,n0.z,1);
	nt0 = nt0 / nt0.w;
	// return the normal as length of length
	nt0 = normalizeVector(vect0, nt0, vNlength);
	n0 = glm::vec3(nt0.x, nt0.y, nt0.z);

	glm::vec3 n1 = vNormals.at(1);
	glm::vec4 nt1 = seriesTransform*glm::vec4(n1.x, n1.y, n1.z, 1);
	nt1 = nt1 / nt1.w;
	nt1 = normalizeVector(vect1,nt1, vNlength);
	n1 = glm::vec3(nt1.x, nt1.y, nt1.z);
	
	glm::vec3 n2 = vNormals.at(2);
	glm::vec4 nt2 = seriesTransform*glm::vec4(n2.x, n2.y, n2.z, 1);
	nt2 = nt2 / nt2.w;
	nt2 = normalizeVector(vect2,nt2, vNlength);
	n2 = glm::vec3(nt2.x, nt2.y, nt2.z);

	glm::vec3 n3;
	if (isGrid) {
		n3 = vNormals.at(3);
		glm::vec4 nt3 = seriesTransform * glm::vec4(n3.x, n3.y, n3.z, 1);
		nt3 = normalizeVector(vect3, nt3, vNlength);
		n3 = glm::vec3(nt3.x, nt3.y, nt3.z);
	}

	// draw the object as triangles collection:
	if(isGrid){
		DrawLine(vect0.x, vect1.x, vect0.y, vect1.y, model->color);
		DrawLine(vect0.x, vect2.x, vect0.y, vect2.y, model->color);
		DrawLine(vect1.x, vect3.x, vect1.y, vect3.y, model->color);
		DrawLine(vect2.x, vect3.x, vect2.y, vect3.y, model->color);
	} else {
		if (!isPointLight) {
			std::vector<glm::vec3>* triangle_colors = estTriangle(scene, model, n0, n1, n2, model->lightType);
			glm::vec3 tri0 = triangle_colors->at(0);
			glm::vec3 tri1 = triangle_colors->at(1);
			glm::vec3 tri2 = triangle_colors->at(2);
			delete triangle_colors; // must be here!
			printTriangle(glm::vec2(vect0.x, vect0.y), glm::vec2(vect1.x, vect1.y), glm::vec2(vect2.x, vect2.y), tri0, tri1, tri2);
		} else {
			printTriangle(glm::vec2(vect0.x, vect0.y), glm::vec2(vect1.x, vect1.y), glm::vec2(vect2.x, vect2.y), model->color);
		}
	}

	// up to the checkbox sign:
	if (!isGrid) {
		if (model->GetFaceNormalView()) {
			float fVlength = model->GetFaceNormalLength();
			glm::vec3 basePoint((vect0.x + vect1.x + vect2.x) / 3, (vect0.y + vect1.y + vect2.y) / 3, (vect0.z + vect1.z + vect2.z) / 3);
			glm::vec3 estfNormal = GetEstimatedFaceNormal(basePoint, vect0, vect1, vect2, fVlength);
			DrawLine(basePoint.x, estfNormal.x, basePoint.y, estfNormal.y, model->GetFaceNormalColor());
		}
		if (model->GetVertexNormalView()) {
			glm::vec4 vertexColor = model->GetVertexNormalColor();
			DrawLine(vect0.x, n0.x, vect0.y, n0.y, vertexColor);
			DrawLine(vect1.x, n1.x, vect1.y, n1.y, vertexColor);
			DrawLine(vect2.x, n2.x, vect2.y, n2.y, vertexColor);
		}
	}
}

float Renderer::Distance(glm::vec2 v1, glm::vec2 v2) {
	return sqrt(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2));
}




void Renderer::drawParallelLight(glm::vec2 from, glm::vec2 to,glm::vec3 color) {
	///////////////////////////////////////////////////////////////////
	//TODO:
	//1. - shift to (0,0)
	//2. normalize
	//3. * size
	//4. + shift
	//////////////////////////////////////////////////////////////////

	glm::vec2 from_plus; 
	glm::vec2 from_minus; 
	float width_baseTriangle = 35;
	if ((from.y - to.y) == 0) {
		from_plus = glm::vec2(from.x, from.y + (width_baseTriangle/2));
		from_minus = glm::vec2(from.x, from.y - (width_baseTriangle / 2));
	}
	else if ((from.x - to.x) == 0) {
		from_plus = glm::vec2(from.x+ (width_baseTriangle / 2), from.y+1);
		from_minus = glm::vec2(from.x- (width_baseTriangle / 2), from.y);
	}
	else {
		float m1 = (from.y - to.y) / (from.x - to.x);
		float m2 = -1 / m1;
		float b2 = from.y - m2 * from.x;
		//Now we have: y = m2 * x + b2
		from_plus = glm::vec2(from.x + 1, m2*(from.x + 1) + b2);
		from_minus = glm::vec2(from.x - 1, m2*(from.x - 1) + b2);
		float d = Distance(from_plus, from_minus);
		float shift = width_baseTriangle / d;
		from_plus = glm::vec2(from.x + shift, m2*(from.x + shift) + b2);
		from_minus = glm::vec2(from.x - shift, m2*(from.x - shift) + b2);
	}
	printTriangle(from_minus,to, from_plus, color);
}

void Renderer::showAllMeshModels(Scene& scene, const ImGuiIO& io) {
	int modelsCount = scene.GetModelCount();
	if (scene.GetModelCount() > 0) {
		for (int k = 0; k < modelsCount; k++) {

			std::shared_ptr<MeshModel> model = scene.GetModel(k);
			std::vector<Face> faces = scene.getModelfaces(k);
			std::vector<glm::vec3> vNormals = scene.getModelNormals(k);
			for (auto face = faces.begin(); face != faces.end(); ++face) {
				if (model->GetModelName().compare("Grid") == 0) {
					showMeshObject(scene, face, vNormals, k, io,false,true);
				} else {
					showMeshObject(scene, face, vNormals, k, io);
					std::shared_ptr<MeshModel> model = scene.GetModel(k);
					if (model->showBoundingBox) {
						RenderBoundingBox(scene, io, k);
					}
				}
			}
		}
	}
	int camerasCount = scene.GetCameraCount();
	//Render All cameras in scene [*** Except the current camera ***]
	if (camerasCount > 0) {
		for (int k = 0; k < camerasCount; k++) {
			if (scene.CurrCam != k) {
				std::vector<Face> faces = scene.getCamerafaces(k);
				std::vector<glm::vec3> vNormals = scene.getCameraNormals(k);
				for (auto face = faces.begin(); face != faces.end(); ++face) {
					showMeshObject(scene, face, vNormals, k, io,true);
				}
			}
			
		}
	}

	int PointLightCount = scene.GetPointLightCount();
	if (PointLightCount > 0) {
		for (int k = 0; k < PointLightCount; k++) {
			std::vector<Face> faces = scene.getPointLightfaces(k);
			std::vector<glm::vec3> vNormals = scene.getPointLightNormals(k);
			for (auto face = faces.begin(); face != faces.end(); ++face) {
				showMeshObject(scene, face, vNormals, k, io, false,false,true);
			}
		}
	}

	int ParallelLightCount = scene.GetParallelLightCount();
	if (ParallelLightCount > 0) {
		for (int k = 0; k < ParallelLightCount; k++) {
			std::shared_ptr<ParallelLight> p = scene.GetParallelLight(k);
			glm::vec3 from3 = p->GetfromVector();
			glm::vec4 from4(from3.x, from3.y, from3.z, 1);
			glm::vec3 to3 = p->GetToVector();
			glm::vec4 to4(to3.x, to3.y, to3.z, 1);
			std::shared_ptr<Camera> active_camera = scene.GetCamera(scene.CurrCam);
			glm::mat4x4 Mc = glm::mat4x4(1);
			glm::mat4x4 Mp = glm::mat4x4(1);
			if (active_camera != NULL) {
				Mc = active_camera->Getview();
				Mp = active_camera->GetProjection();
			}
			glm::mat4x4 seriesTransform = Mp * Mc * p->GetWorldTransformation();
			from4 = seriesTransform * from4;
			from4 = from4 / from4.w;
			to4 = seriesTransform * to4;
			to4 = to4 / to4.w;
			glm::vec2 from2(from4.x, from4.y);
			glm::vec2 to2(to4.x, to4.y);
			drawParallelLight(from2, to2, glm::vec3(0, 0, 0));
		}
	}
	
	
}

/*
Solution steps:
================
1. Render All Models in scene
2. Each camera j  has:
	a. mc , mp -> we use them in case curr_camera == j
	b. Meshmodel -> we use it otherwise
3. Update - for each new transformation on the camera we update:
	a. mc , mp
	b. worldtransform
*/
void Renderer::Render(Scene& scene, const ImGuiIO& io)
{
	/*
	Get mouse position:
	//p2 = io.MousePos.x - (viewportWidth/2);
	//q2 = (viewportHeight/2) - io.MousePos.y;
	*/
	showAllMeshModels(scene, io);
}

//##############################
//##OpenGL stuff. Don't touch.##
//##############################

// Basic tutorial on how opengl works:
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
// don't linger here for now, we will have a few tutorials about opengl later.
void Renderer::initOpenGLRendering()
{
	// Creates a unique identifier for an opengl texture.
	glGenTextures(1, &glScreenTex);

	// Same for vertex array object (VAO). VAO is a set of buffers that describe a renderable object.
	glGenVertexArrays(1, &glScreenVtc);

	GLuint buffer;

	// Makes this VAO the current one.
	glBindVertexArray(glScreenVtc);

	// Creates a unique identifier for a buffer.
	glGenBuffers(1, &buffer);

	// (-1, 1)____(1, 1)
	//	     |\  |
	//	     | \ | <--- The exture is drawn over two triangles that stretch over the screen.
	//	     |__\|
	// (-1,-1)    (1,-1)
	const GLfloat vtc[]={
		-1, -1,
		 1, -1,
		-1,  1,
		-1,  1,
		 1, -1,
		 1,  1
	};

	const GLfloat tex[]={
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1};

	// Makes this buffer the current one.
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// This is the opengl way for doing malloc on the gpu. 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc)+sizeof(tex), NULL, GL_STATIC_DRAW);

	// memcopy vtc to buffer[0,sizeof(vtc)-1]
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);

	// memcopy tex to buffer[sizeof(vtc),sizeof(vtc)+sizeof(tex)]
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

	// Loads and compiles a sheder.
	GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );

	// Make this program the current one.
	glUseProgram(program);

	// Tells the shader where to look for the vertex position data, and the data dimensions.
	GLint  vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition,2,GL_FLOAT,GL_FALSE,0,0 );

	// Same for texture coordinates data.
	GLint  vTexCoord = glGetAttribLocation( program, "vTexCoord" );
	glEnableVertexAttribArray( vTexCoord );
	glVertexAttribPointer( vTexCoord,2,GL_FLOAT,GL_FALSE,0,(GLvoid *)sizeof(vtc) );

	//glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );

	// Tells the shader to use GL_TEXTURE0 as the texture id.
	glUniform1i(glGetUniformLocation(program, "texture"),0);
}

void Renderer::createOpenGLBuffer()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, glScreenTex);

	// malloc for a texture on the gpu.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, viewportWidth, viewportHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glViewport(0, 0, viewportWidth, viewportHeight);
}

void Renderer::SwapBuffers()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, glScreenTex);

	// memcopy's colorBuffer into the gpu.
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, viewportWidth, viewportHeight, GL_RGB, GL_FLOAT, colorBuffer);

	// Tells opengl to use mipmapping
	glGenerateMipmap(GL_TEXTURE_2D);

	// Make glScreenVtc current VAO
	glBindVertexArray(glScreenVtc);

	// Finally renders the data.
	glDrawArrays(GL_TRIANGLES, 0, 6);
}