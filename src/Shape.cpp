#include "Shape.h"
#include "BasicShape.h"
#include "CuboidShape.h"

namespace OpenGLFun {
	IShape::IShape() : _vertices{}, _indices{} {}
	IShape::IShape(std::vector<Vertex> vertices, std::vector<ElementIndex> indices) : _vertices(vertices), _indices(indices) {}
	IShape::~IShape() {}

	const std::vector<Vertex>& IShape::Vertices() const { return _vertices; }
	std::vector<IShape::ElementIndex>& IShape::Indices() { return _indices; }

	IShape* IShape::Vertices(std::vector<Vertex>& vertices) {
		this->_vertices = vertices;
		return this;
	}
	IShape* IShape::Indices(std::vector<IShape::ElementIndex>& indices) {
		this->_indices = indices;
		return this;
	}

	ShapeManager::ShapeManager() : _mapShapeData{} {
		if (SHAPE_MANAGER != nullptr)
			throw SimpleException("Shape manager already created!");

		_mapShapeData.insert({ "basic", std::shared_ptr<IShapeCreator>(new ShapeCreator<BasicShape>()) });
		_mapShapeData.insert({ "cuboid", std::shared_ptr<IShapeCreator>(new ShapeCreator<CuboidShape>()) });
	}
	ShapeManager::~ShapeManager() { _mapShapeData.clear(); }

	std::shared_ptr<IShapeCreator> ShapeManager::GetShapeCreator(std::string dataName) {
		if (_mapShapeData.find(dataName) == _mapShapeData.end()) {
			throw SimpleException(std::string("Could not find \"") + dataName + "\" in shape database");
		}

		return _mapShapeData.find(dataName)->second;
	}
}