#pragma once
#include "Shape.h"

namespace OpenGLFun {
	class BasicShape : public IShape {
	public:
		BasicShape();
		~BasicShape() override;
		void Deserialize(rapidjson::Value const&) override;
	};
}