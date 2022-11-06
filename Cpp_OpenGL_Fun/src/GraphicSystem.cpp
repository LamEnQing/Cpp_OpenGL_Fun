#include "pch.h"

#include "ComponentManager.h"
#include "Engine.h"
#include "GraphicSystem.h"
#include "InputSystem.h"
#include "LevelManager.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "WindowSystem.h"

#include "Button.h"
#include "Camera.h"
#include "Color.h"
#include "ModelComponent.h"
#include "Transform.h"
#include "Sprite.h"

namespace OpenGLFun {
	GraphicSystem* GRAPHICS_SYSTEM = nullptr;

	GraphicSystem::GraphicSystem() : ISystem(), mFramebuffer(WINDOW_SYSTEM->GetWindowWidth(), WINDOW_SYSTEM->GetWindowHeight()), _3DShaderProgram(), _viewportX{ 0 }, _viewportY{ 0 }, _viewportWidth{ WINDOW_SYSTEM->mFrameWidth }, _viewportHeight{ WINDOW_SYSTEM->mFrameHeight } {
		if (GRAPHICS_SYSTEM != nullptr)
			throw SimpleException("Graphics system already created!");

		GRAPHICS_SYSTEM = this;

		Shader vertexShader{}, fragmentShader{};
		if (!vertexShader.Compile(ShaderType::Vertex, "assets/shaders/3d.vert")) {
			vertexShader.Destroy();

			throw SimpleException("Failed to compile 3d.vert.\n");
		}
		if (!fragmentShader.Compile(ShaderType::Fragment, "assets/shaders/3d.frag")) {
			fragmentShader.Destroy();

			throw SimpleException("Failed to compile 3d.frag.\n");
		}
		if (!_3DShaderProgram.CompileAndLink(vertexShader, fragmentShader)) {
			_3DShaderProgram.Destroy();
			throw SimpleException("Failed to compile 3D shader program.\n");
		}

		if (!vertexShader.Compile(ShaderType::Vertex, "assets/shaders/2d.vert")) {
			vertexShader.Destroy();

			throw SimpleException("Failed to compile 2d.vert.\n");
		}
		if (!fragmentShader.Compile(ShaderType::Fragment, "assets/shaders/2d.frag")) {
			fragmentShader.Destroy();

			throw SimpleException("Failed to compile 2d.frag.\n");
		}

		if (!_2DShaderProgram.CompileAndLink(vertexShader, fragmentShader)) {
			_2DShaderProgram.Destroy();
			throw SimpleException("Failed to compile 2D shader program.\n");
		}

		Mesh* mesh = new Mesh();
		Vertex vertex;
		std::vector<unsigned int> indices;
		std::vector<Vertex> vertices = {
			vertex.Pos(-1.0f, 1.0f, 0.0f).Color(1.0f, 1.0f, 1.0f).UV(0.0f, 1.0f), // top left
			vertex.Pos(-1.0f, -1.0f, 0.0f).UV(0.0f, 0.0f), // bottom left
			vertex.Pos(1.0f, -1.0f, 0.0f).UV(1.0f, 0.0f), // bottom right
			vertex.Pos(1.0f, 1.0f, 0.0f).UV(1.0f, 1.0f), // top right
		};
		indices = {
			0, 1, 2,
			2, 3, 0
		};
		mesh->Init(vertices, indices)->SetCull(false)->SetBlend(true);
		_2DShapeModel.AddMesh("idk", std::shared_ptr<Mesh>(mesh));

		SetViewport(0, 0, WINDOW_SYSTEM->GetWindowWidth(), WINDOW_SYSTEM->GetWindowHeight());
	}

	GraphicSystem::~GraphicSystem() {
		_3DShaderProgram.Destroy();
	}

	void GraphicSystem::Update(float const&) {
		glViewport(_viewportX, _viewportY, _viewportWidth, _viewportHeight);
		mFramebuffer.Bind();

		glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (ENGINE->mInDebugMode) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // enable wireframe mode
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // reset to default view
		}

		_3DShaderProgram.use();

