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
#include "Utils.h"

using namespace std;

#define INDEXCOLOR(width,x,y,c) ((x)+(y)*(width))*3+(c)
#define INDEXZ(width,x,y) ((x)+(y)*(width))
#define MaxDepth 4000.0f
#define LIGHT_THRESH 0.7f

void Renderer::LoadShaders(){
	
    colorShader.loadShaders("C:\\Users\\user\\Documents\\GitHub\\project-e-i-team\\Viewer\\shaders\\vshader_color.glsl",
                            "C:\\Users\\user\\Documents\\GitHub\\project-e-i-team\\Viewer\\shaders\\fshader_color.glsl");
                            //("vshader_color.glsl", "fshader_color.glsl");
}

void Renderer::LoadTextures(){
    if (!texture1.loadTexture("bin\\Debug\\crate.jpg", true))
    {
        texture1.loadTexture("bin\\Release\\crate.jpg", true);
    }
}

Renderer::Renderer(){}

Renderer::~Renderer() {}

void Renderer::showAllMeshModels(Scene& scene, const ImGuiIO& io) {
	int modelsCount = scene.GetModelCount();
	if (scene.GetModelCount() > 0) {
		for (int k = 0; k < modelsCount; k++) {
			std::shared_ptr<MeshModel> model = scene.GetModel(k);
            // Activate the 'colorShader' program (vertex and fragment shaders)
            colorShader.use();
            // Set the uniform variables
            colorShader.setUniform("model", model->GetWorldTransformation());
            colorShader.setUniform("view", scene.GetCamera(scene.CurrCam)->Getview());
            colorShader.setUniform("projection", scene.GetCamera(scene.CurrCam)->GetProjection());
            colorShader.setUniform("material.textureMap", 0);
            colorShader.setUniform("material.color", model->color);

            // Set 'texture1' as the active texture at slot #0
            texture1.bind(0);
            
            // Drag our model's faces (triangles) in fill mode
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glBindVertexArray(1);
            glDrawArrays(GL_TRIANGLES, 0, model->GetModelVertices().size());
            glBindVertexArray(0);

            // Unset 'texture1' as the active texture at slot #0
            texture1.unbind(0);

            // Drag our model's faces (triangles) in line mode (wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glBindVertexArray(model->GetVAO());
			std::cout << model->GetModelVertices().size() << std::endl;
			cout << "---------------------" << endl;
            glDrawArrays(GL_TRIANGLES, 0, model->GetModelVertices().size());
            glBindVertexArray(0);
		}
	}
	//int camerasCount = scene.GetCameraCount();
	//for (int k = 0; k < camerasCount; k++) {
	//	if (scene.CurrCam != k) {
 //           // Activate the 'colorShader' program (vertex and fragment shaders)
 //           colorShader.use();
 //           std::shared_ptr<Camera> cam = scene.GetCamera(k);

 //           // Set the uniform variables
 //           colorShader.setUniform("model", cam->GetWorldTransformation());
 //           colorShader.setUniform("view", scene.GetCamera(scene.CurrCam)->Getview());
 //           colorShader.setUniform("projection", scene.GetCamera(scene.CurrCam)->GetProjection());
 //           colorShader.setUniform("material.textureMap", 0);
 //           colorShader.setUniform("material.color", cam->color);

 //           // Set 'texture1' as the active texture at slot #0
 //           texture1.bind(0);

 //           // Drag our model's faces (triangles) in fill mode
 //           glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
 //           glBindVertexArray(cam->GetVAO());
 //           glDrawArrays(GL_TRIANGLES, 0, cam->GetModelVertices().size());
 //           glBindVertexArray(0);

 //           // Unset 'texture1' as the active texture at slot #0
 //           texture1.unbind(0);

 //           // Drag our model's faces (triangles) in line mode (wireframe)
 //           glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
 //           glBindVertexArray(cam->GetVAO());
 //           glDrawArrays(GL_TRIANGLES, 0, cam->GetModelVertices().size());
 //           glBindVertexArray(0);
	//	}
	//}

	//int PointLightCount = scene.GetPointLightCount();
	//if (PointLightCount > 0) {
	//	for (int k = 0; k < PointLightCount; k++) {
 //           // Activate the 'colorShader' program (vertex and fragment shaders)
 //           colorShader.use();
 //           std::shared_ptr<PointLight> pointLight = scene.GetPointLight(k);
 //           // Set the uniform variables
 //           colorShader.setUniform("model", pointLight->GetWorldTransformation());
 //           colorShader.setUniform("view", scene.GetCamera(scene.CurrCam)->Getview());
 //           colorShader.setUniform("projection", scene.GetCamera(scene.CurrCam)->GetProjection());
 //           colorShader.setUniform("material.textureMap", 0);
 //           colorShader.setUniform("material.color", pointLight->color);

 //           // Set 'texture1' as the active texture at slot #0
 //           texture1.bind(0);

 //           // Drag our model's faces (triangles) in fill mode
 //           glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
 //           glBindVertexArray(pointLight->GetVAO());
 //           glDrawArrays(GL_TRIANGLES, 0, pointLight->GetModelVertices().size());
 //           glBindVertexArray(0);

 //           // Unset 'texture1' as the active texture at slot #0
 //           texture1.unbind(0);

 //           // Drag our model's faces (triangles) in line mode (wireframe)
 //           glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
 //           glBindVertexArray(pointLight->GetVAO());
 //           glDrawArrays(GL_TRIANGLES, 0, pointLight->GetModelVertices().size());
 //           glBindVertexArray(0);
	//	}
	//}

	//Draw All Parallel Lights
    /*
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
			drawParallelLight(scene,from4, to4, p->color);
		}
	}
	

	//Draw Ambient Light
	std::shared_ptr<AmbientLight> Ambient = scene.GetAmbient();
	glm::vec3 AmbientBasePoint3 = Ambient->GetBaseVector();
	glm::vec4 AmbientBasePoint4(AmbientBasePoint3.x, AmbientBasePoint3.y, AmbientBasePoint3.z, 1);
	
	std::shared_ptr<Camera> active_camera = scene.GetCamera(scene.CurrCam);
	glm::mat4x4 Mc = glm::mat4x4(1);
	glm::mat4x4 Mp = glm::mat4x4(1);
	if (active_camera != NULL) {
		Mc = active_camera->Getview();
		Mp = active_camera->GetProjection();
	}
	glm::mat4x4 seriesTransform = Mp * Mc * Ambient->GetWorldTransformation();
	AmbientBasePoint4 = seriesTransform * AmbientBasePoint4;
	AmbientBasePoint4 = AmbientBasePoint4 / AmbientBasePoint4.w;
		
	drawAmbientLight(scene,AmbientBasePoint4, Ambient->color);
    */
}

void Renderer::Render(Scene& scene, const ImGuiIO& io)
{	
	showAllMeshModels(scene, io);
}

//##############################
//##OpenGL stuff. Don't touch.##
//##############################
// Basic tutorial on how opengl works:
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
// don't linger here for now, we will have a few tutorials about opengl later.