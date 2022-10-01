#include "pch.h"
#include "main.h"

#include "ComponentManager.h"
#include "GraphicSystem.h"
#include "InputSystem.h"
#include "WindowSystem.h"

#include "Button.h"
#include "Camera.h"
#include "Color.h"
#include "ModelComponent.h"
#include "Transform.h"

namespace OpenGLFun {
	GraphicSystem* GRAPHICS_SYSTEM = nullptr;;

	GraphicSystem::GraphicSystem() : ISystem(), _mainShaderProgram() {
		if (GRAPHICS_SYSTEM != nullptr)
			throw SimpleException("Graphics system already created!");

		Shader vertexShader{}, fragmentShader{};
		if (!vertexShader.Compile(ShaderType::Vertex, "assets/shaders/basic.vert")) {
			vertexShader.Destroy();

			throw SimpleException("Failed to compile basic.vert.\n");
		}
		if (!fragmentShader.Compile(ShaderType::Fragment, "assets/shaders/basic.frag")) {
			fragmentShader.Destroy();

			throw SimpleException("Failed to compile basic.frag.\n");
		}

		if (!_mainShaderProgram.CompileAndLink(vertexShader, fragmentShader)) {
			_mainShaderProgram.Destroy();
			throw SimpleException("Failed to compile main shader program.\n");
		}

		if (!vertexShader.Compile(ShaderType::Vertex, "assets/shaders/2d.vert")) {
			vertexShader.Destroy();

			throw SimpleException("Failed to compile 2d.vert.\n");
		}
		if (!fragmentShader.Compile(ShaderType::Fragment, "assets/shaders/basic.frag")) {
			fragmentShader.Destroy();

			throw SimpleException("Failed to compile basic.frag.\n");
		}

		if (!_2DShaderProgram.CompileAndLink(vertexShader, fragmentShader)) {
			_2DShaderProgram.Destroy();
			throw SimpleException("Failed to compile 2D shader program.\n");
		}

		Vertex vertex;
		std::vector<Vertex> vertices = {
			// bottom face (-ve y), blue
			vertex.Pos(-0.5f, -0.5f,  0.5f).Color(0.0f, 0.0f, 1.0f),  // front left
			vertex.Pos(-0.5f, -0.5f, -0.5f),  // back left
			vertex.Pos(0.5f, -0.5f, -0.5f),  // back right
			vertex.Pos(0.5f, -0.5f, -0.5f),  // back right
			vertex.Pos(0.5f, -0.5f,  0.5f),  // front right
			vertex.Pos(-0.5f, -0.5f,  0.5f),  // front left

			// top face (-ve y), cyan
			vertex.Pos(-0.5f, 0.5f, -0.5f).Color(0.0f, 1.0f, 1.0f),  // top left
			vertex.Pos(-0.5f, 0.5f,  0.5f),  // bottom left
			vertex.Pos(0.5f, 0.5f, -0.5f),  // top right
			vertex.Pos(0.5f, 0.5f, -0.5f),  // top right
			vertex.Pos(-0.5f, 0.5f,  0.5f),  // bottom left
			vertex.Pos(0.5f, 0.5f,  0.5f),  // bottom right

			// front face (+ve z), green
			vertex.Pos(-0.5f,  0.5f, 0.5f).Color(0.0f, 1.0f, 0.0f),  // top left
			vertex.Pos(-0.5f, -0.5f, 0.5f),  // bottom left
			vertex.Pos(0.5f,  0.5f, 0.5f),  // top right
			vertex.Pos(0.5f,  0.5f, 0.5f),  // top right
			vertex.Pos(-0.5f, -0.5f, 0.5f),  // bottom left
			vertex.Pos(0.5f, -0.5f, 0.5f),  // bottom right

			// back face (-ve z), yellow
			vertex.Pos(0.5f,  0.5f, -0.5f).Color(1.0f, 1.0f, 0.0f),  // top left
			vertex.Pos(0.5f, -0.5f, -0.5f),  // bottom left
			vertex.Pos(-0.5f,  0.5f, -0.5f),  // top right
			vertex.Pos(-0.5f,  0.5f, -0.5f),  // top right
			vertex.Pos(0.5f, -0.5f, -0.5f),  // bottom left
			vertex.Pos(-0.5f, -0.5f, -0.5f),  // bottom right

			// left face (-ve x), purple
			vertex.Pos(-0.5f,  0.5f, -0.5f).Color(1.0f, 0.0f, 1.0f),  // top left
			vertex.Pos(-0.5f, -0.5f, -0.5f),  // bottom left
			vertex.Pos(-0.5f,  0.5f,  0.5f),  // top right
			vertex.Pos(-0.5f,  0.5f,  0.5f),  // top right
			vertex.Pos(-0.5f, -0.5f, -0.5f),  // bottom left
			vertex.Pos(-0.5f, -0.5f,  0.5f),  // bottom right

			// right face (+ve x), red
			vertex.Pos(0.5f,  0.5f,  0.5f).Color(1.0f, 0.0f, 0.0f),  // top left
			vertex.Pos(0.5f, -0.5f,  0.5f),  // bottom left
			vertex.Pos(0.5f,  0.5f, -0.5f),  // top right
			vertex.Pos(0.5f,  0.5f, -0.5f),  // top right
			vertex.Pos(0.5f, -0.5f,  0.5f),  // bottom left
			vertex.Pos(0.5f, -0.5f, -0.5f)   // bottom right
		};
		// left face (-ve x), purple
		//quad.init(vertex.Pos(-0.5f, 0.5f, -0.5f).Color(1.0f, 0.0f, 1.0f), vertex.Pos(-0.5f, -0.5f, -0.5f).Color(1.0f, 0.0f, 1.0f), vertex.Pos(-0.5f, -0.5f, 0.5f).Color(1.0f, 0.0f, 1.0f), vertex.Pos(-0.5f, 0.5f, 0.5f).Color(1.0f, 0.0f, 1.0f));
		//vertices.insert(vertices.end(), quad.Vertices().begin(), quad.Vertices().end());
		//// right face (+ve x), red. Top-left, anti-clockwise
		//quad.init(vertex.Pos(0.5f, 0.5f, 0.5f).Color(1.0f, 0.0f, 0.0f), vertex.Pos(0.5f, -0.5f, 0.5f).Color(1.0f, 0.0f, 0.0f), vertex.Pos(0.5f, -0.5f, -0.5f).Color(1.0f, 0.0f, 0.0f), vertex.Pos(0.5f, 0.5f, -0.5f).Color(1.0f, 0.0f, 0.0f));
		//vertices.insert(vertices.end(), quad.Vertices().begin(), quad.Vertices().end());
		_rainbowCubeModel.Init(vertices);

		vertices.clear();
		vertices.insert(vertices.end(), {
			// x-axis, red
			vertex.Pos(0.0f, 0.0f, 0.0f).Color(1.0f, 0.0f, 0.0f),
			vertex.Pos(0.5f, 0.0f, 0.0f),

			// z-axis, blue
			vertex.Pos(0.0f, 0.0f, 0.0f).Color(0.0f, 0.0f, 1.0f),
			vertex.Pos(0.0f, 0.0f, 0.5f),

			// y-axis, green
			vertex.Pos(0.0f, 0.0f, 0.0f).Color(0.0f, 1.0f, 0.0f),
			vertex.Pos(0.0f, 0.5f, 0.0f)
			});
		_axisModel.Init(vertices).SetDrawMode(GL_LINE_STRIP).SetCull(false);

		vertices = {
			vertex.Pos(-0.5f, 0.5f, 0.0f).Color(1.0f, 1.0f, 1.0f), // top left
			vertex.Pos(-0.5f, -0.5f, 0.0f), // bottom left
			vertex.Pos(0.5f, -0.5f, 0.0f), // bottom right

			vertex.Pos(0.5f, -0.5f, 0.0f), // bottom right
			vertex.Pos(0.5f, 0.5f, 0.0f), // top right
			vertex.Pos(-0.5f, 0.5f, 0.0f), // top left
		};
		_2DShapeModel.Init(vertices).SetCull(false).SetBlend(true);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // enable wireframe mode
		glEnable(GL_DEPTH_TEST);
	}