		glm::mat4 model;
		Vec4f tintColor(1.0f);
		// 3D Drawing
		if (ENGINE->mPlayerId != -1 && COMPONENT_MANAGER->HasComponent(ENGINE->mPlayerId, ComponentType::Camera)) {
			glEnable(GL_DEPTH_TEST);

			Camera* playerCamera = COMPONENT_MANAGER->GetComponent<Camera>(ENGINE->mPlayerId, ComponentType::Camera);
			Transform* playerTransforms = COMPONENT_MANAGER->GetComponent<Transform>(ENGINE->mPlayerId, ComponentType::Transform);

			// Calculate camera offset, by applying rotation to the cam offset. Note: mCamRotation.x rotates about the y-axis
			float xRot = glm::radians(playerCamera->mCamRotation.x);
			Vec2f camOffsetRotated = Vec2f(playerCamera->mCamOffset.x * cos(xRot) - playerCamera->mCamOffset.y * sin(xRot), playerCamera->mCamOffset.x * sin(xRot) + playerCamera->mCamOffset.y * cos(xRot));

			// camera pos, target pos, up direction
			Vec3f lookAtLerp = playerCamera->mLookAt;
			Vec3f camPos = playerTransforms->mPosition + Vec3f(camOffsetRotated.x, playerCamera->mEyeHeight - lookAtLerp.y, camOffsetRotated.y);
			Vec3f target = playerTransforms->mPosition + Vec3f(0.0f, playerCamera->mEyeHeight, 0.0f) + lookAtLerp;

			glm::mat4 view = glm::lookAt(glm::vec3(camPos.x, camPos.y, camPos.z), glm::vec3(target.x, target.y, target.z), glm::vec3(playerCamera->mCamUp.x, playerCamera->mCamUp.y, playerCamera->mCamUp.z));
			glm::mat4 proj = glm::mat4(1.0f);

			// fov, width/height ratio, near, far
			proj = glm::perspective(glm::radians(45.0f), static_cast<float>(WINDOW_SYSTEM->GetWindowWidth()) / WINDOW_SYSTEM->GetWindowHeight(), 0.1f, 50.0f);

			for (EntityId const& entityId : ENTITY_MANAGER->GetEntities()) {
				if (!COMPONENT_MANAGER->HasComponent(entityId, ComponentType::Transform) || !COMPONENT_MANAGER->HasComponent(entityId, ComponentType::Model))
					continue;

				ModelComponent* modelComp = COMPONENT_MANAGER->GetComponent<ModelComponent>(entityId, ComponentType::Model);
				if (modelComp->mModelType == ModelType::TwoD) continue;

				model = glm::mat4(1.0f);
				Texture* texture = RESOURCE_MANAGER->GetTexture("no_texture.png");
				Transform* entityTransform = COMPONENT_MANAGER->GetComponent<Transform>(entityId, ComponentType::Transform);

				if (COMPONENT_MANAGER->HasComponent(entityId, ComponentType::Sprite))
					texture = RESOURCE_MANAGER->GetTexture(COMPONENT_MANAGER->GetComponent<Sprite>(entityId, ComponentType::Sprite)->mTextureFilepath);
				if (COMPONENT_MANAGER->HasComponent(entityId, ComponentType::Color))
					tintColor = COMPONENT_MANAGER->GetComponent<Color>(entityId, ComponentType::Color)->mRgba;
				model = glm::translate(glm::mat4(1.0f), vec3f_to_vec3(entityTransform->mPosition));
				model = glm::rotate(model, glm::radians(entityTransform->mRotation.y), glm::vec3(0.0f, 1.0f, 0.0));
				model = glm::scale(model, vec3f_to_vec3(entityTransform->mScale));

				RESOURCE_MANAGER->Get3DModel(entityId)
					->SetCull(modelComp->mShouldCull)
					.Draw3D(_3DShaderProgram.mProgramId, model, view, proj, texture->mGLTextureId, tintColor);
			}
			glDisable(GL_DEPTH_TEST);
		}

