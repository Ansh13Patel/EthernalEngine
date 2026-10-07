#include "Rendering/PlaneMesh.h"

namespace EthernalEngine
{
	PlaneMesh::PlaneMesh(float width, float height, float thickness) : Mesh(
		GenerateVertices(width, height, thickness),
		GenerateIndices())
	{

	}

	std::vector<Vertex> PlaneMesh::GenerateVertices(float width, float height, float thickness)
	{
		float halfWidth = width * 0.5f;
		float halfHeight = height * 0.5f;
		float halfThickness = thickness * 0.5f;

		return
		{
			// =========================
			// Top
			// =========================

			{{-halfWidth,  halfThickness, -halfHeight},
				{0, 1, 0}, {0, 0}},

			{{ halfWidth,  halfThickness, -halfHeight},
				{0, 1, 0}, {1, 0}},

			{{ halfWidth,  halfThickness,  halfHeight},
				{0, 1, 0}, {1, 1}},

			{{-halfWidth,  halfThickness,  halfHeight},
				{0, 1, 0}, {0, 1}},


				// =========================
				// Bottom
				// =========================

				{{-halfWidth, -halfThickness,  halfHeight},
					{0, -1, 0}, {0, 1}},

				{{ halfWidth, -halfThickness,  halfHeight},
					{0, -1, 0}, {1, 1}},

				{{ halfWidth, -halfThickness, -halfHeight},
					{0, -1, 0}, {1, 0}},

				{{-halfWidth, -halfThickness, -halfHeight},
					{0, -1, 0}, {0, 0}},


					// =========================
					// Front (+Z)
					// =========================

					{{-halfWidth,  halfThickness, halfHeight},
						{0, 0, 1}, {0, 1}},

					{{ halfWidth,  halfThickness, halfHeight},
						{0, 0, 1}, {1, 1}},

					{{ halfWidth, -halfThickness, halfHeight},
						{0, 0, 1}, {1, 0}},

					{{-halfWidth, -halfThickness, halfHeight},
						{0, 0, 1}, {0, 0}},


						// =========================
						// Back (-Z)
						// =========================

						{{ halfWidth,  halfThickness, -halfHeight},
							{0, 0, -1}, {1, 1}},

						{{-halfWidth,  halfThickness, -halfHeight},
							{0, 0, -1}, {0, 1}},

						{{-halfWidth, -halfThickness, -halfHeight},
							{0, 0, -1}, {0, 0}},

						{{ halfWidth, -halfThickness, -halfHeight},
							{0, 0, -1}, {1, 0}},


							// =========================
							// Right (+X)
							// =========================

							{{halfWidth,  halfThickness, -halfHeight},
								{1, 0, 0}, {1, 1}},

							{{halfWidth, -halfThickness, -halfHeight},
								{1, 0, 0}, {1, 0}},

							{{halfWidth, -halfThickness,  halfHeight},
								{1, 0, 0}, {0, 0}},

							{{halfWidth,  halfThickness,  halfHeight},
								{1, 0, 0}, {0, 1}},


								// =========================
								// Left (-X)
								// =========================

								{{-halfWidth,  halfThickness,  halfHeight},
									{-1, 0, 0}, {1, 1}},

								{{-halfWidth, -halfThickness,  halfHeight},
									{-1, 0, 0}, {1, 0}},

								{{-halfWidth, -halfThickness, -halfHeight},
									{-1, 0, 0}, {0, 0}},

								{{-halfWidth,  halfThickness, -halfHeight},
									{-1, 0, 0}, {0, 1}}
		};
	}

	std::vector<unsigned int> PlaneMesh::GenerateIndices()
	{
		return
		{
			// Top
			0, 1, 2,
			2, 3, 0,
			// Bottom
			4, 5, 6,
			6, 7, 4,
			// Front
			8, 9, 10,
			10, 11, 8,
			// Back
			12, 13, 14,
			14, 15, 12,
			// Right
			16, 17, 18,
			18, 19, 16,
			// Left
			20, 21, 22,
			22, 23, 20
		};
	}

}