#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/stat.h>

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

#define OUTPUT_DIR "./obj"
#define OUTPUT_NAME "hmc"
#include "../levels/hmc/custom_c/custom.collision.inc.c"
#define sCollision col_hmc_1_0xe048a28

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

struct Color
{
    float r, g, b;
};

static Color hsv2rgb(float H, float S, float V) {
	float r, g, b;
	
	float h = H / 360;
	float s = S / 100;
	float v = V / 100;
	
	int i = (h * 6);
	float f = h * 6 - i;
	float p = v * (1 - s);
	float q = v * (1 - f * s);
	float t = v * (1 - (1 - f) * s);
	
	switch (i % 6) {
		case 0: r = v, g = t, b = p; break;
		case 1: r = q, g = v, b = p; break;
		case 2: r = p, g = v, b = t; break;
		case 3: r = p, g = q, b = v; break;
		case 4: r = t, g = p, b = v; break;
		case 5: r = v, g = p, b = q; break;
	}
	
	return { r, g, b };
}

int main()
{
    mkdir(OUTPUT_DIR, 0755);
    FILE* fobj = fopen(OUTPUT_DIR "/" OUTPUT_NAME ".obj", "w");
    FILE* fmtl = fopen(OUTPUT_DIR "/" OUTPUT_NAME ".mtl", "w");

	int16_t* collisionData = (int16_t*)sCollision;
	{
		int16_t init = *collisionData++;
		assert(init == TERRAIN_LOAD_VERTICES);
        fprintf(fobj, "mtllib " OUTPUT_NAME ".mtl" "\n");
	}

	std::vector<Vtx> vertices;

	int16_t numVertices;
	{
		numVertices = *collisionData++;
	}

	vertices.reserve(numVertices);
	{
		for (int i = 0; i < numVertices; i++)
		{
			int16_t x = *collisionData++;
			int16_t y = *collisionData++;
			int16_t z = *collisionData++;
            fprintf(fobj, "v %d %d %d\n", x, y, z);

			vertices.push_back(Vtx{ { x, y, z } });
		}
	}

	std::vector<Pack> packs;

	while (1)
	{
		int16_t type = *collisionData++;
		if (TERRAIN_LOAD_CONTINUE == type)
		{
            int num = -1;
			for (auto& pack : packs)
			{
                num++;
				if (pack.tris.empty())
					continue;

                float h = num * 360.f / packs.size();
                Color c = hsv2rgb(h, 100, 100);

                fprintf(fobj, "usemtl mat%d\n", pack.type);
                fprintf(fmtl, "newmtl mat%d\n", pack.type);
                fprintf(fmtl, "Ka %f %f %f\n", pack.type, c.r, c.g, c.b);
				for (const auto& tri : pack.tris)
				{
                    fprintf(fobj, "f %d %d %d\n", 1+tri.v[0], 1+tri.v[1], 1+tri.v[2]);
				}
			}

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

            Tri tri{ { i1, i2, i3 }, force };
            packs.back().tris.push_back(tri);
		}
	}

	{
		int16_t end = *collisionData++;
		assert(end == TERRAIN_LOAD_END);
	}

    fclose(fobj);
    fclose(fmtl);
}