		// 2D Drawing
		glm::vec3 sample_vec3;
		_2DShaderProgram.use();
		for (EntityId const& entityId : ENTITY_MANAGER->GetEntities()) {
			if (!COMPONENT_MANAGER->HasComponent(entityId, ComponentType::Transform) || !COMPONENT_MANAGER->HasComponent(entityId, ComponentType::Model))
				continue;

			ModelComponent* modelComp = COMPONENT_MANAGER->GetComponent<ModelComponent>(entityId, ComponentType::Model);
			if (modelComp->mModelType == ModelType::ThreeD) continue;

			model = glm::mat4(1.0f);
			Texture* texture = RESOURCE_MANAGER->GetTexture("no_texture.png");
			Transform* entityTransform = COMPONENT_MANAGER->GetComponent<Transform>(entityId, ComponentType::Transform);

			if (COMPONENT_MANAGER->HasComponent(entityId, ComponentType::Sprite))
				texture = RESOURCE_MANAGER->GetTexture(COMPONENT_MANAGER->GetComponent<Sprite>(entityId, ComponentType::Sprite)->mTextureFilepath);
			tintColor = {1.0f, 1.0f, 1.0f, 1.0f};
			if (COMPONENT_MANAGER->HasComponent(entityId, ComponentType::Color))
				tintColor = COMPONENT_MANAGER->GetComponent<Color>(entityId, ComponentType::Color)->mRgba;

			Vec2f uvDimensions = { 1.0f, 1.0f }, uvOffsetPos = { 0.0f, 0.0f };
			if (COMPONENT_MANAGER->HasComponent(entityId, ComponentType::Sprite)) {
				Sprite* spriteComp = COMPONENT_MANAGER->GetComponent<Sprite>(entityId, ComponentType::Sprite);
				uvDimensions = { static_cast<float>(spriteComp->mUVDimensions[0]) / texture->imgWidth, static_cast<float>(spriteComp->mUVDimensions[1]) / texture->imgHeight };
				uvOffsetPos = { static_cast<float>(spriteComp->mUVPosition[0]) / texture->imgWidth, static_cast<float>(spriteComp->mUVPosition[1]) / texture->imgHeight };
			}

			sample_vec3 = vec3f_to_vec3(entityTransform->mPosition);
			sample_vec3[0] /= static_cast<float>(WINDOW_SYSTEM->GetWindowWidth()) / 2.0f;
			sample_vec3[1] /= static_cast<float>(WINDOW_SYSTEM->GetWindowHeight()) / 2.0f;
			model = glm::translate(glm::mat4(1.0f), sample_vec3);
			//model = glm::rotate(model, glm::radians(entityTransform->mRotation.y), glm::vec3(0.0f, 1.0f, 0.0));

			sample_vec3 = vec3f_to_vec3(entityTransform->mScale);
			if (entityTransform->mScale.x < 0)
				sample_vec3.x = WINDOW_SYSTEM->mFrameWidth;
			if (entityTransform->mScale.y < 0)
				sample_vec3.y = WINDOW_SYSTEM->mFrameHeight;
			sample_vec3[0] /= WINDOW_SYSTEM->mFrameWidth;
			sample_vec3[1] /= WINDOW_SYSTEM->mFrameHeight;
			sample_vec3[2] = 0.0f;
			model = glm::scale(model, sample_vec3);

			RESOURCE_MANAGER->Get2DModel(modelComp->mModelFilepath)
				->SetCull(modelComp->mShouldCull)
				.Draw2D(_2DShaderProgram.mProgramId, model, texture->mGLTextureId, uvDimensions, uvOffsetPos, tintColor);
		}
		glm::mat4 transformMtx(1.0f);
		mFramebuffer.Unbind();

		// Draw Scene image
		mFramebuffer.Draw(_2DShaderProgram, _2DShapeModel);

