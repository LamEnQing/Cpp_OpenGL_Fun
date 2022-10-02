#pragma once
#include "Shape.h"

namespace OpenGLFun {
	class CuboidShape : public IShape {
	public:
		CuboidShape();
		~CuboidShape() override;
		void Deserialize(rapidjson::Value const&) override;
	};
}