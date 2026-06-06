#pragma once

#include <vector>
#include <glad/glad.h>

namespace EthernalEngine
{
    class Mesh
    {
    public:
        Mesh() = default;
        virtual ~Mesh();
        virtual void Draw() = 0;

    protected:

        void SetupMesh();

        unsigned int VAO = 0;
        unsigned int VBO = 0;
        unsigned int EBO = 0;

        std::vector<float> vertices;
        std::vector<unsigned int> indices;
    };
}