		if (ENGINE->mIsPaused) {
			_2DShaderProgram.use();

			/*for (EntityId const& entityId : LEVEL_MANAGER->mPauseScreenObjs) {
				if (!COMPONENT_MANAGER->HasComponent(entityId, ComponentType::Transform) || !COMPONENT_MANAGER->HasComponent(entityId, ComponentType::Model) || !COMPONENT_MANAGER->HasComponent(entityId, ComponentType::Color))
					continue;

				Texture* texture = RESOURCE_MANAGER->GetTexture("no_texture.png");
				if (COMPONENT_MANAGER->HasComponent(entityId, ComponentType::Sprite))
					texture = RESOURCE_MANAGER->GetTexture(COMPONENT_MANAGER->GetComponent<Sprite>(entityId, ComponentType::Sprite)->mTextureFilepath);

				ModelComponent* modelComp = COMPONENT_MANAGER->GetComponent<ModelComponent>(entityId, ComponentType::Model);
				Transform* entityTransform = COMPONENT_MANAGER->GetComponent<Transform>(entityId, ComponentType::Transform);

				sample_vec3 = vec3f_to_vec3(entityTransform->mPosition);
				sample_vec3[0] /= static_cast<float>(WINDOW_SYSTEM->GetWindowWidth()) / 2.0f;
				sample_vec3[1] /= static_cast<float>(WINDOW_SYSTEM->GetWindowHeight()) / 2.0f;
				model = glm::translate(glm::mat4(1.0f), sample_vec3);

				sample_vec3 = vec3f_to_vec3(entityTransform->mScale);
				sample_vec3[0] /= WINDOW_SYSTEM->GetWindowWidth();
				sample_vec3[1] /= WINDOW_SYSTEM->GetWindowHeight();
				sample_vec3[2] = 0.0f;
				sample_vec3 *= 2.0f;
				model = glm::scale(model, sample_vec3);

				Vec2f uvDimensions = { 1.0f, 1.0f }, uvOffsetPos = { 0.0f, 0.0f };
				if (COMPONENT_MANAGER->HasComponent(entityId, ComponentType::Sprite)) {
					Sprite* spriteComp = COMPONENT_MANAGER->GetComponent<Sprite>(entityId, ComponentType::Sprite);
					if (spriteComp->mUVDimensions[0] != 0)
						uvDimensions = { static_cast<float>(spriteComp->mUVDimensions[0]) / texture->imgWidth, static_cast<float>(spriteComp->mUVDimensions[1]) / texture->imgHeight };
					uvOffsetPos = { static_cast<float>(spriteComp->mUVPosition[0]) / texture->imgWidth, static_cast<float>(spriteComp->mUVPosition[1]) / texture->imgHeight };
				}
				auto color = COMPONENT_MANAGER->GetComponent<Color>(entityId, ComponentType::Color)->mRgba;

				// need to localise the mouse pos origin (top left) to opengl origin (center of screen)
				float mousePosX = INPUT_SYSTEM->mMousePos.x - WINDOW_SYSTEM->GetWindowWidth() / 2.0f;
				float mousePosY = WINDOW_SYSTEM->GetWindowHeight() - INPUT_SYSTEM->mMousePos.y - WINDOW_SYSTEM->GetWindowHeight() / 2.0f;

				if (INPUT_SYSTEM->IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
					std::cout << "Mouse Pos:" << mousePosX << ' ' << mousePosY << '\n';
				}

				if (COMPONENT_MANAGER->HasComponent(entityId, ComponentType::Button)) {
					float buttonPosX = entityTransform->mPosition.x;
					float buttonPosY = entityTransform->mPosition.y;
					float buttonWidth = entityTransform->mScale.x / 2.0f;
					float buttonHeight = entityTransform->mScale.y / 2.0f;

					if (mousePosX >= buttonPosX - buttonWidth && mousePosX < buttonPosX + buttonWidth
						&& mousePosY > buttonPosY - buttonHeight && mousePosY <= buttonPosY + buttonHeight) {
						Button* buttonComp = COMPONENT_MANAGER->GetComponent<Button>(entityId, ComponentType::Button);
						color = buttonComp->mHoverRgba;
						if (buttonComp->mHoverUVDimensions[0] != 0)
							uvDimensions = { static_cast<float>(buttonComp->mHoverUVDimensions[0]) / texture->imgWidth, static_cast<float>(buttonComp->mHoverUVDimensions[1]) / texture->imgHeight };
						uvOffsetPos = { static_cast<float>(buttonComp->mHoverUVPos[0]) / texture->imgWidth, static_cast<float>(buttonComp->mHoverUVPos[1]) / texture->imgHeight };
					}
				}
				_2DShapeModel
					.SetCull(modelComp->mShouldCull)
					.SetBlend(modelComp->mEnableBlend)
					.Draw2D(_2DShaderProgram.mProgramId, model, texture->mGLTextureId, uvDimensions, uvOffsetPos, color);
			}*/
		}
	}

	void GraphicSystem::SetViewport(int posX, int posY, int width, int height) {
		_viewportX = posX;
		_viewportY = posY;
		_viewportWidth = width;
		_viewportHeight = height;
	}

	void GraphicSystem::CreateGLTexture(Texture* texture) {
		unsigned int texId;

		glGenTextures(1, &texId); // create an id for the texture
		glBindTexture(GL_TEXTURE_2D, texId); // bind to transfer img data onto texture

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		GLint imgFormat = texture->imgChannels == 3 ? GL_RGB : GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, imgFormat, texture->imgWidth, texture->imgHeight, 0, imgFormat, GL_UNSIGNED_BYTE, texture->imgData);
		glGenerateMipmap(GL_TEXTURE_2D);

		texture->mGLTextureId = texId;
	}

	void GraphicSystem::DeleteGLTexture(unsigned int& textureId) {
		glDeleteTextures(1, &textureId);
	}
}