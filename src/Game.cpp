#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Main.h"
#include "Game.h"

#include "Camera.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "Shape.h"

#include "Player.h"

namespace OpenGLFun {
namespace Game {
	// Common
	OpenGLFun::Player player;
	bool isPaused;

	// Client / Render
	bool isMouseLocked;

	float deltaTime;
	float lastFrame;

	bool initialMouseMovement; // if the application was first launched, there wouldn't be any old mouse pos.
	float mouseSensitivity;
	glm::vec2 mousePosOld;

	static ShaderProgram mainShaderProgram;

	OpenGLFun::Camera camera;

	OpenGLFun::Model rainbowCubeModel;
	OpenGLFun::Model axisModel;

	void Init(GLFWwindow* window) {
		// Common variables initialisation
		player.prevPos = player.pos = { -3.0f, 0.0f, 0.0f };
		player.headXRotOld = player.headXRot = player.yRotOld = player.yRot = 0.0f;
		player.movementSpeed = 3.0f;

		isPaused = false;

		// Client variables initialisation
		isMouseLocked = true;
		deltaTime = lastFrame = 0.0f;

		// Hide mouse cursor and lock mouse to the game
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		initialMouseMovement = true;
		mouseSensitivity = 1.0f;
		mousePosOld = { 0.0f, 0.0f };

		camera.camOffset = { 0.0f, 1.0f, 0.0f };
		camera.lookAt = { 1.0f, 0.0f, 0.0f };
		camera.camUp = { 0.0f, 1.0f, 0.0f };

		Shader vertexShader{}, fragmentShader{};
		if (!vertexShader.compile(ShaderType::Vertex, "assets/shaders/basic.vert")) {
			vertexShader.destroy();

			printf("Failed to compile vertex shader.\n");

			return;
		}
		if (!fragmentShader.compile(ShaderType::Fragment, "assets/shaders/basic.frag")) {
			fragmentShader.destroy();

			printf("Failed to compile fragment shader.\n");

			return;
		}

		if (!mainShaderProgram.compileAndLink(vertexShader, fragmentShader)) {
			mainShaderProgram.destroy();
			printf("Failed to compile the shader program.\n");
		}

		//Quad quad;
		Vertex vertex;
		std::vector<Vertex> vertices = {
			// bottom face (-ve y), blue
			vertex.Pos(-0.5f, -0.5f,  0.5f).Color(0.0f, 0.0f, 1.0f),  // front left
			vertex.Pos(-0.5f, -0.5f, -0.5f).Color(0.0f, 0.0f, 1.0f),  // back left
			vertex.Pos(0.5f, -0.5f, -0.5f).Color(0.0f, 0.0f, 1.0f),  // back right
			vertex.Pos(0.5f, -0.5f, -0.5f).Color(0.0f, 0.0f, 1.0f),  // back right
			vertex.Pos(0.5f, -0.5f,  0.5f).Color(0.0f, 0.0f, 1.0f),  // front right
			vertex.Pos(-0.5f, -0.5f,  0.5f).Color(0.0f, 0.0f, 1.0f),  // front left

			// top face (-ve y), cyan
			vertex.Pos(-0.5f, 0.5f, -0.5f).Color(0.0f, 1.0f, 1.0f),  // top left
			vertex.Pos(-0.5f, 0.5f,  0.5f).Color(0.0f, 1.0f, 1.0f),  // bottom left
			vertex.Pos(0.5f, 0.5f, -0.5f).Color(0.0f, 1.0f, 1.0f),  // top right
			vertex.Pos(0.5f, 0.5f, -0.5f).Color(0.0f, 1.0f, 1.0f),  // top right
			vertex.Pos(-0.5f, 0.5f,  0.5f).Color(0.0f, 1.0f, 1.0f),  // bottom left
			vertex.Pos(0.5f, 0.5f,  0.5f).Color(0.0f, 1.0f, 1.0f),  // bottom right

			// front face (+ve z), green
			vertex.Pos(-0.5f,  0.5f, 0.5f).Color(0.0f, 1.0f, 0.0f),  // top left
			vertex.Pos(-0.5f, -0.5f, 0.5f).Color(0.0f, 1.0f, 0.0f),  // bottom left
			vertex.Pos(0.5f,  0.5f, 0.5f).Color(0.0f, 1.0f, 0.0f),  // top right
			vertex.Pos(0.5f,  0.5f, 0.5f).Color(0.0f, 1.0f, 0.0f),  // top right
			vertex.Pos(-0.5f, -0.5f, 0.5f).Color(0.0f, 1.0f, 0.0f),  // bottom left
			vertex.Pos(0.5f, -0.5f, 0.5f).Color(0.0f, 1.0f, 0.0f),  // bottom right

			// back face (-ve z), yellow
			vertex.Pos(0.5f,  0.5f, -0.5f).Color(1.0f, 1.0f, 0.0f),  // top left
			vertex.Pos(0.5f, -0.5f, -0.5f).Color(1.0f, 1.0f, 0.0f),  // bottom left
			vertex.Pos(-0.5f,  0.5f, -0.5f).Color(1.0f, 1.0f, 0.0f),  // top right
			vertex.Pos(-0.5f,  0.5f, -0.5f).Color(1.0f, 1.0f, 0.0f),  // top right
			vertex.Pos(0.5f, -0.5f, -0.5f).Color(1.0f, 1.0f, 0.0f),  // bottom left
			vertex.Pos(-0.5f, -0.5f, -0.5f).Color(1.0f, 1.0f, 0.0f),  // bottom right

			// left face (-ve x), purple
			vertex.Pos(-0.5f,  0.5f, -0.5f).Color(1.0f, 0.0f, 1.0f),  // top left
			vertex.Pos(-0.5f, -0.5f, -0.5f).Color(1.0f, 0.0f, 1.0f),  // bottom left
			vertex.Pos(-0.5f,  0.5f,  0.5f).Color(1.0f, 0.0f, 1.0f),  // top right
			vertex.Pos(-0.5f,  0.5f,  0.5f).Color(1.0f, 0.0f, 1.0f),  // top right
			vertex.Pos(-0.5f, -0.5f, -0.5f).Color(1.0f, 0.0f, 1.0f),  // bottom left
			vertex.Pos(-0.5f, -0.5f,  0.5f).Color(1.0f, 0.0f, 1.0f),  // bottom right

			// right face (+ve x), red
			vertex.Pos(0.5f,  0.5f,  0.5f).Color(1.0f, 0.0f, 0.0f),  // top left
			vertex.Pos(0.5f, -0.5f,  0.5f).Color(1.0f, 0.0f, 0.0f),  // bottom left
			vertex.Pos(0.5f,  0.5f, -0.5f).Color(1.0f, 0.0f, 0.0f),  // top right
			vertex.Pos(0.5f,  0.5f, -0.5f).Color(1.0f, 0.0f, 0.0f),  // top right
			vertex.Pos(0.5f, -0.5f,  0.5f).Color(1.0f, 0.0f, 0.0f),  // bottom left
			vertex.Pos(0.5f, -0.5f, -0.5f).Color(1.0f, 0.0f, 0.0f)   // bottom right
		};
		// left face (-ve x), purple
		//quad.init(vertex.Pos(-0.5f, 0.5f, -0.5f).Color(1.0f, 0.0f, 1.0f), vertex.Pos(-0.5f, -0.5f, -0.5f).Color(1.0f, 0.0f, 1.0f), vertex.Pos(-0.5f, -0.5f, 0.5f).Color(1.0f, 0.0f, 1.0f), vertex.Pos(-0.5f, 0.5f, 0.5f).Color(1.0f, 0.0f, 1.0f));
		//vertices.insert(vertices.end(), quad.Vertices().begin(), quad.Vertices().end());
		//// right face (+ve x), red. Top-left, anti-clockwise
		//quad.init(vertex.Pos(0.5f, 0.5f, 0.5f).Color(1.0f, 0.0f, 0.0f), vertex.Pos(0.5f, -0.5f, 0.5f).Color(1.0f, 0.0f, 0.0f), vertex.Pos(0.5f, -0.5f, -0.5f).Color(1.0f, 0.0f, 0.0f), vertex.Pos(0.5f, 0.5f, -0.5f).Color(1.0f, 0.0f, 0.0f));
		//vertices.insert(vertices.end(), quad.Vertices().begin(), quad.Vertices().end());
		rainbowCubeModel.init(vertices);

		vertices.clear();
		vertices.insert(vertices.end(), {
			// x-axis, red
			vertex.Pos(0.0f, 0.0f, 0.0f).Color(1.0f, 0.0f, 0.0f),
			vertex.Pos(0.5f, 0.0f, 0.0f).Color(1.0f, 0.0f, 0.0f),

			// z-axis, blue
			vertex.Pos(0.0f, 0.0f, 0.0f).Color(0.0f, 0.0f, 1.0f),
			vertex.Pos(0.0f, 0.0f, 0.5f).Color(0.0f, 0.0f, 1.0f),

			// y-axis, green
			vertex.Pos(0.0f, 0.0f, 0.0f).Color(0.0f, 1.0f, 0.0f),
			vertex.Pos(0.0f, 0.5f, 0.0f).Color(0.0f, 1.0f, 0.0f)
			});
		axisModel.init(vertices).SetDrawMode(GL_LINE_STRIP).SetCull(false);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // enable wireframe mode
		glEnable(GL_DEPTH_TEST);
	}

