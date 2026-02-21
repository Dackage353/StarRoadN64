#include <assert.h>
#include <stdio.h>
#include <stdint.h>

#include <algorithm>
#include <vector>
#include <optional>

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

// Collision Tri With Special Params
#define COL_TRI_SPECIAL(v1, v2, v3, param) v1, v2, v3, param

enum TerrainLoadCmd {
	TERRAIN_LOAD_VERTICES = 0x40, // Begins vertices list for collision triangles
	TERRAIN_LOAD_CONTINUE,        // Stop loading vertices but continues to load other collision commands
	TERRAIN_LOAD_END,             // End the collision list
	TERRAIN_LOAD_ENVIRONMENT      // Loads water/HMC gas
};

#define Collision int16_t

// #include "../levels/wdw/custom_c/custom.collision.inc.c"
// #define sCollision col_wdw_1_0xe090ee8

#include "../levels/ttc/custom_c/custom.collision.inc.c"
#define sCollision col_ttc_1_0xe03dd58

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
	std::optional<int16_t> force;
};

struct Pack
{
	int16_t type;
	std::vector<Tri> tris;
};

/* CCCandy
static const Vtx sBannedVertices[] = {
	Vtx{ { -623, 3181, 4850 } },
	Vtx{ { 1767, 3181, 5365 } },
};
*/

static const Vtx sBannedVertices[] = {
	{ { -2510, 3420, -987 } },
	{ { -2513, 3420, -5168 } },
	{ { 4699, 2520, 1076 } },
	{ { 4685, -756, 1071 } },
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
				if (pack.tris.empty())
					continue;

				printf("COL_TRI_INIT(%d, %d),\n", pack.type, (int) pack.tris.size());
				for (const auto& tri : pack.tris)
				{
					if (tri.force)
					{
						printf("COL_TRI_SPECIAL(%d, %d, %d, %d),\n", tri.v[0], tri.v[1], tri.v[2], *tri.force);
					}
					else
					{
						printf("COL_TRI(%d, %d, %d),\n", tri.v[0], tri.v[1], tri.v[2]);
					}
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
			std::optional<int16_t> force;
			if (44 == type)
			{
				force = *collisionData++;
			}

			const Vtx& v1 = vertices[i1];
			const Vtx& v2 = vertices[i2];
			const Vtx& v3 = vertices[i3];

			if (!skip(v1) && !skip(v2) && !skip(v3))
			{
				Tri tri{ { i1, i2, i3 }, force };
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
