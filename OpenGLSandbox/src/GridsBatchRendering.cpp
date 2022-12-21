#include "GridsBatchRendering.h"

#include <imgui/imgui.h>

namespace OpenGLSandbox {
	static int maxGridWidth{ static_cast<int>(SCREEN_WIDTH / 4.0f) }, maxGridHeight{ static_cast<int>(SCREEN_HEIGHT / 4.0f) };

	GridsBatchRendering::GridsBatchRendering() : gridWidth{ 3 }, gridHeight{ 2 } {
		size_t maxQuad = static_cast<size_t>(maxGridWidth * maxGridHeight);
		CreateBuffers(maxQuad * 4 * sizeof(Vertex), maxQuad * 6 * sizeof(VertexIndexType));

		UpdateGrid();
	}

	GridsBatchRendering::~GridsBatchRendering() {}

	void GridsBatchRendering::UpdateGrid() {
		this->meshData.clear();
		this->indexData.clear();

		float gridSqWidth = 2.0f / static_cast<float>(gridWidth);
		float gridSqHeight = 2.0f / static_cast<float>(gridHeight);

		size_t quadCount = gridWidth * gridHeight;

		this->meshData.reserve(quadCount);
		this->indexData.reserve(quadCount * 6);

		float red = 1.0f, green = 1.0f, blue = 1.0f;
		for (int j = 0; j < gridHeight; j++) { // Every row
			for (int i = 0; i < gridWidth; i++) { // Every column
				/* Say you have a grid:
					01
					23
				0 and 3 share the same color
				1 and 2 share another color

				0 and 2 lies on the first column, that is, column of index 0. So it's an even column
				1 and 3 lies on the second column, that is, column of index 1. So it's an odd column

				0 and 1 lies on the first row, that is, row of index 0. So it's an even row
				2 and 3 lies on the second row, that is, row of index 1. So it's an odd row
				*/
				if ((i % 2 == 0 && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1)) { // 0 and 3
					// make white
					red = green = blue = 1.0f;
				}
				else if ((i % 2 == 1 && j % 2 == 0) || (i % 2 == 0 && j % 2 == 1)) { // 1 and 2
					// make black
					red = green = blue = 0.0f;
				}

				this->meshData.push_back(Mesh(std::vector<VertexWrapper>{
					{ -1.0f + (gridSqWidth * i), 1.0f - (gridSqHeight * (j + 1)), 0.0f, Vertex(red, green, blue, 1.0f, 0.0f, 0.0f, 0.0f) },
					{ -1.0f + (gridSqWidth * (i + 1)), 1.0f - (gridSqHeight * (j + 1)), 0.0f, Vertex(red, green, blue, 1.0f, 0.0f, 0.0f, 0.0f) },
					{ -1.0f + (gridSqWidth * (i + 1)), 1.0f - (gridSqHeight * j), 0.0f, Vertex(red, green, blue, 1.0f, 0.0f, 0.0f, 0.0f) },
					{ -1.0f + (gridSqWidth * i), 1.0f - (gridSqHeight * j), 0.0f, Vertex(red, green, blue, 1.0f, 0.0f, 0.0f, 0.0f) }
				}, {}, { 1.0f, 1.0f }, 0.0f).ApplyTransformation(false));

				VertexIndexType vertexStart = static_cast<VertexIndexType>((j * gridWidth) + i) * 4; // get the current quad index, and then multiply by 4 to get the current vertex index
				this->indexData.emplace_back(vertexStart);
				this->indexData.emplace_back(vertexStart + 1);
				this->indexData.emplace_back(vertexStart + 2);

				this->indexData.emplace_back(vertexStart + 2);
				this->indexData.emplace_back(vertexStart + 3);
				this->indexData.emplace_back(vertexStart);

				std::cout << "vertexStart:" << vertexStart << std::endl;
			}
		}

		UpdateBuffers();
	}

	void GridsBatchRendering::Draw(ShaderProgram& shaderPgm) {
		ImGui::Begin("GridBatchRendering", NULL, ImGuiWindowFlags_HorizontalScrollbar);

		static int gridCols = gridWidth, gridRows = gridHeight;

		ImGui::Text("Grid Column count:"); ImGui::SameLine();
		ImGui::SliderInt("##GridBatchColumnSlider", &gridCols, 1, maxGridWidth, NULL, ImGuiSliderFlags_AlwaysClamp);

		ImGui::Text("Grid Row count:"); ImGui::SameLine();
		ImGui::SliderInt("##GridBatchRowSlider", &gridRows, 1, maxGridHeight, NULL, ImGuiSliderFlags_AlwaysClamp);

		ImGui::End();

		if (gridCols != gridWidth || gridRows != gridHeight) {
			this->gridWidth = gridCols;
			this->gridHeight = gridRows;
			UpdateGrid();
		}

		glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderPgm.use();
		glBindVertexArray(this->vao);
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexData.size()), GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}
}