	void Input(GLFWwindow* window) {
		float playerSpeed = player.movementSpeed * deltaTime;
		//float rotation = 10.0f * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			player.prevPos = player.pos;
			player.pos += glm::normalize(camera.lookAt) * playerSpeed;
		}
		else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			player.prevPos = player.pos;
			player.pos -= glm::normalize(camera.lookAt) * playerSpeed;
		}

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			player.prevPos = player.pos;
			player.pos -= glm::normalize(glm::cross(camera.lookAt, camera.camUp)) * playerSpeed;
		}
		else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			player.prevPos = player.pos;
			player.pos += glm::normalize(glm::cross(camera.lookAt, camera.camUp)) * playerSpeed;
		}

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			player.prevPos = player.pos;
			player.pos += camera.camUp * playerSpeed;
		}
		else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
			player.prevPos = player.pos;
			player.pos -= camera.camUp * playerSpeed;
		}

		/*if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			player.headXRotOld = player.headXRot;
			player.headXRot -= rotation;
		}
		else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {

			player.headXRotOld = player.headXRot;
			player.headXRot += rotation;
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			player.yRotOld = player.yRot;
			player.yRot -= rotation;
		}
		else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			player.yRotOld = player.yRot;
			player.yRot += rotation;
		}*/
	}

	void MouseCallback(GLFWwindow*, double xPosIn, double yPosIn) {
		if (isPaused) return;
		if (initialMouseMovement) {
			mousePosOld = { (float)xPosIn, (float)yPosIn };
			initialMouseMovement = false;
		}
		printf("Mouse Input: %.2f, %.2f\n", xPosIn, yPosIn);
		printf("Mouse Old: %.2f, %.2f\n", mousePosOld.x, mousePosOld.y);

		glm::vec2 mouseOffset = glm::vec2((float)xPosIn - mousePosOld.x, mousePosOld.y - (float)yPosIn); // mouse origin starts from top-left, so for y it has to be old-new to get the correct signed value
		mousePosOld = glm::vec2(xPosIn, yPosIn);
		printf("Mouse Offset: %.2f, %.2f\n", mouseOffset.x, mouseOffset.y);

		mouseOffset *= 0.1f * mouseSensitivity;

		player.headXRotOld = player.headXRot;
		player.headYRotOld = player.headYRot;

		player.headXRot += mouseOffset.x;
		player.headYRot += mouseOffset.y;

		if (player.headYRot > 89.0f)
			player.headYRot = 89.0f;
		else if (player.headYRot < -89.0f)
			player.headYRot = -89.0f;

		glm::vec3 dir = glm::vec3(1.0f);
		dir.x = cos(glm::radians(player.headXRot)) * cos(glm::radians(player.headYRot));
		dir.y = sin(glm::radians(player.headYRot));
		dir.z = sin(glm::radians(player.headXRot)) * cos(glm::radians(player.headYRot));
		camera.lookAtPrev = camera.lookAt;
		camera.lookAt = glm::normalize(dir);
		printf("Camera Look At: %.2f, %.2f, %.2f\n", dir.x, dir.y, dir.z);
	}

	void KeyCallback(GLFWwindow* window, int key, int, int action, int) {
		if (action == GLFW_PRESS) {
			if (key == GLFW_KEY_ESCAPE) {
				if (isMouseLocked) {
					isMouseLocked = false;
					isPaused = true;
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				}
				else {
					isMouseLocked = true;
					isPaused = false;
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
					glfwSetCursorPos(window, (double)engine->GetWindowWidth() / 2.0, (double)engine->GetWindowHeight() / 2.0);
					mousePosOld = { (float)engine->GetWindowWidth() / 2.0f, (float)engine->GetWindowHeight() / 2.0f };
				}
			}
		}
	}

	void Update() {
		if (player.prevPos != player.pos) {
			printf("Pos: %.2f, %.2f, %.2f\n", player.pos.x, player.pos.y, player.pos.z);
			player.prevPos = player.pos;
		}
		if (player.headXRotOld != player.headXRot) {
			printf("X Head Rot: %.2f\n", player.headXRot);
			player.headXRotOld = player.headXRot;
		}
		if (player.headYRotOld != player.headYRot) {
			printf("Y Head Rot: %.2f\n", player.headYRot);
			player.headYRotOld = player.headYRot;
		}
		if (player.yRotOld != player.yRot) {
			printf("Y Rot: %.2f\n", player.yRot);
			player.yRotOld = player.yRot;
		}
	}

	void Render() {
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mainShaderProgram.use();

		// camera pos, target pos, up direction
		glm::vec3 lookAtLerp = camera.lookAt/* + (camera.lookAt - camera.lookAtPrev) * deltaTime*/;
		glm::mat4 view = glm::lookAt(player.pos + camera.camOffset, player.pos + camera.camOffset + lookAtLerp, camera.camUp);
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);

		// fov, aspect ratio, znear, zfar
		proj = glm::perspective(glm::radians(45.0f), (float)engine->GetWindowWidth() / engine->GetWindowHeight(), 0.1f, 50.0f);

		//model = glm::scale(model, glm::vec3(0.5));
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		/*model = glm::rotate(model, glm::radians(player.yRot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(player.headXRot), glm::vec3(1.0f, 0.0f, 0.0f));*/
		rainbowCubeModel.draw(mainShaderProgram.programId, model, view, proj);

		model = glm::mat4(1.0f);
		axisModel.draw(mainShaderProgram.programId, model, view, proj);
	}


	const float& GetDeltaTime() {
		return deltaTime;
	}
	const float& GetLastFrame() {
		return lastFrame;
	}
	void SetDeltaTime(const float& time) {
		deltaTime = time;
	}
	void SetLastFrame(const float& frame) {
		lastFrame = frame;
	}
}
}