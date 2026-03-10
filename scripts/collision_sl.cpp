#include <assert.h>
#include <stdio.h>
#include <stdint.h>

#include <algorithm>
#include <vector>

// Collision Data Routine Initiate
#define COL_INIT() TERRAIN_LOAD_VERTICES

// Collision Vertices Read Initiate
#define COL_VERTEX_INIT(vtxNum) vtxNum

// Collision Vertex
#define COL_VERTEX(x, y, z) x, y, z

// Collision Tris Initiate
#define COL_TRI_INIT(surfType, triNum) surfType, triNum

// Collision Tri
#define COL_TRI(v1, v2, v3) v1, v2, v3

// Collision Tris Stop Loading
#define COL_TRI_STOP() TERRAIN_LOAD_CONTINUE

// End Collision Data
#define COL_END() TERRAIN_LOAD_END

enum TerrainLoadCmd {
	TERRAIN_LOAD_VERTICES = 0x40, // Begins vertices list for collision triangles
	TERRAIN_LOAD_CONTINUE,        // Stop loading vertices but continues to load other collision commands
	TERRAIN_LOAD_END,             // End the collision list
	TERRAIN_LOAD_ENVIRONMENT      // Loads water/HMC gas
};

#define Collision int16_t

#include "../levels/sl/custom_c/custom.collision.inc.c"
#define sCollision col_sl_1_0xe02ec20

struct Vtx
{
	int16_t v[3];

	Vtx operator-(const Vtx& other) const
	{
		return Vtx{ { (int16_t)(v[0] - other.v[0])
					, (int16_t)(v[1] - other.v[1])
					, (int16_t)(v[2] - other.v[2]) } };
	}

	Vtx operator+(const Vtx& other) const
	{
		return Vtx{ { (int16_t)(v[0] + other.v[0])
					, (int16_t)(v[1] + other.v[1])
					, (int16_t)(v[2] + other.v[2]) } };
	}

	int len2() const
	{
		return v[0] * v[0]
		     + v[1] * v[1]
			 + v[2] * v[2];
	}
};

struct Tri
{
	int16_t v[3];

	int32_t prio;
};

struct Pack
{
	int16_t type;
	std::vector<Tri> tris;
};

static const Vtx sBannedVertices[] = {
	{ { -2919, 5807, 4612 } },
	{ { -3941, 5807, 3988 } },
	{ { -8655, 5807, 3528 } },
};

static bool skip(const Vtx& v)
{
	for (const auto& banned : sBannedVertices)
	{
		Vtx diff = v - banned;
		int len = diff.len2();
		if (len < 100 * 100)
		{
			printf("// skipping vertex %d, %d, %d\n", v.v[0], v.v[1], v.v[2]);
			return true;
		}
	}

	return false;
}

static int32_t calculate_priority(const Vtx& v1, const Vtx& v2, const Vtx& v3)
{
	const int16_t ys[] = { v1.v[1], v2.v[1], v3.v[1] };
	int16_t maxY = *std::max_element(ys, ys + sizeof(ys) / sizeof(ys[0]));

	int32_t vec0[] = { int32_t(v2.v[0] - v1.v[0]), int32_t(v2.v[1] - v1.v[1]), int32_t(v2.v[2] - v1.v[2]) };
	int32_t vec1[] = { int32_t(v3.v[0] - v1.v[0]), int32_t(v3.v[1] - v1.v[1]), int32_t(v3.v[2] - v1.v[2]) };

	int32_t ny = vec0[2] * vec1[0] - vec0[0] * vec1[2];
	return ny > 0 ? maxY : -maxY - 32000;
}

int main()
{
//	for (int i = 0; i < sizeof(sCollision) / 2; i++)
//	{
//		std::swap(sCollision[2*i], sCollision[2*i+1]);
//	}

	int16_t* collisionData = (int16_t*)sCollision;
	{
		int16_t init = *collisionData++;
		assert(init == TERRAIN_LOAD_VERTICES);
		printf("COL_INIT(),\n");
	}

	std::vector<Vtx> vertices;

	int16_t numVertices;
	{
		numVertices = *collisionData++;
		printf("COL_VERTEX_INIT(%d),\n", numVertices);
	}

	vertices.reserve(numVertices);
	{
		for (int i = 0; i < numVertices; i++)
		{
			int16_t x = *collisionData++;
			int16_t y = *collisionData++;
			int16_t z = *collisionData++;
			printf("COL_VERTEX(%d, %d, %d),\n", x, y, z);

			vertices.push_back(Vtx{ { x, y, z } });
		}
	}

	std::vector<Pack> packs;
	packs.push_back(Pack{ 21, {} });

	while (1)
	{
		int16_t type = *collisionData++;
		if (TERRAIN_LOAD_CONTINUE == type)
		{
			for (auto& pack : packs)
			{
				std::sort(pack.tris.begin(), pack.tris.end(),
					[](const Tri& a, const Tri& b)
					{
						return a.prio < b.prio;
					});

				if (pack.tris.empty())
					continue;

				printf("COL_TRI_INIT(%d, %d),\n", pack.type, (int) pack.tris.size());
				for (const auto& tri : pack.tris)
				{
					printf("COL_TRI(%d, %d, %d),\n", tri.v[0], tri.v[1], tri.v[2]);
				}
			}

			printf("COL_TRI_STOP(),\n");
			break;
		}

		packs.push_back(Pack{ type, {} });

		int16_t count = *collisionData++;
		for (int i = 0; i < count; i++)
		{
			int16_t i1 = *collisionData++;
			int16_t i2 = *collisionData++;
			int16_t i3 = *collisionData++;

			const Vtx& v1 = vertices[i1];
			const Vtx& v2 = vertices[i2];
			const Vtx& v3 = vertices[i3];

			if (!skip(v1) && !skip(v2) && !skip(v3))
			{
				Tri tri{ { i1, i2, i3 } };
				packs.back().tris.push_back(tri);
			}
			else
			{
				printf("// skipping triangle %d, %d, %d\n", i1, i2, i3);
			}
		}
	}

	{
		int16_t end = *collisionData++;
		assert(end == TERRAIN_LOAD_END);
		printf("COL_END(),\n");
	}
}
