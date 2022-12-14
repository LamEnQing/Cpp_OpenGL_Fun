#pragma once
#include "pch.h"
#include "ShaderProgram.h"
#include "Model.h"

namespace OpenGLFun {
	class Framebuffer { 
	public:
		unsigned mTextureId;
		bool mHasResized;

		Framebuffer(int const& pixelsWidth, int const& pixelsHeight);
		~Framebuffer();

		void PreResize(int const& pixelsWidth, int const& pixelsHeight);
		void PostResize();
		void Resize(int const& pixelsWidth, int const& pixelsHeight);
		void Bind();
		void Unbind();

		void Draw(ShaderProgram& shdrPgm, Model& model);
	private:
		unsigned int _bufferId;
		int _width, _height;
	};
}