	GraphicSystem::~GraphicSystem() {
		_2DShapeModel.Destroy();
		_axisModel.Destroy();
		_mainShaderProgram.Destroy();
	}

	void GraphicSystem::Update(float const&) {
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		_mainShaderProgram.use();

		Camera* playerCamera = COMPONENT_MANAGER->GetComponent<Camera>(engine->mPlayerId, ComponentType::Camera);
		Transform* playerPos = COMPONENT_MANAGER->GetComponent<Transform>(engine->mPlayerId, ComponentType::Transform);
		// camera pos, target pos, up direction1
		Vec3f lookAtLerp = playerCamera->mLookAt;
		Vec3f eye = playerPos->mPosition + playerCamera->mCamOffset;
		Vec3f center = playerPos->mPosition + playerCamera->mCamOffset + lookAtLerp;
		glm::mat4 view = glm::lookAt(glm::vec3(eye.x, eye.y, eye.z), glm::vec3(center.x, center.y, center.z), glm::vec3(playerCamera->mCamUp.x, playerCamera->mCamUp.y, playerCamera->mCamUp.z));
		glm::mat4 model;
		glm::mat4 proj = glm::mat4(1.0f);

		// fov, width/height ratio, near, far
		proj = glm::perspective(glm::radians(45.0f), static_cast<float>(WINDOW_SYSTEM->GetWindowWidth()) / WINDOW_SYSTEM->GetWindowHeight(), 0.1f, 50.0f);

		for (EntityId const& entityId : ENTITY_MANAGER->GetEntities()) {
			if (!COMPONENT_MANAGER->HasComponent(entityId, ComponentType::Transform) || !COMPONENT_MANAGER->HasComponent(entityId, ComponentType::Model))
				continue;

			model = glm::mat4(1.0f);
			ModelComponent* modelComp = COMPONENT_MANAGER->GetComponent<ModelComponent>(entityId, ComponentType::Model);
			Transform* entityTransform = COMPONENT_MANAGER->GetComponent<Transform>(entityId, ComponentType::Transform);

			if (modelComp->mModelType == ModelComponent::Type::Axis) {
				model = glm::translate(glm::mat4(1.0f), vec3f_to_vec3(entityTransform->mPosition));
				_axisModel.Draw3D(_mainShaderProgram.mProgramId, model, view, proj, texture->mGLTextureId);
			}

			if (modelComp->mModelType == ModelComponent::Type::Cube) {
				// SRT
				// rotation is zyx. That is when we apply in math, it's reversed order
				model = glm::translate(glm::mat4(1.0f), vec3f_to_vec3(entityTransform->mPosition));
				model = glm::scale(model, vec3f_to_vec3(entityTransform->mScale));

				/*glm::mat4 mtxRotX = {
					1, 0, 0, 0,
					0, glm::cos(glm::radians(45.0f)), -glm::sin(glm::radians(45.0f)), 0,
					0, glm::sin(glm::radians(45.0f)), glm::cos(glm::radians(45.0f)), 0,
					0, 0, 0, 1
				};
				glm::mat4 mtxRotY = {
					glm::cos(glm::radians(45.0f)), 0, -glm::sin(glm::radians(45.0f)), 0,
					0, 1, 0, 0,
					glm::sin(glm::radians(45.0f)), 0, glm::cos(glm::radians(45.0f)), 0,
					0, 0, 0, 1
				};
				glm::mat4 mtxRotZ = {
					glm::cos(glm::radians(45.0f)), -glm::sin(glm::radians(45.0f)), 0, 0,
					glm::sin(glm::radians(45.0f)), glm::cos(glm::radians(45.0f)), 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1
				};
				model = glm::transpose(mtxRotZ * mtxRotY * mtxRotX) * model;
				std::cout << "I believe this is identity:" << glm::to_string(glm::mat4(1.0f)) << '\n';
				std::cout << glm::to_string(glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f))) << '\n';*/
				/*model = glm::rotate(model, glm::radians(entityTransform->mRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::rotate(model, glm::radians(entityTransform->mRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, glm::radians(entityTransform->mRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));*/
				_rainbowCubeModel.Draw3D(_mainShaderProgram.mProgramId, model, view, proj, texture->mGLTextureId);
			}
		}

		if (INPUT_SYSTEM->mIsPaused) {
			glm::vec3 sample_vec3;
			_2DShaderProgram.use();

			//glDisable(GL_DEPTH);
			for (EntityId const& entityId : ENTITY_MANAGER->GetEntities()) {
				if (!COMPONENT_MANAGER->HasComponent(entityId, ComponentType::Transform) || !COMPONENT_MANAGER->HasComponent(entityId, ComponentType::Model) || !COMPONENT_MANAGER->HasComponent(entityId, ComponentType::Color))
					continue;

				ModelComponent* modelComp = COMPONENT_MANAGER->GetComponent<ModelComponent>(entityId, ComponentType::Model);
				if (modelComp->mModelType != ModelComponent::Type::TwoD) continue;
				Transform* entityTransform = COMPONENT_MANAGER->GetComponent<Transform>(entityId, ComponentType::Transform);

				sample_vec3 = vec3f_to_vec3(entityTransform->mPosition);
				sample_vec3[0] /= WINDOW_SYSTEM->GetWindowWidth();
				sample_vec3[1] /= WINDOW_SYSTEM->GetWindowHeight();
				model = glm::translate(glm::mat4(1.0f), sample_vec3);

				sample_vec3 = vec3f_to_vec3(entityTransform->mScale);
				sample_vec3[0] /= WINDOW_SYSTEM->GetWindowWidth();
				sample_vec3[1] /= WINDOW_SYSTEM->GetWindowHeight();
				sample_vec3[2] = 0.0f;
				sample_vec3 *= 2.0f;
				model = glm::scale(model, sample_vec3);

				auto color = COMPONENT_MANAGER->GetComponent<Color>(entityId, ComponentType::Color)->mRgba;

				if (COMPONENT_MANAGER->HasComponent(entityId, ComponentType::Button)) {
					// need to localise the mouse pos origin (top left) to opengl origin (center of screen)
					float mousePosX = INPUT_SYSTEM->mMousePos.x - WINDOW_SYSTEM->GetWindowWidth() / 2.0f;
					float mousePosY = WINDOW_SYSTEM->GetWindowHeight() - INPUT_SYSTEM->mMousePos.y - WINDOW_SYSTEM->GetWindowHeight() / 2.0f;
					float buttonPosX = entityTransform->mPosition.x;
					float buttonPosY = entityTransform->mPosition.y;
					float buttonWidth = entityTransform->mScale.x / 2.0f;
					float buttonHeight = entityTransform->mScale.y / 2.0f;

					/*std::cout << "==== Hovering ====\n";
					std::cout << "Mouse Pos:" << mousePosX << ' ' << mousePosY << '\n';
					std::cout << "==== End Hovering ====\n";*/

					if (mousePosX >= buttonPosX - buttonWidth && mousePosX < buttonPosX + buttonWidth
						&& mousePosY > buttonPosY - buttonHeight && mousePosY <= buttonPosY + buttonHeight) {
					}
				}
				_2DShapeModel
					.SetBlend(modelComp->mEnableBlend)
					.Draw2D(_2DShaderProgram.mProgramId, model, texture->mGLTextureId, uvDimensions, uvOffsetPos, color);
			}
			//glEnable(GL_DEPTH);
		}

		glfwSwapBuffers(WINDOW_SYSTEM->mWindow);
	}
}