#pragma once
#include "pch.h"
#include "Vertex.h"
#include "Vec4f.h"

namespace OpenGLFun {
	class IShape {
	public:
		using ElementIndex = unsigned int;

		IShape();
		virtual ~IShape();

		const std::vector<Vertex>& Vertices() const;
		std::vector<ElementIndex>& Indices();

		virtual void Deserialize(rapidjson::Value const& jsonObj) = 0;
	protected:
		std::vector<Vertex> _vertices;
		std::vector<ElementIndex> _indices;
	};

	class IShapeCreator {
	public:
		IShapeCreator() {}
		virtual ~IShapeCreator() {}
		virtual IShape* Create() = 0;
	};

	template<typename T>
	class ShapeCreator : public IShapeCreator {
	public:
		ShapeCreator() {}
		~ShapeCreator() {}
		IShape* Create() override {
			return new T();
		}
	};

	class ShapeManager {
	public:
		ShapeManager();
		~ShapeManager();

		template<typename T>
		void RegisterShapeData(std::string dataName, ShapeCreator<T>* creator) {
			_mapShapeData.insert({ dataName, std::shared_ptr<IShapeCreator>(creator) });
		}
		std::shared_ptr<IShapeCreator> GetShapeCreator(std::string dataname);
	private:
		std::map<std::string, std::shared_ptr<IShapeCreator>> _mapShapeData;
	};
	extern ShapeManager* SHAPE_MANAGER;
}