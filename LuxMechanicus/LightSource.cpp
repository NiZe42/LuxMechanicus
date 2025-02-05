#include "LightSource.h"

LightSource::LightSource(glm::vec3 position,
	glm::vec3 rotation,
	glm::vec3 scale,
	const char* vertexShaderPath,
	const char* fragmentShaderPath)
    : GameObject(position,
		rotation,
		scale,
		vertexShaderPath,
		fragmentShaderPath), 
	pColor(glm::vec3(1.0f)),
	pIntensity(1.0f) {

}

glm::vec3 LightSource::GetColor() const {
    return pColor;
}

void LightSource::SetColor(glm::vec3 color){
	pColor = color;
}

float LightSource::GetIntensity()const {
	return pIntensity;
}

void LightSource::SetIntensity(float intensity) {
	pIntensity = intensity;
}

void LightSource::Render(glm::mat4 viewMatrix, glm::mat4 projectMatrix) {

	pShader->SetUniformVector("lightColor", GetColor());

	GameObject::Render(viewMatrix, projectMatrix);
}