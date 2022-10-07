#include "pch.h"
#include "Model.h"

#include "BasicShape.h"
#include "Serializer.h"
#include "WindowSystem.h"

namespace OpenGLFun {
	static std::unordered_map<std::string, int> DRAW_MODES = {
		{ "triangles", GL_TRIANGLES },
		{ "lines", GL_LINE_STRIP }
	};

	Model::Model() {}

	Model& Model::Init(std::vector<std::shared_ptr<IShape>>& shapes) {
		_vertexCount = 0;
		std::vector<Vertex> vertices;
		std::vector<IShape::ElementIndex> indices;
		for (std::shared_ptr<IShape> shape : shapes) {
			vertices.insert(vertices.end(), shape->Vertices().begin(), shape->Vertices().end());
			for (IShape::ElementIndex& index : shape->Indices()) {
				index += static_cast<IShape::ElementIndex>(_vertexCount); // offset the index by how many vertices were added before this shape
				indices.push_back(index);
			}
			_vertexCount += static_cast<int>(shape->Vertices().size());
		}

		return Init(vertices, indices);
	}

	Model& Model::Init(std::vector<Vertex>& vertices) {
		_vertexCount = static_cast<int>(vertices.size());

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
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vertex::mPos) + sizeof(Vertex::mColor))); // assign to location = 2, or the texture variable in the vertex shader, also set the offset, offset of Pos + Color
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind VBO
		glBindVertexArray(0); // unbind VAO

		return *this;
	}

	Model& Model::Init(std::vector<Vertex>& vertices, std::vector<IShape::ElementIndex>& indices) {
		_vertexCount = static_cast<int>(vertices.size());
		_indexCount = static_cast<int>(indices.size());

		glGenVertexArrays(1, &_vao);
		glGenBuffers(1, &_vbo);
		glGenBuffers(1, &_ebo);

		glBindVertexArray(_vao);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices.front(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices.front(), GL_STATIC_DRAW);

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
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vertex::mPos) + sizeof(Vertex::mColor))); // assign to location = 2, or the texture variable in the vertex shader, also set the offset, offset of Pos + Color
		glEnableVertexAttribArray(2);

		// unbind EBO only after unbinding VAO, since VAO needs to save the information of EBO
		glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind VBO
		glBindVertexArray(0); // unbind VAO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind EBO

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
		//glDrawArrays(this->_drawMode, 0, _vertexCount);
		glDrawElements(this->_drawMode, _indexCount, GL_UNSIGNED_INT, 0);
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

		// setup settings
		if (this->_shouldCull)
			glEnable(GL_CULL_FACE); // enable cull, by default culls back faces. Back faces are CW
		else
			glDisable(GL_CULL_FACE);

		if (this->_shouldBlend)
			glEnable(GL_BLEND);
		else
			glDisable(GL_BLEND);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureId);

		// actual drawing
		glBindVertexArray(_vao);
		//glDrawArrays(this->_drawMode, 0, _vertexCount);
		glDrawElements(this->_drawMode, _indexCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0); // unbind

		glBindTexture(GL_TEXTURE_2D, 0);

		// "turn off" settings
		if (this->_shouldCull)
			glDisable(GL_CULL_FACE);
		else
			glEnable(GL_CULL_FACE);

		if (this->_shouldBlend)
			glDisable(GL_BLEND);
		else
			glEnable(GL_BLEND);

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

	void Model::DeserializeJson(rapidjson::Value const& jsonObj) {
		if (!jsonObj.HasMember("draw_mode") || !jsonObj["draw_mode"].IsString())
			throw JsonReadException("Model", "draw_mode", "string");

		std::string drawModeStr = jsonObj["draw_mode"].GetString();
		if (DRAW_MODES.find(drawModeStr) == DRAW_MODES.end())
			throw SimpleException(std::string("In Model, could not find draw_mode \"") + drawModeStr + "\"");

		SetDrawMode(DRAW_MODES.at(drawModeStr));
		std::vector<std::shared_ptr<IShape>> shapes;

		if (!jsonObj.HasMember("shapes") || !jsonObj["shapes"].IsArray())
			throw JsonReadException("Model", "shapes", "array");

		rapidjson::Value const& shapeArr = jsonObj["shapes"].GetArray();
		for (rapidjson::SizeType i = 0; i < shapeArr.Size(); i++) {
			const rapidjson::Value& shapeJson = shapeArr[i];

			if (!shapeJson.IsObject())
				throw JsonReadException("Model", std::string("shapes[") + std::to_string(i) + "]", "JSON object");

			// "type" member must be a string
			if (!shapeJson.HasMember("type") || !shapeJson["type"].IsString())
				throw JsonReadException("Model", std::string("shapes[") + std::to_string(i) + "]", "type", "string");

			IShape* shape = SHAPE_MANAGER->GetShapeCreator(shapeJson["type"].GetString())->Create();
			shape->Deserialize(shapeJson);
			shapes.emplace_back(shape);
		}

		Init(shapes);
	}

	void Model::DeserializeObj(std::string const& filepath) {
		std::ifstream fs;
		fs.open(filepath, std::ifstream::in);

		if (!fs.is_open())
			throw SimpleException(std::string("Failed to open ") + filepath);

		std::vector<std::shared_ptr<IShape>> shapes;
		BasicShape* shape = new BasicShape();

		std::vector<Vertex> vertices;
		std::vector<IShape::ElementIndex> vertexIndices;
		std::vector<Vec2f> textureCoordinates;

		std::string line;
		char c; // to read starting character, so that we can read the number values after it
		std::stringstream ss;

		std::cout << "\nDeserialising obj " << filepath << "\n----------------------------\n";
		while (!std::getline(fs, line).eof()) {
			if (Serializer::DoesFilenameStartWith(line, "o "))
				std::cout << line << '\n';

			if (Serializer::DoesFilenameStartWith(line, "v ")) {
				ss.clear(); // must clear stream, so as to not "corrupt" the reading
				ss << line;

				float x, y, z;
				ss >> c >> x >> y >> z; // read v, then the next 3 float values

				vertices.push_back(Vertex().Pos(x, y, z).Color(0.7f, 0.0f, 0.7f));
				std::cout << "v " << x << ' ' << y << ' ' << z << '\n';
			}

			if (Serializer::DoesFilenameStartWith(line, "vt ")) {
				std::cout << line << '\n';
				ss.clear();
				ss << line; // line would be "vt u v"

				float u, v;
				ss >> c >> c >> u >> v;

				textureCoordinates.push_back(Vec2f(u, v));
				std::cout << "vt " << u << ' ' << v << '\n';
			}

			if (Serializer::DoesFilenameStartWith(line, "f ")) { // read a face, which should be a trianglular face, hence 3 vertices only
				std::cout << line << '\n';
				ss.clear();
				ss << line; // line would be "f vIdx/vtexIdx/vNormIdx"

				IShape::ElementIndex v1, vt1, vn1, v2, vt2, vn2, v3, vt3, vn3;
				ss >> c >> v1 >> c >> vt1 >> c >> vn1; // need to read '/', so c is put between the integers
				ss >> v2 >> c >> vt2 >> c >> vn2;
				ss >> v3 >> c >> vt3 >> c >> vn3;

				// the read indices start from 1, but in code, indices start from 0, hence got to shift down by 1
				v1--;
				v2--;
				v3--;
				vt1--;
				vt2--;
				vt3--;

				vertexIndices.insert(vertexIndices.end(), { v1, v2, v3 });

				vertices[v1].UV(textureCoordinates[vt1].x, textureCoordinates[vt1].y);
				vertices[v2].UV(textureCoordinates[vt2].x, textureCoordinates[vt2].y);
				vertices[v3].UV(textureCoordinates[vt3].x, textureCoordinates[vt3].y);
			}
		}
		shape->Vertices(vertices);
		shape->Indices(vertexIndices);
		shapes.emplace_back(shape);
		std::cout << "Shapes:" << shapes.size() << '\n' << std::endl;

		fs.close();
		Init(shapes);
	}
}