#include "Renderer.h"
#include "MeshModel.h"
#include "ShaderProgram.h"
#include <imgui/imgui.h>
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include "Utils.h"


Renderer::Renderer() {}

Renderer::~Renderer() {}

void Renderer::Render(const std::shared_ptr<Scene>& scene)
{
    int lightsCount = scene->GetPointLightCount();

    glm::vec4 lightColors[2];
    lightColors[0] = glm::vec4(0, 0, 0, 0);
    lightColors[1] = glm::vec4(0, 0, 0, 0);

    glm::vec4 lightPos[2];
    lightPos[0] = glm::vec4(0, 0, 0, 0);
    lightPos[1] = glm::vec4(0, 0, 0, 0);

    // prepare lights:
    for (int i = 0; i < lightsCount; ++i) {
        if (i > 1) { break; } // 2 point lights are allowed and being accepted with the shader parameters
        lightColors[i] = glm::vec4(scene->GetPointLight(i)->GetColor(),1.0f);
        lightPos[i] = glm::vec4(scene->GetPointLight(i)->GetPositionAfterTrans(),1.0f);
    }
    // Activate the 'colorShader' program (vertex and fragment shaders)
    colorShader.use();
    // Set the uniform variables
    Camera camera = scene->GetActiveCamera();
    colorShader.setUniform("view", camera.GetViewTransformation());
    colorShader.setUniform("projection", camera.GetProjectionTransformation());
    colorShader.setUniform("material.textureMap", 0);
    colorShader.setUniform("ambientColorScene", glm::vec4(scene->GetAmbientLight().GetColor(),1.0f));
    colorShader.setUniform("lightColor1", glm::vec4(glm::vec3(lightColors[0]),1.0f));
    colorShader.setUniform("lightColor2", glm::vec4(glm::vec3(lightColors[1]),1.0f));
    colorShader.setUniform("lightPos1", lightPos[0]);
    colorShader.setUniform("lightPos2", lightPos[1]);

	int cameraCount = scene->GetCameraCount();
	if (cameraCount > 0) {
		const Camera& camera = scene->GetActiveCamera();
		for (int currentModelIndex = 0; currentModelIndex < scene->GetModelCount(); currentModelIndex++) {
			std::shared_ptr<MeshModel> currentModel = scene->GetModel(currentModelIndex);

            // Activate the 'colorShader' program (vertex and fragment shaders)
            colorShader.use();
            colorShader.setUniform("model", currentModel->GetWorldTransformation() * currentModel->GetModelTransformation());
            colorShader.setUniform("material.modelColor", glm::vec4(currentModel->GetColor(),1.0f));
            colorShader.setUniform("material.AmbientColor", glm::vec4(scene->GetAmbientLight().GetColor(),1.0f));
            colorShader.setUniform("material.DiffuseColor", glm::vec4(currentModel->GetDiffuseColor(),1.0f));
            colorShader.setUniform("material.SpecualrColor", glm::vec4(currentModel->GetSpecuralColor(),1.0f));
            colorShader.setUniform("material.Ka", currentModel->GetKa());
            colorShader.setUniform("material.Kd", currentModel->GetKd());
            colorShader.setUniform("material.Ks", currentModel->GetKs());
            colorShader.setUniform("material.Ksh", currentModel->GetExponent());
            colorShader.setUniform("needTexture", currentModel->IsTextureNeeded());
            colorShader.setUniform("needToon", currentModel->isToonShadingMode());
            colorShader.setUniform("needNoiseBump", currentModel->isNoisyMode());
            colorShader.setUniform("numColors", currentModel->GetToonShadingRange()); // toon-shading manner

            if(!currentModel->IsWireOnlyMode()) {
                texture1.bind(0);
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                glBindVertexArray(currentModel->GetVAO());
                glDrawArrays(GL_TRIANGLES, 0, currentModel->GetModelVertices().size());
                glBindVertexArray(0);
                texture1.unbind(0);
            }

            colorShader.setUniform("material.AmbientColor", currentModel->GetAmbientColorLines());
            colorShader.setUniform("material.DiffuseColor", currentModel->GetDiffuseColorLines());
            colorShader.setUniform("material.SpecualrColor", currentModel->GetSpecularColorLines());

            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glBindVertexArray(currentModel->GetVAO());
            glDrawArrays(GL_TRIANGLES, 0, currentModel->GetModelVertices().size());
            glBindVertexArray(0);
		}
	
		int PointLightCount = scene->GetPointLightCount();
		for (int currentMPointIndex = 0; currentMPointIndex < PointLightCount; currentMPointIndex++) {
            std::shared_ptr<PointLight> currentPoint = scene->GetPointLight(currentMPointIndex);

            colorShader.use();
            colorShader.setUniform("model", currentPoint->GetWorldTransformation() * currentPoint->GetModelTransformation());
            colorShader.setUniform("material.modelColor", glm::vec4(currentPoint->GetColor(), 1.0f));
            colorShader.setUniform("material.AmbientColor", glm::vec4(currentPoint->GetColor(), 1.0f));
            colorShader.setUniform("material.DiffuseColor", glm::vec4(currentPoint->GetColor(), 1.0f));
            colorShader.setUniform("material.SpecualrColor", glm::vec4(currentPoint->GetColor(), 1.0f));
            colorShader.setUniform("material.Ka", 1);
            colorShader.setUniform("material.Kd", 1);
            colorShader.setUniform("material.Ks", 1);
            colorShader.setUniform("material.Ksh", 1);
            colorShader.setUniform("needTexture", false);
            colorShader.setUniform("needToon", false);
            colorShader.setUniform("numColors", 0); // toon shading manner

			
            // Activate the 'colorShader' program (vertex and fragment shaders)
            // colorShader.use();

            colorShader.setUniform("model", currentPoint->GetWorldTransformation() * currentPoint->GetModelTransformation());
            colorShader.setUniform("color", glm::vec4(currentPoint->GetColor(),1.0f));

			// Drag our model's faces (triangles) in fill mode
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glBindVertexArray(1);
			glDrawArrays(GL_TRIANGLES, 0, scene->GetModel(0)->GetModelVertices().size());
			glBindVertexArray(0);
		}
	}
}

void Renderer::LoadShaders() {

	colorShader.loadShaders("vshader_color.glsl", "fshader_color.glsl");
}

void Renderer::LoadTextures() {
	if (!texture1.loadTexture("bin\\Debug\\crate.jpg", true)) 
    {
		texture1.loadTexture("bin\\Release\\crate.jpg", true);
	}
}