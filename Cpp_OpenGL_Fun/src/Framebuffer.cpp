#include "Framebuffer.h"

namespace OpenGLFun {
	Framebuffer::Framebuffer(int const& pixelsWidth, int const& pixelsHeight) : mHasResized{ true }, mTextureId{ 0 }, _bufferId{ 0 }, _width{ 0 }, _height{ 0 } {
		Resize(pixelsWidth, pixelsHeight);
	}

	Framebuffer::~Framebuffer() {
		if (mTextureId != 0)
			glDeleteTextures(1, &mTextureId);
		if (_bufferId != 0)
			glDeleteFramebuffers(1, &_bufferId);
	}

	void Framebuffer::PreResize(int const& pixelsWidth, int const& pixelsHeight) {
		mHasResized = false;

		_width = pixelsWidth;
		_height = pixelsHeight;
	}

	void Framebuffer::PostResize() {
		mHasResized = true;
		Resize(_width, _height);
	}

	void Framebuffer::Resize(int const& pixelsWidth, int const& pixelsHeight) {
		mHasResized = true;

		if (mTextureId != 0)
			glDeleteTextures(1, &mTextureId);
		if (_bufferId != 0)
			glDeleteFramebuffers(1, &_bufferId);

		glGenFramebuffers(1, &_bufferId);
		glBindFramebuffer(GL_FRAMEBUFFER, _bufferId);
		// Create texture to attach to framebuffer
		glGenTextures(1, &mTextureId);
		glBindTexture(GL_TEXTURE_2D, mTextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pixelsWidth, pixelsHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTextureId, 0);

		// ==== RenderBuffer ====
		unsigned int renderBuffer;
		glGenRenderbuffers(1, &renderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, pixelsWidth, pixelsHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);

		auto frameBufferError = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (frameBufferError != GL_FRAMEBUFFER_COMPLETE)
			throw SimpleException(std::string("Framebuffer is not complete, error status:") + std::to_string(frameBufferError));
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteRenderbuffers(1, &renderBuffer);
	}

	void Framebuffer::Bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, _bufferId);
	}

	void Framebuffer::Unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::Draw(ShaderProgram& shdrPgm, Model& model) {
		shdrPgm.use();
		model.SetCull(true).SetBlend(true).Draw2D(shdrPgm.mProgramId, glm::mat4(1.0f), mTextureId, { 1.0f, 1.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	}
}