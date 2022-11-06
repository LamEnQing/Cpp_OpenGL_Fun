#pragma once
#include "Shape.h"

namespace OpenGLFun {
	class BasicShape : public IShape {
	public:
		BasicShape();
		BasicShape(std::vector<Vertex> vertices, std::vector<ElementIndex> indices);
		~BasicShape() override;
		void Deserialize(rapidjson::Value const&) override;
	};
}