/*
Created by Lam En Qing.

Description:
	StarSystem attempts to implement a perspective camera that orbits the world origin, as well as basic lighting.
	To demonstrate "perspective" as well as "lighting", a "star system" is rendered, comprising of the Sun and the Moon. (I know the Moon doesn't exactly orbit the Sun, but don't hate me)

	There are 2 shader files, "star_system" and "star_system_emissive". "star_system" is where the basic lighting stuffs are implemented.
	While "star_system_emissive" is an attempt of controlling the ambient lighting acting upon the vertices, there's no post-effect "emission", like there is no tiny amounts of rays being emitted that simulates the real-world effect of a light glowing in the dark, not just glowing at the source.
	The Moon uses the non-emissive shader while the Sun uses the emissive shader.

Credits:
	* LearnOpenGL
		Basic Lighting guide, for ambient and diffuse lighting. (https://learnopengl.com/Lighting/Basic-Lighting)
	* Megabyte Softworks
		Calculating the xyz of the camera position using elevation angle and the angle that circulates around the origin in a horizontal manner. (https://www.mbsoftworks.sk/tutorials/opengl4/026-camera-pt3-orbit-camera/)
*/

#include "StarSystem.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>

#include <iostream>
#include <string>

#include "ShaderProgram.h"
#include "Texture.h"

