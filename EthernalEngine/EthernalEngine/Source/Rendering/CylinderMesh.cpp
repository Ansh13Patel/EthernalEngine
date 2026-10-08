#include "Rendering/CylinderMesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <vector>
#include <cmath>

namespace EthernalEngine
{
	CylinderMesh::CylinderMesh(float radius, float height, int segments) : Mesh(
		[&]()
		{
            std::vector<Vertex> vertices;

            float halfHeight = height * 0.5f;

            // ========================================
            // Side vertices
            // ========================================

            for (unsigned int i = 0; i <= segments; ++i)
            {
                float u =
                    static_cast<float>(i) / segments;

                float theta =
                    u * glm::two_pi<float>();

                float x =
                    std::cos(theta) * radius;

                float z =
                    std::sin(theta) * radius;

                glm::vec3 normal =
                    glm::normalize(glm::vec3(x, 0.0f, z));

                // Bottom
                vertices.push_back(
                    {
                        {x, -halfHeight, z},
                        normal,
                        {u, 0.0f}
                    });

                // Top
                vertices.push_back(
                    {
                        {x, halfHeight, z},
                        normal,
                        {u, 1.0f}
                    });
            }


            // ========================================
            // Top cap
            // ========================================

            unsigned int topCenterIndex =
                static_cast<unsigned int>(vertices.size());

            vertices.push_back(
                {
                    {0.0f, halfHeight, 0.0f},
                    {0, 1, 0},
                    {0.5f, 0.5f}
                });

            for (unsigned int i = 0; i <= segments; ++i)
            {
                float u =
                    static_cast<float>(i) / segments;

                float theta =
                    u * glm::two_pi<float>();

                float x =
                    std::cos(theta) * radius;

                float z =
                    std::sin(theta) * radius;

                vertices.push_back(
                    {
                        {x, halfHeight, z},
                        {0, 1, 0},
                        {
                            0.5f + x / (2.0f * radius),
                            0.5f + z / (2.0f * radius)
                        }
                    });
            }


            // ========================================
            // Bottom cap
            // ========================================

            unsigned int bottomCenterIndex =
                static_cast<unsigned int>(vertices.size());

            vertices.push_back(
                {
                    {0.0f, -halfHeight, 0.0f},
                    {0, -1, 0},
                    {0.5f, 0.5f}
                });

            for (unsigned int i = 0; i <= segments; ++i)
            {
                float u =
                    static_cast<float>(i) / segments;

                float theta =
                    u * glm::two_pi<float>();

                float x =
                    std::cos(theta) * radius;

                float z =
                    std::sin(theta) * radius;

                vertices.push_back(
                    {
                        {x, -halfHeight, z},
                        {0, -1, 0},
                        {
                            0.5f + x / (2.0f * radius),
                            0.5f + z / (2.0f * radius)
                        }
                    });
            }

            return vertices;
		}(),
        [&]()
        {
            std::vector<unsigned int> indices;


            // ========================================
            // Sides
            // ========================================

            for (unsigned int i = 0; i < segments; ++i)
            {
                unsigned int current = i * 2;
                unsigned int next = (i + 1) * 2;

                // First triangle
                indices.push_back(current);
                indices.push_back(next);
                indices.push_back(current + 1);

                // Second triangle
                indices.push_back(current + 1);
                indices.push_back(next);
                indices.push_back(next + 1);
            }


            // ========================================
            // Top cap
            // ========================================

            unsigned int topCenter =
                2 * (segments + 1);

            unsigned int topStart =
                topCenter + 1;

            for (unsigned int i = 0; i < segments; ++i)
            {
                indices.push_back(topCenter);
                indices.push_back(topStart + i);
                indices.push_back(topStart + i + 1);
            }


            // ========================================
            // Bottom cap
            // ========================================

            unsigned int bottomCenter =
                topStart + (segments + 1);

            unsigned int bottomStart =
                bottomCenter + 1;

            for (unsigned int i = 0; i < segments; ++i)
            {
                indices.push_back(bottomCenter);
                indices.push_back(bottomStart + i + 1);
                indices.push_back(bottomStart + i);
            }

            return indices;
        }()
	) {}
}