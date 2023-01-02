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

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

#include "GlobalDeclared.h"
#include "ShaderProgram.h"

namespace OpenGLSandbox {
	class StarSystem {
	protected:
		struct MyVertex {
			float position[3];
			float normal[3];
			float uv[2];

			// directions must be: north, south, west, east, up, down
			MyVertex(float posX, float posY, float posZ, std::string direction, float u, float v) : position{ posX, posY, posZ }, normal{ 0.0f, 0.0f, 0.0f }, uv{ u, v } {
				if (direction == "+x")
					normal[0] = 1.0f;
				else if (direction == "-x")
					normal[0] = -1.0f;

				else if (direction == "+z")
					normal[2] = 1.0f;
				else if (direction == "-z")
					normal[2] = -1.0f;

				else if (direction == "+y")
					normal[1] = 1.0f;
				else if (direction == "-y")
					normal[1] = -1.0f;
			}
		};

		GLuint vao{ uint_max }, vbo{ uint_max }, ibo{ uint_max };
		GLuint sunTex{ uint_max }, moonTex{ uint_max };
		ShaderProgram mainShdr, emissiveShdr;

		float emissiveStrength{ 0.4f };
		float fov{ 45.0f };
		float camDist{ 70.0f }, camRotY{ 0.0f }, camElevation{ 0.0f }; // elevation traverse on the y-axis, makes cam go up or down. camRotY is rotating around by y-axis, basically doing a merry-go-around a point.
		glm::vec3 camPos{1.0f};

		std::vector<MyVertex> vertices;
		std::vector<VertexIndexType> indices;

		void ImGuiControls();

		void DrawCube(glm::vec3 position, float scale, float rotX, float rotY, GLuint texId);
	public:
		StarSystem();

		~StarSystem();

		void Draw();
	};
}