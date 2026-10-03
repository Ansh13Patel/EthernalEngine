#include "Rendering/SphereMesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <vector>
#include <cmath>

namespace EthernalEngine
{
	SphereMesh::SphereMesh(float radius, unsigned int segments, unsigned int rings) : Mesh(
		[&]()
		{
			std::vector<Vertex> vertices;

			for (unsigned int ring = 0; ring <= rings; ++ring)
			{
				float v =
					static_cast<float>(ring) / rings;

				float phi =
					v * glm::pi<float>();

				for (unsigned int segment = 0;
					segment <= segments;
					++segment)
				{
					float u =
						static_cast<float>(segment) / segments;

					float theta =
						u * glm::two_pi<float>();

					float x =
						std::sin(phi) *
						std::cos(theta);

					float y =
						std::cos(phi);

					float z =
						std::sin(phi) *
						std::sin(theta);

					glm::vec3 normal(x, y, z);

					glm::vec3 position =
						normal * radius;

					vertices.push_back(
						{
							position,
							normal,
							{u, v}
						});
				}
			}

			return vertices;
		}(),

			[&]()
			{
				std::vector<unsigned int> indices;

				for (unsigned int ring = 0;
					ring < rings;
					++ring)
				{
					for (unsigned int segment = 0;
						segment < segments;
						++segment)
					{
						unsigned int current =
							ring * (segments + 1) + segment;

						unsigned int next =
							current + segments + 1;

						indices.push_back(current);
						indices.push_back(next);
						indices.push_back(current + 1);

						indices.push_back(current + 1);
						indices.push_back(next);
						indices.push_back(next + 1);
					}
				}

				return indices;
			}()
				)
	{
	}
}