namespace OpenGLSandbox {
	StarSystem::StarSystem() {
		if (!mainShdr.CompileAndLink("star_system")) {
			mainShdr.Destroy();
			throw std::exception("Failed to compile 'star_system' shaders.\n");
		}
		if (!emissiveShdr.CompileAndLink("star_system_emissive")) {
			emissiveShdr.Destroy();
			throw std::exception("Failed to compile 'star_system_emissive' shaders.\n");
		}

		this->sunTex = LoadTexture("assets/textures/sun.png");
		this->moonTex = LoadTexture("assets/textures/moon.png");

		// Right-hand Rule means that x is forward, z is rightward, y is upwards
		// Vertex Order: Bottom Left, Bottom Right, Top Right, Top Left
		vertices.reserve(6);
#pragma region Side facing us
		vertices.emplace_back(0.5f, -0.5f,  0.5f, "+x", 0.0f, 0.0f);
		vertices.emplace_back(0.5f, -0.5f, -0.5f, "+x", 1.0f, 0.0f);
		vertices.emplace_back(0.5f,  0.5f, -0.5f, "+x", 1.0f, 1.0f);
		vertices.emplace_back(0.5f,  0.5f,  0.5f, "+x", 0.0f, 1.0f);
#pragma endregion

#pragma region Side facing away from us
		vertices.emplace_back(-0.5f, -0.5f, -0.5f, "-x", 0.0f, 0.0f);
		vertices.emplace_back(-0.5f, -0.5f,  0.5f, "-x", 1.0f, 0.0f);
		vertices.emplace_back(-0.5f,  0.5f,  0.5f, "-x", 1.0f, 1.0f);
		vertices.emplace_back(-0.5f,  0.5f, -0.5f, "-x", 0.0f, 1.0f);
#pragma endregion

#pragma region Left Side
		vertices.emplace_back( 0.5f, -0.5f, -0.5f, "-z", 0.0f, 0.0f);
		vertices.emplace_back(-0.5f, -0.5f, -0.5f, "-z", 1.0f, 0.0f);
		vertices.emplace_back(-0.5f,  0.5f, -0.5f, "-z", 1.0f, 1.0f);
		vertices.emplace_back( 0.5f,  0.5f, -0.5f, "-z", 0.0f, 1.0f);
#pragma endregion

#pragma region Right Side
		vertices.emplace_back(-0.5f, -0.5f, 0.5f, "+z", 0.0f, 0.0f);
		vertices.emplace_back( 0.5f, -0.5f, 0.5f, "+z", 1.0f, 0.0f);
		vertices.emplace_back( 0.5f,  0.5f, 0.5f, "+z", 1.0f, 1.0f);
		vertices.emplace_back(-0.5f,  0.5f, 0.5f, "+z", 0.0f, 1.0f);
#pragma endregion

#pragma region Top Side
		vertices.emplace_back(-0.5f, 0.5f, -0.5f, "+y", 0.0f, 0.0f);
		vertices.emplace_back(-0.5f, 0.5f,  0.5f, "+y", 1.0f, 0.0f);
		vertices.emplace_back( 0.5f, 0.5f,  0.5f, "+y", 1.0f, 1.0f);
		vertices.emplace_back( 0.5f, 0.5f, -0.5f, "+y", 0.0f, 1.0f);
#pragma endregion

#pragma region Bottom side
		vertices.emplace_back( 0.5f, -0.5f, -0.5f, "-y", 0.0f, 0.0f);
		vertices.emplace_back( 0.5f, -0.5f,  0.5f, "-y", 1.0f, 0.0f);
		vertices.emplace_back(-0.5f, -0.5f,  0.5f, "-y", 1.0f, 1.0f);
		vertices.emplace_back(-0.5f, -0.5f, -0.5f, "-y", 0.0f, 1.0f);
#pragma endregion

		indices.reserve(36);

		for (int i = 0; i < 6; i++) {
			VertexIndexType vertexStart = i * 4;
			indices.emplace_back(vertexStart);
			indices.emplace_back(vertexStart + 1);
			indices.emplace_back(vertexStart + 2);

			indices.emplace_back(vertexStart + 2);
			indices.emplace_back(vertexStart + 3);
			indices.emplace_back(vertexStart);
		}

		// A way to debug OpenGL issues, put it after every GL line if you find that glGetError returned a nonzero number
		//std::cout << "Line 1:" << glGetError() << std::endl;

		glGenVertexArrays(1, &this->vao);
		glBindVertexArray(this->vao);

		glGenBuffers(1, &this->vbo);
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(MyVertex), &vertices.front(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertex), 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertex), (void*)(offsetof(MyVertex, normal)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MyVertex), (void*)(offsetof(MyVertex, uv)));

		glGenBuffers(1, &this->ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(VertexIndexType), &indices.front(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	StarSystem::~StarSystem() {
		glDeleteTextures(1, &sunTex);
	}

	void StarSystem::Draw() {
		this->ImGuiControls();

		glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		glm::vec3 sunPos(0.0f, 0.0f, 0.0f);
		glm::vec3 moonPos(-50.0f, -20.0f, -50.0f);
		// Sun
		{
			emissiveShdr.use();
			auto emissionLoc = glGetUniformLocation(currShdrPgm, "myEmissiveStrength");
			glUniform1f(emissionLoc, emissiveStrength);
			DrawCube(sunPos, 30.0f, 0.0f, 0.0f, sunTex);
		}

		// Moon
		{
			mainShdr.use();
			auto lightPosLoc = glGetUniformLocation(currShdrPgm, "myLightPos");
			glUniform3fv(lightPosLoc, 1, glm::value_ptr(sunPos));

			/*auto lightColorLoc = glGetUniformLocation(currShdrPgm, "myLightColor");
			glUniform3fv(lightColorLoc, 1, glm::value_ptr(glm::vec3(emissiveStrength, emissiveStrength, emissiveStrength)));*/

			DrawCube(moonPos, 10.0f, 0.0f, 0.0f, moonTex);
		}

		glDisable(GL_DEPTH_TEST);
	}

	void StarSystem::DrawCube(glm::vec3 position, float scale, float rotX, float rotY, GLuint texId) {
		static float scrWidth = SCREEN_WIDTH / 4.0f, scrHeight = SCREEN_HEIGHT / 4.0f;

		// Orbit Camera Implementation: https://www.mbsoftworks.sk/tutorials/opengl4/026-camera-pt3-orbit-camera/
		float angle = glm::radians(camRotY), angleElevation = glm::radians(camElevation);

		camPos = glm::vec3(glm::cos(angle) * glm::cos(angleElevation), glm::sin(angleElevation), -glm::sin(angle) * glm::cos(angleElevation)); // cos(angleElevation) is the hypotenuse length for the x and z positions
		camPos *= camDist; // scale vector by camera distance

		glm::vec3 target(0.f);
		glm::vec3 camDir = glm::normalize(camPos - target);
		glm::vec3 camRight = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), camDir));
		glm::vec3 camUp = glm::cross(camDir, camRight);

		glm::mat4 viewMtx = glm::lookAt(camPos, target, camUp);
		//glm::mat4 projMtx = glm::ortho(-scrWidth / zoom, scrWidth / zoom, -scrHeight / zoom, scrHeight / zoom, 0.0f, 100000.0f);
		glm::mat4 projMtx = glm::perspective(glm::radians(fov), SCREEN_WIDTH/static_cast<float>(SCREEN_HEIGHT), 0.1f, 10000.0f);
		glm::mat4 finalMtx = projMtx * viewMtx;
		auto transformsLoc = glGetUniformLocation(currShdrPgm, "uTransformation");
		glUniformMatrix4fv(transformsLoc, 1, GL_FALSE, glm::value_ptr(finalMtx));

		glm::mat4 modelMtx(1.0f);
		modelMtx = glm::translate(modelMtx, glm::vec3(position[0], position[1], position[2]));
		modelMtx = glm::scale(modelMtx, glm::vec3(scale, scale, scale));
		modelMtx = glm::rotate(modelMtx, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
		modelMtx = glm::rotate(modelMtx, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));

		auto modelLoc = glGetUniformLocation(currShdrPgm, "uModel");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMtx));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texId);

		glBindVertexArray(this->vao);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}

	void StarSystem::ImGuiControls() {
		ImGui::Begin("StarSystem", NULL, ImGuiWindowFlags_HorizontalScrollbar);

		ImGui::SliderFloat("Emissive Strength", &emissiveStrength, 0.1f, 1.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp);
		//ImGui::SliderFloat("FOV", &fov, 10.0f, 120.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);

		ImGui::SliderFloat("Camera Distance", &camDist, 1.0f, 500.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp);
		ImGui::SliderFloat("Camera Rotate About Angle", &camRotY, -180.0f, 180.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp);
		ImGui::SliderFloat("Camera Elevation", &camElevation, -89.0f, 89.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp);

		ImGui::Text((std::string("Position:") + std::to_string(camPos.x) + ", " + std::to_string(camPos.y) + ", " + std::to_string(camPos.z)).c_str());

		ImGui::End();
	}
}