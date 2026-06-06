#include "CubeMesh.h"

namespace EthernalEngine
{
	CubeMesh::CubeMesh()
	{
		vertices = {
			0.5f,  0.5f, 0.5f,     1.0f, 1.0f,
	       -0.5f, 0.5f,  0.5f,     0.0f, 1.0f,
	       -0.5f,-0.5f,  0.5f,     0.0f, 0.0f,

		    0.5f,  0.5f, 0.5f,     1.0f, 1.0f,
	       -0.5f,-0.5f,  0.5f,     0.0f, 0.0f,
		    0.5f, -0.5f, 0.5f,     1.0f, 0.0f,

		    0.5f,  0.5f, -0.5f,     1.0f, 1.0f,
	       -0.5f, 0.5f,  -0.5f,     0.0f, 1.0f,
	       -0.5f,-0.5f,  -0.5f,     0.0f, 0.0f,

		    0.5f,  0.5f, -0.5f,     1.0f, 1.0f,
	       -0.5f,-0.5f,  -0.5f,     0.0f, 0.0f,
		    0.5f, -0.5f, -0.5f,     1.0f, 0.0f,

		    0.5f,  0.5f, -0.5f,     1.0f, 1.0f,
	       -0.5f, 0.5f,  -0.5f,     0.0f, 1.0f,
	       -0.5f, 0.5f,   0.5f,     0.0f, 0.0f,

	        0.5f,  0.5f, -0.5f,     1.0f, 1.0f,
	       -0.5f, 0.5f,   0.5f,     0.0f, 0.0f,
	        0.5f,  0.5f,  0.5f,     1.0f, 0.0f,

	        0.5f, -0.5f, -0.5f,     1.0f, 1.0f,
	       -0.5f,-0.5f,  -0.5f,     0.0f, 1.0f,
	       -0.5f,-0.5f,   0.5f,     0.0f, 0.0f,

	        0.5f, -0.5f, -0.5f,     1.0f, 1.0f,
	       -0.5f,-0.5f,   0.5f,     0.0f, 0.0f,
	        0.5f, -0.5f,  0.5f,     1.0f, 0.0f,

	        0.5f,  0.5f, -0.5f,     1.0f, 1.0f,
	        0.5f, -0.5f, -0.5f,     0.0f, 1.0f,
	        0.5f, -0.5f,  0.5f,     0.0f, 0.0f,

	        0.5f,  0.5f, -0.5f,     1.0f, 1.0f,
	        0.5f, -0.5f,  0.5f,     0.0f, 0.0f,
	        0.5f,  0.5f,  0.5f,     1.0f, 0.0f,

	       -0.5f,  0.5f, -0.5f,     1.0f, 1.0f,
	       -0.5f, -0.5f, -0.5f,     0.0f, 1.0f,
	       -0.5f, -0.5f,  0.5f,     0.0f, 0.0f,

	       -0.5f,  0.5f, -0.5f,     1.0f, 1.0f,
	       -0.5f, -0.5f,  0.5f,     0.0f, 0.0f,
	       -0.5f,  0.5f,  0.5f,     1.0f, 0.0f
		};

		SetupMesh();
	}

	void CubeMesh::Draw()
	{
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size() / 5));
		glBindVertexArray(0);
	}
}