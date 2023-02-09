#include "Texture.h"

#include <stb/stb_image.h>

#include <iostream>

namespace OpenGLSandbox {
	GLuint LoadTexture(std::string path, int* imgWidth, int* imgHeight) {
		int width, height, channels;
		path = "assets/textures/" + path;
		auto* pixels = stbi_load(path.c_str(), &width, &height, &channels, 0);

		if (imgWidth != nullptr)
			*imgWidth = width;
		if (imgHeight != nullptr)
			*imgHeight = height;

		if (!pixels) {
			std::cout << "Failed to load image " << path << std::endl;
			return 0;
		}

		GLuint texId;
		glGenTextures(1, &texId); // create an id for the texture
		glBindTexture(GL_TEXTURE_2D, texId); // bind to transfer img data onto texture

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, channels == 3 ? GL_RGB8 : GL_RGBA8, width, height, 0, channels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(pixels);

		return texId;
	}
}