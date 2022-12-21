// Most code from: https://www.youtube.com/watch?v=Th4huqR77rI
#pragma once

#include "IBatchRenderer.h"

namespace OpenGLSandbox {
	class BatchRendering : public IBatchRenderer {
	private:
		GLuint sinonSqTex{ UINT32_MAX }, gwenStacyTex{ UINT32_MAX };

		bool hasVertexDataChanged{ false };

		const size_t maxQuad{ 100 };
		size_t quadCount{ 0 };
		int selectedQuad{ -1 };
	public:
		BatchRendering();
		~BatchRendering();

		void Draw(OpenGLSandbox::ShaderProgram& shdrPgm) override;
	};
}