#pragma once
#include "pch.h"
#include "Framebuffer.h"
#include "Model.h"
#include "OrthoCamera2D.h"
#include "ShaderProgram.h"
#include "System.h"
#include "Texture.h"

namespace OpenGLFun {
	class GraphicSystem : public ISystem {
	public:
		Framebuffer mFramebuffer;

		GraphicSystem();
		virtual ~GraphicSystem() override;
		virtual void Update(float const&) override;

		void SetViewport(int posX, int posY, int width, int height);
		Texture* CreateGLTexture(std::string const& filepath);
		void DeleteGLTexture(Texture* texture);
	private:
		ShaderProgram _3DShaderProgram;
		ShaderProgram _2DShaderProgram;

		// Viewport
		int _viewportX, _viewportY, _viewportWidth, _viewportHeight;

		OpenGLFun::Model _2DShapeModel;

		OpenGLFun::OrthoCamera _camera2D;
	};
	extern GraphicSystem* GRAPHICS_SYSTEM;
}