#include "custom.model.inc.h"
Vtx VB_ttm_geo_000DF4_0x702b570[] = {
{ 90, 360, 18, 0, 0, 2048, 0, 0, 127, 255 },
{ -90, 0, 18, 0, -1024, 0, 0, 0, 127, 255 },
{ 90, 0, 18, 0, 0, 0, 0, 0, 127, 255 },
{ -90, 0, 18, 0, -1024, 0, 0, 0, 127, 255 },
{ 90, 360, 18, 0, 0, 2048, 0, 0, 127, 255 },
{ -90, 360, 18, 0, -1024, 2048, 0, 0, 127, 255 },
{ 90, 360, 18, 0, 307, -64, 0, 127, 0, 255 },
{ -90, 360, -18, 0, -307, 64, 0, 127, 0, 255 },
{ -90, 360, 18, 0, -307, -64, 0, 127, 0, 255 },
{ -90, 360, -18, 0, -307, 64, 0, 127, 0, 255 },
{ 90, 360, 18, 0, 307, -64, 0, 127, 0, 255 },
{ 90, 360, -18, 0, 307, 64, 0, 127, 0, 255 },
{ 90, 360, 18, 0, -64, 1228, 127, 0, 0, 255 },
{ 90, 0, -18, 0, 64, 0, 127, 0, 0, 255 },
{ 90, 360, -18, 0, 64, 1228, 127, 0, 0, 255 },
};

Vtx VB_ttm_geo_000DF4_0x702b670[] = {
{ 90, 0, -18, 0, 64, 0, 127, 0, 0, 255 },
{ 90, 360, 18, 0, -64, 1228, 127, 0, 0, 255 },
{ 90, 0, 18, 0, -64, 0, 127, 0, 0, 255 },
{ -90, 0, 18, 0, 307, -64, 0, -127, 0, 255 },
{ 90, 0, -18, 0, -307, 64, 0, -127, 0, 255 },
{ 90, 0, 18, 0, -307, -64, 0, -127, 0, 255 },
{ 90, 0, -18, 0, -307, 64, 0, -127, 0, 255 },
{ -90, 0, 18, 0, 307, -64, 0, -127, 0, 255 },
{ -90, 0, -18, 0, 307, 64, 0, -127, 0, 255 },
{ -90, 0, 18, 0, 64, 0, -127, 0, 0, 255 },
{ -90, 360, -18, 0, -64, 1228, -127, 0, 0, 255 },
{ -90, 0, -18, 0, -64, 0, -127, 0, 0, 255 },
{ -90, 360, -18, 0, -64, 1228, -127, 0, 0, 255 },
{ -90, 0, 18, 0, 64, 0, -127, 0, 0, 255 },
{ -90, 360, 18, 0, 64, 1228, -127, 0, 0, 255 },
};

Vtx VB_ttm_geo_000DF4_0x702b770[] = {
{ -90, 360, -18, 0, 1024, 2048, 0, 0, -127, 255 },
{ 90, 0, -18, 0, 0, 0, 0, 0, -127, 255 },
{ -90, 0, -18, 0, 1024, 0, 0, 0, -127, 255 },
{ 90, 0, -18, 0, 0, 0, 0, 0, -127, 255 },
{ -90, 360, -18, 0, 1024, 2048, 0, 0, -127, 255 },
{ 90, 360, -18, 0, 0, 2048, 0, 0, -127, 255 },
{ 0, 180, 0, 0, 0, 0, 0, 0, 0, 255 },
{ 0, 180, 0, 0, 0, 0, 0, 0, 0, 255 },
{ 0, 180, 0, 0, 0, 0, 0, 0, 0, 255 },
{ 0, 180, 0, 0, 0, 0, 0, 0, 0, 255 },
{ 0, 180, 0, 0, 0, 0, 0, 0, 0, 255 },
{ 0, 180, 0, 0, 0, 0, 0, 0, 0, 255 },
{ 0, 180, 0, 0, 0, 0, 0, 0, 0, 255 },
{ 0, 180, 0, 0, 0, 0, 0, 0, 0, 255 },
{ 0, 180, 0, 0, 0, 0, 0, 0, 0, 255 },
};

