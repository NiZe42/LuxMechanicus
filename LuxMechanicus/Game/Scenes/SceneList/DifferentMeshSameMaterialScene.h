#pragma once
#include "..\Scene.h"
#include "..\..\..\Utils\Environment.h"
#include "..\..\..\Renderer\Mesh\MeshCache.h"
#include "../../../Renderer/Lighting/LightSource.h"
#include "..\..\Camera\Camera.h"

class DifferentMeshSameMaterialScene : public Scene
{

public:

	void Initialize() override {
		/*
		glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  -3.2f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
		};
		*/
		srand(1);

		for (int i = 0; i < 500; i++)
		{

			std::string texturePath = "/Textures/bricks.jpg";
			std::string meshPath = "/Models/teapot_flat.obj";
			int randomIndex = rand() % 5;

			switch (randomIndex) {
			case 0: 
				//texturePath = "/Textures/land.jpg";
				meshPath = "/Models/cone_smooth.obj";
				break;
			case 1:
				//texturePath = "/Textures/water.jpg";
				meshPath = "/Models/cube_smooth.obj";
				break;
			case 2: 
				//texturePath = "/Textures/MinecraftDirtTexture.jpg";
				meshPath = "/Models/cylinder_smooth.obj";
				break;
			case 3: 
				//texturePath = "/Textures/diffuse1.jpg";
				meshPath = "/Models/suzanna_flat.obj";
				break;
			default:
				break;
			}

			GameObject* square = new GameObject(
				glm::vec3((rand() % 200 - 100) * 0.1f,
					(rand() % 200 - 100) * 0.1f,
					(rand() % 200 - 100) * 0.1f),
				glm::vec3(rand() % 360, rand() % 360, rand() % 360),
				glm::vec3(0.7f),
				(std::string(Environment::GetRootPath()) + "/Shaders/ColorVert.glsl").c_str(),
				(std::string(Environment::GetRootPath()) + "/Shaders/ColorFrag.glsl").c_str(),
				(std::string(Environment::GetRootPath()) + texturePath).c_str());

			Mesh* mesh;

			//if (randomIndex == 0)
			mesh = MeshCache::GetMesh((std::string(Environment::GetRootPath()) + meshPath).c_str());
			//else
			//	mesh = MeshCache::GetMesh((std::string(Environment::GetRootPath()) + "/Models/suzanna_flat.obj").c_str());

			square->SetMesh(mesh);

			AddChild(square);

			std::cout << "Creating obj " << square->GetGameObjectId() << " at: " << square->GetPosition().x << ", " << square->GetPosition().y << ", " << square->GetPosition().z << std::endl;
		}

		GameObject* plane = new GameObject(
			glm::vec3(0, -4, 0),
			glm::vec3(0, 0, 0),
			glm::vec3(10, 10, 10),
			(std::string(Environment::GetRootPath()) + "/Shaders/ColorVert.glsl").c_str(),
			(std::string(Environment::GetRootPath()) + "/Shaders/ColorFrag.glsl").c_str());
		//(std::string(Environment::GetRootPath()) + "/Textures/bricks.jpg").c_str());

		Mesh* planeMesh;
		planeMesh = MeshCache::GetMesh((std::string(Environment::GetRootPath()) + "/Models/plane.obj").c_str());

		plane->SetMesh(planeMesh);
		AddChild(plane);

		for (size_t i = 0; i < GetChildren().size(); i++)
		{
			GameObject* gameobject = dynamic_cast<GameObject*>(GetChildren()[i]);
			std::cout << gameobject->GetGameObjectId() << std::endl;
		}

		Camera* camera = new Camera(2.0f, 0.1f);
		camera->SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));
		activeCamera = camera;

		for (int i = 0; i < 2; i++) {
			LightSource* lightSource1 = new LightSource(
				glm::vec3(1.0f, 2.0f + i, 1.0f),
				glm::vec3(0.0f),
				glm::vec3(0.5f),
				(std::string(Environment::GetRootPath()) + "/Shaders/LightSourceVert.glsl").c_str(),
				(std::string(Environment::GetRootPath()) + "/Shaders/LightSourceFrag.glsl").c_str());

			lightSource1->SetDefaultValues(LightType::DIRECTIONAL);
			glm::vec3 direction = glm::normalize(glm::vec3(
				cos(i * 0.4f),
				-1.0f,
				sin(i * 0.4f)
			));
			lightSource1->SetDirection(direction);

			float hue = i / 16.0f; // [0, 1]
			glm::vec3 color = glm::vec3(
				0.5f + 0.5f * sin(6.2831f * hue),
				0.5f + 0.5f * sin(6.2831f * hue + 2.094f),  // + 120
				0.5f + 0.5f * sin(6.2831f * hue + 4.188f)   // + 240
			);
			lightSource1->SetColor(color);

			lightSource1->SetIntensity(0.5f);

			Mesh* lightSourceMesh1 = MeshCache::GetMesh((std::string(Environment::GetRootPath()) + "/Models/cube_smooth.obj").c_str());
			lightSource1->SetMesh(lightSourceMesh1);
			AddChild(lightSource1);
			lightSources.push_back(lightSource1);
		}

		/*LightSource* lightSource2 = new LightSource(
			glm::vec3(-3.0f, 2.0f, 2.0f),
			glm::vec3(0.0f),
			glm::vec3(0.5f),
			(std::string(Environment::GetRootPath()) + "/Shaders/LightSourceVert.glsl").c_str(),
			(std::string(Environment::GetRootPath()) + "/Shaders/LightSourceFrag.glsl").c_str());

		lightSource2->SetDefaultValues(LightType::POINT);
		lightSource2->SetColor(glm::vec3(0.0f, 0.0f, 1.0f));

		Mesh* lightSourceMesh2 = MeshCache::GetMesh((std::string(Environment::GetRootPath()) + "/Models/cube_smooth.obj").c_str());
		lightSource2->SetMesh(lightSourceMesh2);
		mainScene->AddChild(lightSource2);
		renderProcessor->AddLight(lightSource2);*/

	}

private:
};