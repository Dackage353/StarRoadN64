#include "custom.model.inc.h"
Vtx VB_rr_geo_000908_0x3032048[] = {
{{{ 128, 512, 0 }, 0, { 990, 0 }, { 0, 0, 127, 255}}},
{{{ -127, 512, 0 }, 0, { 0, 0 }, { 0, 0, 127, 255}}},
{{{ -127, 0, 0 }, 0, { 0, 2012 }, { 0, 0, 127, 255}}},
{{{ 128, 0, 0 }, 0, { 990, 2012 }, { 0, 0, 127, 255}}},
};

ALIGNED8 u8 rr_geo_000908__texture_0300BD10[] = {
#include "levels/ttm/custom_textures/0300BD10.rgba16.inc.c"
};
Gfx DL_rr_geo_000908_0x701b0c8[] = {
gsDPPipeSync(),
gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0),
gsSPGeometryMode(G_SHADING_SMOOTH, 0),
gsDPSetTile(0, 2, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0),
gsSPTexture(65535, 65535, 0, 0, 1),
gsDPTileSync(),
gsDPSetTile(0, 2, 8, 0, 0, 0, 2, 6, 0, 2, 5, 0),
gsDPSetTileSize(0, 0, 0, 124, 252),
gsDPSetTextureImage(0, 2, 1, rr_geo_000908__texture_0300BD10),
gsDPLoadSync(),
gsDPLoadBlock(7, 0, 0, 2047, 256),
gsSPLightColor(1, 0xFFFFFFFF),
gsSPLightColor(2, 0x7F7F7FFF),
gsSPVertex(VB_rr_geo_000908_0x3032048, 4, 0),
gsSP2Triangles(0, 1, 2, 0,0, 2, 3, 0),
gsSPTexture(65535, 65535, 0, 0, 0),
gsDPPipeSync(),
gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
gsSPGeometryMode(0, G_SHADING_SMOOTH),
gsSPEndDisplayList(),
};

