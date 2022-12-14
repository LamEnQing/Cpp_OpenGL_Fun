#define STB_IMAGE_IMPLEMENTATION
#define _CRTDBG_MAP_ALLOC
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include <crtdbg.h>
#include <exception>
#include <iostream>

#include "ShaderProgram.h"
#include "Vertex.h"

GLFWwindow* windowPtr = nullptr;
OpenGLSandbox::ShaderProgram batchShdrPgm;

void WindowCreation() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); // can resize or not?

	windowPtr = glfwCreateWindow(1280, 720, "OpenGL Sandbox", NULL, NULL);
	if (windowPtr == nullptr) {
		throw std::exception("Failed to create GLFW window");
	}
	glfwMakeContextCurrent(windowPtr);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		throw std::exception("Failed to initialize GLAD");
	}
}

void ShadersInit() {
	if (!batchShdrPgm.CompileAndLink("assets/shaders/batch.vert", "assets/shaders/batch.frag")) {
		batchShdrPgm.Destroy();
		throw std::exception("Failed to compile 2D shader program.\n");
	}
}

GLuint LoadTexture(const std::string& path) {
	int width, height, channels;
	auto* pixels = stbi_load(path.c_str(), &width, &height, &channels, 0);

	if (!pixels) {
		std::cout << "Failed to load image " << path << std::endl;
		return 0;
	}

	GLuint texId;
	glGenTextures(1, &texId); // create an id for the texture
	glBindTexture(GL_TEXTURE_2D, texId); // bind to transfer img data onto texture

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, channels == 3 ? GL_RGB8 : GL_RGBA8, width, height, 0, channels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	stbi_image_free(pixels);

	return texId;
}

// From: https://www.youtube.com/watch?v=Th4huqR77rI
class BatchRenderingIntro {
private:
	unsigned int vert_arr{ UINT32_MAX }, vert_buffer{ UINT32_MAX }, indices_buffer{ UINT32_MAX };

	GLuint sinonSqTex{ UINT32_MAX }, gwenStacyTex{ UINT32_MAX };
public:
	void Init() {
		this->sinonSqTex = LoadTexture("assets/textures/sinon_square.png");
		this->gwenStacyTex = LoadTexture("assets/textures/gwen_stacy.png");

		// Create a vertex array (VAO)
		glGenVertexArrays(1, &vert_arr);
		glBindVertexArray(vert_arr);

		// Create Vertex Buffer (VBO) and populate it with data from vertices
		glGenBuffers(1, &vert_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, vert_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(OpenGLSandbox::Vertex) * 1000, nullptr, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(OpenGLSandbox::Vertex), 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(OpenGLSandbox::Vertex), (void*)(offsetof(OpenGLSandbox::Vertex, color)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(OpenGLSandbox::Vertex), (void*)(offsetof(OpenGLSandbox::Vertex, texCoords)));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(OpenGLSandbox::Vertex), (void*)(offsetof(OpenGLSandbox::Vertex, texID)));

		uint32_t indices[] = {
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4
		};

		// Crate Indices Buffer (EBO)
		glGenBuffers(1, &indices_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind VBO
		glBindVertexArray(0); // unbind VAO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind EBO
	}

	void Draw() {
		// x,y,z, r,g,b,a, u,v, texIndex
		float vertices[] = {
			-0.75f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			-0.25f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			-0.25f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
			-0.75f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

			0.25f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			0.75f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			0.75f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			0.25f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		};

		// Set dynamic vertex buffer
		static bool hasRender = false;
		if (!hasRender) {
			glBindBuffer(GL_ARRAY_BUFFER, vert_buffer);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			hasRender = true;
		}

		glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		batchShdrPgm.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->sinonSqTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, this->gwenStacyTex);

		auto texLoc = glGetUniformLocation(batchShdrPgm.mProgramId, "myTextures");
		int samplers[2] = { 0, 1 };
		glUniform1iv(texLoc, 2, samplers);

		glBindVertexArray(vert_arr);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);
	}

	~BatchRenderingIntro() {
		GLuint textures[2] = { sinonSqTex, gwenStacyTex };
		glDeleteTextures(2, textures);
	}
} batchRenderIntro;

int main(void) {
	#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(175);
	#endif

	try {
		stbi_set_flip_vertically_on_load(1);

		WindowCreation();
		ShadersInit();

		batchRenderIntro.Init();

		while (!glfwWindowShouldClose(windowPtr)) {
			glfwPollEvents();

			// Drawing codes
			batchRenderIntro.Draw();

			glfwSwapBuffers(windowPtr);
		}
	}
	catch (const std::exception& e) {
		std::cout << "\nEncountered an error with the Engine:\n"
			<< "------------------------------------------------------------------------\n"
			<< e.what() << '\n';
		std::cout << "------------------------------------------------------------------------\n";
	}

	glfwTerminate();

	return 0;
}