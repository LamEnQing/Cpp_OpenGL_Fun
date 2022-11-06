#pragma once
#include "pch.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "System.h"
#include "Texture.h"

namespace OpenGLFun {
	class GraphicSystem : public ISystem {
	public:
		unsigned int mFrameBufferTex;

		GraphicSystem();
		virtual ~GraphicSystem() override;
		virtual void Update(float const&) override;

		void SetViewport(int posX, int posY, int width, int height);
		void CreateGLTexture(Texture* texture);
		void DeleteGLTexture(unsigned int& texture);
	private:
		ShaderProgram _3DShaderProgram;
		ShaderProgram _2DShaderProgram;

		unsigned int _frameBuffer;
		unsigned int _renderBuffer;

		// Viewport
		int _viewportX, _viewportY, _viewportWidth, _viewportHeight;

		OpenGLFun::Model _2DShapeModel;
	};
	extern GraphicSystem* GRAPHICS_SYSTEM;
}