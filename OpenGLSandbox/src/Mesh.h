#pragma once

#include <glm/glm.hpp>
#include <glm/vec2.hpp>

#include <cmath> // sin, cos
#include <vector>

#include "GlobalDeclared.h"
#include "Vertex.h"

namespace OpenGLSandbox {
	// A mesh can be a quad, a circle, a triangle, whatever kind of shape. But at the base level, these shapes are comprised of m_vertices or points.
	class Mesh {
	public:
		std::vector<VertexWrapper> m_vertices;

		float m_position[2]; // for now, is all in 2D, so only need x and y
		float m_rotation;
		float m_scale[2];

		Mesh(std::vector<VertexWrapper>&& _vertices, glm::vec2&& position, glm::vec2&& scale, float rotation) : m_vertices(std::move(_vertices)), m_position{ position[0], position[1] }, m_scale{ scale[0], scale[1] }, m_rotation{ rotation } {
			ApplyTransformation(); // need to apply transformations to the position
		}
		~Mesh() {}

		void SetPosition(float x, float y) {
			m_position[0] = x;
			m_position[1] = y;

			ApplyTransformation();
		}

		void SetRotation(float rotation) {
			m_rotation = rotation;

			ApplyTransformation();
		}

		void SetScale(float width, float height) {
			m_scale[0] = width;
			m_scale[1] = height;

			ApplyTransformation();
		}

		void SetTransforms(float posX, float posY, float rotation, float width, float height) {
			m_position[0] = posX;
			m_position[1] = posY;
			m_rotation = rotation;
			m_scale[0] = width;
			m_scale[1] = height;
			
			ApplyTransformation();
		}

		void ApplyTransformation() {
			for (size_t i = 0; i < m_vertices.size(); i++) {
				VertexWrapper& vertWrap = m_vertices.at(i);

				// Apply scaling
				vertWrap.vertex.position[0] = vertWrap.m_position[0] * m_scale[0];
				vertWrap.vertex.position[1] = vertWrap.m_position[1] * m_scale[1];


				// Apply rotation
				float x = vertWrap.vertex.position[0];
				float y = vertWrap.vertex.position[1];
				float rotRadians = glm::radians(m_rotation);
				float cosVal = cos(rotRadians), sinVal = sin(rotRadians);

				vertWrap.vertex.position[0] = x * cosVal - y * sinVal;
				vertWrap.vertex.position[1] = x * sinVal + y * cosVal;


				// Apply translation
				vertWrap.vertex.position[0] += m_position[0];
				vertWrap.vertex.position[1] += m_position[1];

				// Normalise the coordinates to range -1 and 1
				vertWrap.vertex.position[0] /= SCREEN_WIDTH / 2.0f;
				vertWrap.vertex.position[1] /= SCREEN_HEIGHT / 2.0f;
			}
		}
	};
}