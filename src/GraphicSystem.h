#pragma once
#include "Model.h"
#include "ShaderProgram.h"
#include "System.h"

namespace OpenGLFun {
	class GraphicSystem : public ISystem {
	public:
		GraphicSystem();
		virtual ~GraphicSystem() override;
		virtual void Update(double const&) override;
	private:
		ShaderProgram _mainShaderProgram;
		ShaderProgram _2DShaderProgram;

		OpenGLFun::Model _rainbowCubeModel;
		OpenGLFun::Model _axisModel;
		OpenGLFun::Model _2DShapeModel;
	};
}