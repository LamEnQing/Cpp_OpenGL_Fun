#pragma once

#include "IBatchRenderer.h"

namespace OpenGLSandbox {
	class GridsBatchRendering : public IBatchRenderer {
	private:
		int gridWidth, gridHeight;

		void UpdateGrid();
	public:
		GridsBatchRendering();
		~GridsBatchRendering();

		void Draw(ShaderProgram& shdrPgm) override;
	};
}