#include "Shape.h"
#include "BasicShape.h"
#include "CuboidShape.h"

namespace OpenGLFun {
	IShape::IShape() : _vertices(), _indices{} {}
	IShape::~IShape() {}

	const std::vector<Vertex>& IShape::Vertices() const { return _vertices; }
	std::vector<IShape::ElementIndex>& IShape::Indices() { return _indices; }

	ShapeManager::ShapeManager() : _mapShapeData{} {
		if (SHAPE_MANAGER != nullptr)
			throw SimpleException("Shape manager already created!");

		_mapShapeData.insert({ "basic", std::shared_ptr<IShapeCreator>(new ShapeCreator<BasicShape>()) });
	}
	ShapeManager::~ShapeManager() { _mapShapeData.clear(); }

	std::shared_ptr<IShapeCreator> ShapeManager::GetShapeCreator(std::string dataName) {
		if (_mapShapeData.find(dataName) == _mapShapeData.end()) {
			throw SimpleException(std::string("Could not find \"") + dataName + "\" in shape database");
		}

		return _mapShapeData.find(dataName)->second;
	}
}