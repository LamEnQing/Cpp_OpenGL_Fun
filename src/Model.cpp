#include "pch.h"

#include "Model.h"
#include "WindowSystem.h"

namespace OpenGLFun {
	Model& Model::Init(std::vector<Vertex>& vertices) {
		_vertexCount = (unsigned int)vertices.size();

		glGenVertexArrays(1, &_vao);
		glGenBuffers(1, &_vbo);

		glBindVertexArray(_vao);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices.front(), GL_STATIC_DRAW);
		/*
			First param: Since vertex shader set location to 0, we set attribute to 0
			Second param: Size of vertex attribute, since it's vec3, there's only 3 values
			Third param: Set the type of data, vec3 is in float, so GL_FLOAT
			Fourth param: Do we want the data to be normalised? We don't want, so set to false
			Fifth param: Called "stride", it's how many space between consecutive vertex attributes. Since it's only position and rgb, that is 6 float values per vertex, we set to 3 * float's bytes
			Sixth param: The offset for the position data, since vertices only contain position values, no need to offset
		*/
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); // assign to location = 0, or the position variable in the vertex shader
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vertex::mPos))); // assign to location = 1, or the color variable in the vertex shader, also set the offset, offset of Pos
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind VBO
		glBindVertexArray(0); // unbind VAO

		return *this;
	}

	Model& Model::Destroy() {
		glDeleteVertexArrays(1, &_vao);
		glDeleteBuffers(1, &_vbo);
		return *this;
	}

	Model& Model::Draw2D(unsigned int& shaderProgram, glm::mat4& transformMtx, unsigned int textureId, Vec2f uvDimensions, Vec2f uvOffsetPos, Vec4f tintColor) {
		unsigned int transformLoc = glGetUniformLocation(shaderProgram, "uTransformation");
		unsigned int tintColorLoc = glGetUniformLocation(shaderProgram, "uTintColor");
		unsigned int texDimLoc = glGetUniformLocation(shaderProgram, "uTexDimensions");
		unsigned int texCoordLoc = glGetUniformLocation(shaderProgram, "uTexCoord");

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformMtx));

		glm::vec4 tintVec{ tintColor[0], tintColor[1], tintColor[2], tintColor[3] };
		glUniform4fv(tintColorLoc, 1, glm::value_ptr(tintVec));

		glm::vec2 someVec2{ uvDimensions[0], uvDimensions[1] };
		glUniform2fv(texDimLoc, 1, glm::value_ptr(someVec2));

		someVec2 = { uvOffsetPos[0], uvOffsetPos[1] };
		glUniform2fv(texCoordLoc, 1, glm::value_ptr(someVec2));

		if (this->_shouldCull)
			glEnable(GL_CULL_FACE); // enable cull, by default culls back faces. Back faces are CW

		if (this->_shouldBlend) {
			glEnable(GL_BLEND);
			glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		else {
			glDisable(GL_BLEND);
		}

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glBindVertexArray(_vao);
		glDrawArrays(this->_drawMode, 0, _vertexCount);
		glBindVertexArray(0); // unbind
		glBindTexture(GL_TEXTURE_2D, 0);

		if (this->_shouldBlend)
			glDisable(GL_BLEND);
		else
			glEnable(GL_BLEND);

		if (this->_shouldCull)
			glDisable(GL_CULL_FACE);
		glDisable(GL_TEXTURE_2D);

		return *this;
	}

	Model& Model::Draw3D(unsigned int& shaderProgram, glm::mat4& modelMtx, glm::mat4& viewMtx, glm::mat4& projMtx, unsigned int textureId, Vec4f tintColor) {
		unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
		unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
		unsigned int projLoc = glGetUniformLocation(shaderProgram, "proj");
		unsigned int tintColorLoc = glGetUniformLocation(shaderProgram, "tintColor");

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMtx));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMtx));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projMtx));

		glm::vec4 tintVec{ tintColor[0], tintColor[1], tintColor[2], tintColor[3] };
		glUniform4fv(tintColorLoc, 1, glm::value_ptr(tintVec));

		if (this->_shouldCull)
			glEnable(GL_CULL_FACE); // enable cull, by default culls back faces. Back faces are CW

		if (this->_shouldBlend)
			glEnable(GL_BLEND);
		else
			glDisable(GL_BLEND);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glBindVertexArray(_vao);
		glDrawArrays(this->_drawMode, 0, _vertexCount);
		glBindVertexArray(0); // unbind
		glBindTexture(GL_TEXTURE_2D, 0);

		if (this->_shouldBlend)
			glDisable(GL_BLEND);
		else
			glEnable(GL_BLEND);

		if (this->_shouldCull)
			glDisable(GL_CULL_FACE);
		glDisable(GL_TEXTURE_2D);

		return *this;
	}

	Model& Model::SetDrawMode(const int& mode) {
		this->_drawMode = mode;

		return *this;
	}

	Model& Model::SetCull(const bool& cull) {
		this->_shouldCull = cull;
		return *this;
	}

	Model& Model::SetBlend(const bool& blend) {
		this->_shouldBlend = blend;
		return *this;
	}
}