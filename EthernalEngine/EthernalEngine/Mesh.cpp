#include "Mesh.h"
#include <iostream>

namespace EthernalEngine {

	Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
	{
		this->vertices = vertices;
		this->indices = indices;

		SetupMesh();
	}

	Mesh::~Mesh()
	{
		if (VAO != 0) glDeleteVertexArrays(1, &VAO);
		if (VBO != 0) glDeleteBuffers(1, &VBO);
		if (EBO != 0) glDeleteBuffers(1, &EBO);
	}

	void Mesh::Draw()
	{
		glBindVertexArray(VAO);

		if (!indices.empty())
		{
			glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
		}
		else
		{
			glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));
		}

		glBindVertexArray(0);
	}
	
	void Mesh::SetupMesh()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		if (!indices.empty())
		{
			glGenBuffers(1, &EBO);
		}

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		if (!indices.empty())
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), 
				GL_STATIC_DRAW);
		}

		// Position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		glEnableVertexAttribArray(0);

		// Normal
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(1);

		// UV
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);
	}
}