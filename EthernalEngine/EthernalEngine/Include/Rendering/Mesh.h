#pragma once

#include "Rendering/Texture.h"

#include <vector>
#include <memory>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <assimp/texture.h>

enum DefaultMeshType
{
    None,
    Cube,
    Sphere,
    Cylinder,
    Plane
};
namespace EthernalEngine
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoord;
    };

    class Mesh
    {
    public:
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
        ~Mesh();
        virtual void Draw();

    protected:

        void SetupMesh();

        unsigned int VAO = 0;
        unsigned int VBO = 0;
        unsigned int EBO = 0;

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
    };
}