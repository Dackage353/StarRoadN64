#include "game/water.h"

extern Gfx *geo_star_road_cull(s32 callContext, struct GraphNode *node, UNUSED Mat4 mtx);

extern Gfx DL_ending_1_0xe082470_m[];
extern Gfx DL_ending_1_0xe063110_m[];

const GeoLayout Geo_ending_1_0x2461700[]= {
GEO_NODE_SCREEN_AREA(10,160,120,160,120),
GEO_OPEN_NODE(),
GEO_ZBUFFER(0),
GEO_OPEN_NODE(),
GEO_NODE_ORTHO(100),
GEO_OPEN_NODE(),
GEO_BACKGROUND(SkyboxCustom38895296_skybox_Index+10, geo_skybox_main),
GEO_CLOSE_NODE(),
GEO_CLOSE_NODE(),
GEO_ZBUFFER(1),
GEO_OPEN_NODE(),
GEO_CAMERA_FRUSTUM_WITH_FUNC(45,100,30000, geo_camera_fov),
GEO_OPEN_NODE(),
GEO_CAMERA(16,0,2000,6000,3072,0,60928, geo_camera_main),
GEO_OPEN_NODE(),
GEO_ASM(18, geo_star_road_cull),
GEO_DISPLAY_LIST(LAYER_OPAQUE,DL_ending_1_0xe063110),
GEO_ASM(18, geo_star_road_cull),
GEO_DISPLAY_LIST(LAYER_ALPHA,DL_ending_1_0xe082470),
GEO_ASM(17, geo_star_road_cull),
GEO_DISPLAY_LIST(LAYER_OPAQUE,DL_ending_1_0xe063110_m),
GEO_ASM(17, geo_star_road_cull),
GEO_DISPLAY_LIST(LAYER_ALPHA,DL_ending_1_0xe082470_m),
GEO_RENDER_OBJ(),
GEO_ASM(0, geo_envfx_main),
GEO_ASM(WATER_ENDING, geo_render_water_dl),
GEO_CLOSE_NODE(),
GEO_CLOSE_NODE(),
GEO_CLOSE_NODE(),
GEO_ZBUFFER(0),
GEO_OPEN_NODE(),
GEO_ASM(0, geo_cannon_circle_base),
GEO_CLOSE_NODE(),
GEO_CLOSE_NODE(),
GEO_END(),
};