extern Gfx DL_ttm_geo_000DF4_0x702b9d0[];
Gfx DL_ttm_geo_000DF4_0x702bb60[] = {
gsDPPipeSync(),
gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, SHADE, TEXEL0, 0, SHADE, 0, 0, 0, 0, SHADE),
gsSPGeometryMode(G_SHADING_SMOOTH, 0),
gsDPSetTile(0, 2, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0),
gsSPTexture(65535, 65535, 0, 0, 1),
gsDPTileSync(),
gsDPSetTile(0, 2, 8, 0, 0, 0, 0, 5, 0, 0, 5, 0),
gsDPSetTileSize(0, 0, 0, 124, 124),
gsSPDisplayList(DL_ttm_geo_000DF4_0x702b9d0),
gsSPTexture(65535, 65535, 0, 0, 0),
gsDPPipeSync(),
gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE, 0, 0, 0, SHADE),
gsSPGeometryMode(0, G_SHADING_SMOOTH),
gsSPEndDisplayList(),
};

ALIGNED8 u8 ttm_geo_000DF4__texture_09004800[] = {
#include "levels/wdw/custom_textures/09004800.rgba16.inc.c"
};
ALIGNED8 u8 ttm_geo_000DF4__texture_03021750[] = {
#include "levels/wdw/custom_textures/03021750.rgba16.inc.c"
};
ALIGNED8 u8 ttm_geo_000DF4__texture_03020750[] = {
#include "levels/wdw/custom_textures/03020750.rgba16.inc.c"
};
Gfx DL_ttm_geo_000DF4_0x702b9d0[] = {
gsSPLightColor(1, 0xFFFFFFFF),
gsSPLightColor(2, 0x7F7F7FFF),
gsSPVertex(VB_ttm_geo_000DF4_0x702b570, 15, 0),
gsDPSetTextureImage(0, 2, 1, ttm_geo_000DF4__texture_03020750),
gsDPSetTile(0, 2, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0),
gsDPLoadSync(),
gsDPLoadBlock(7, 0, 0, 2047, 256),
gsDPPipeSync(),
gsDPSetTile(0, 2, 8, 0, 0, 0, 0, 6, 0, 0, 5, 0),
gsDPSetTileSize(0, 0, 0, 124, 252),
gsSP2Triangles(0, 1, 2, 0,3, 4, 5, 0),
gsDPSetTextureImage(0, 2, 1, ttm_geo_000DF4__texture_09004800),
gsDPSetTile(0, 2, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0),
gsDPLoadSync(),
gsDPLoadBlock(7, 0, 0, 1023, 256),
gsDPPipeSync(),
gsDPSetTile(0, 2, 8, 0, 0, 0, 0, 5, 0, 0, 5, 0),
gsDPSetTileSize(0, 0, 0, 124, 124),
gsSP2Triangles(6, 7, 8, 0,9, 10, 11, 0),
gsSP1Triangle(12, 13, 14, 0),
gsSPVertex(VB_ttm_geo_000DF4_0x702b670, 15, 0),
gsSP2Triangles(0, 1, 2, 0,3, 4, 5, 0),
gsSP2Triangles(6, 7, 8, 0,9, 10, 11, 0),
gsSP1Triangle(12, 13, 14, 0),
gsSPVertex(VB_ttm_geo_000DF4_0x702b770, 15, 0),
gsDPSetTextureImage(0, 2, 1, ttm_geo_000DF4__texture_03021750),
gsDPSetTile(0, 2, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0),
gsDPLoadSync(),
gsDPLoadBlock(7, 0, 0, 2047, 256),
gsDPPipeSync(),
gsDPSetTile(0, 2, 8, 0, 0, 0, 0, 6, 0, 0, 5, 0),
gsDPSetTileSize(0, 0, 0, 124, 252),
gsSP2Triangles(0, 1, 2, 0,3, 4, 5, 0),
gsSP1Triangle(6, 7, 8, 0),
gsSPEndDisplayList(),
};

