#pragma once
#include "pch.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "System.h"
#include "Texture.h"

namespace OpenGLFun {
	class GraphicSystem : public ISystem {
	public:
		GraphicSystem();
		virtual ~GraphicSystem() override;
		virtual void Update(float const&) override;

		void CreateGLTexture(Texture* texture);
		void DeleteGLTexture(unsigned int& texture);
	private:
		ShaderProgram _mainShaderProgram;
		ShaderProgram _2DShaderProgram;

		OpenGLFun::Model _rainbowCubeModel;
		OpenGLFun::Model _axisModel;
		OpenGLFun::Model _2DShapeModel;
	};
	extern GraphicSystem* GRAPHICS_SYSTEM;
}