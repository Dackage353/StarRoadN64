#include <ultra64.h>
#include "sm64.h"
#include "behavior_data.h"
#include "model_ids.h"
#include "seq_ids.h"
#include "dialog_ids.h"
#include "segment_symbols.h"
#include "level_commands.h"

#include "game/level_update.h"

#include "levels/scripts.h"

#include "actors/common1.h"
#include "make_const_nonconst.h"
#include "levels/vcutm/header.h"

/* Fast64 begin persistent block [scripts] */
#include "levels/vcutm/custom_c/custom.collision.inc.c"
#define vcutm_area_1_collision col_vcutm_1_0xe024188
extern const GeoLayout Geo_vcutm_1_0x1e41700[];
#define vcutm_area_1 Geo_vcutm_1_0x1e41700

extern const GeoLayout star_road_vc_light[];
extern const GeoLayout star_road_vc_light1[];
extern const GeoLayout star_road_vc_light2[];
extern const GeoLayout star_road_vc_light3[];
extern const GeoLayout star_road_vc_light4[];
extern const GeoLayout star_road_vc_cog[];

#define bhvClockMinuteHand bhvStarRoadStarReplica
extern const BehaviorScript bhvStarRoadVCCog[];
#define bhvRrRotatingBridgePlatform bhvStarRoadVCCog
/* Fast64 end persistent block [scripts] */

const LevelScript level_vcutm_entry[] = {
	INIT_LEVEL(),
	LOAD_MIO0(0x07, _vcutm_segment_7SegmentRomStart, _vcutm_segment_7SegmentRomEnd), 
	LOAD_VANILLA_OBJECTS(0, rr, sky),
	LOAD_MIO0(8,_common0_mio0SegmentRomStart,_common0_mio0SegmentRomEnd),
	LOAD_RAW(15,_common0_geoSegmentRomStart,_common0_geoSegmentRomEnd),
	LOAD_MIO0(5,_group8_mio0SegmentRomStart,_group8_mio0SegmentRomEnd),
	LOAD_RAW(12,_group8_geoSegmentRomStart,_group8_geoSegmentRomEnd),
	LOAD_MIO0(6,_group15_mio0SegmentRomStart,_group15_mio0SegmentRomEnd),
	LOAD_RAW(13,_group15_geoSegmentRomStart,_group15_geoSegmentRomEnd),
	ALLOC_LEVEL_POOL(),
	MARIO(MODEL_MARIO, 0x00000001, bhvMario), 
	LOAD_MODEL_FROM_GEO(22, warp_pipe_geo),
	LOAD_MODEL_FROM_GEO(23, bubbly_tree_geo),
	LOAD_MODEL_FROM_GEO(24, spiky_tree_geo),
	LOAD_MODEL_FROM_GEO(25, snow_tree_geo),
	LOAD_MODEL_FROM_GEO(31, metal_door_geo),
	LOAD_MODEL_FROM_GEO(32, hazy_maze_door_geo),
	LOAD_MODEL_FROM_GEO(34, castle_door_0_star_geo),
	LOAD_MODEL_FROM_GEO(35, castle_door_1_star_geo),
	LOAD_MODEL_FROM_GEO(36, castle_door_3_stars_geo),
	LOAD_MODEL_FROM_GEO(37, key_door_geo),
	LOAD_MODEL_FROM_GEO(38, castle_door_geo),
	// LOAD_MODEL_FROM_DL(84,0x05002e00,4),
	// LOAD_MODEL_FROM_DL(86,0x05003120,4),
	// LOAD_MODEL_FROM_DL(132,0x08025f08,4),
	// LOAD_MODEL_FROM_DL(158,0x0302c8a0,4),
	// LOAD_MODEL_FROM_DL(159,0x0302bcd0,4),
	// LOAD_MODEL_FROM_DL(161,0x0301cb00,4),
	// LOAD_MODEL_FROM_DL(164,0x04032a18,4),
	// LOAD_MODEL_FROM_DL(201,0x080048e0,4),
	// LOAD_MODEL_FROM_DL(218,0x08024bb8,4),
	JUMP_LINK(script_func_global_1),
	JUMP_LINK(script_func_global_9),
	JUMP_LINK(script_func_global_16),
	JUMP_LINK(script_func_vo_rr),
	LOAD_MODEL_FROM_GEO(19, star_road_vc_cog),
	LOAD_MODEL_FROM_GEO(65, star_road_vc_light),
    LOAD_MODEL_FROM_GEO(MODEL_RR_TRICKY_TRIANGLES_FRAME1, star_road_vc_light1),
    LOAD_MODEL_FROM_GEO(MODEL_RR_TRICKY_TRIANGLES_FRAME2, star_road_vc_light2),
    LOAD_MODEL_FROM_GEO(MODEL_RR_TRICKY_TRIANGLES_FRAME3, star_road_vc_light3),
    LOAD_MODEL_FROM_GEO(MODEL_RR_TRICKY_TRIANGLES_FRAME4, star_road_vc_light4),
	/* Fast64 begin persistent block [level commands] */
	/* Fast64 end persistent block [level commands] */

	AREA(1, vcutm_area_1),
		TERRAIN(vcutm_area_1_collision),
		MACRO_OBJECTS(vcutm_area_1_macro_objs),
		SET_BACKGROUND_MUSIC(0x00, SEQ_LEVEL_GRASS),
		TERRAIN_TYPE(TERRAIN_GRASS),
		/* Fast64 begin persistent block [area commands] */
		TERRAIN(col_vcutm_1_0xe024188),
		SET_BACKGROUND_MUSIC(0,25),

		OBJECT_WITH_ACTS(0,-464,526,-464,0,270,0,0xa0000, bhvSpinAirborneWarp,63),
		OBJECT_WITH_ACTS(137,2038,185,-464,0,0,0,0x20000, bhvExclamationBox,63),
		OBJECT_WITH_ACTS(85,-4751,-77,-459,0,90,0,0x20000, bhvCapSwitch,63),
		OBJECT_WITH_ACTS(207,4093,-77,-464,0,270,0,0x0, bhvFloorSwitchAnimatesObject,63),
		OBJECT_WITH_ACTS(137,-2327,706,825,0,0,0,0x20000, bhvExclamationBox,63),
		OBJECT_WITH_ACTS(137,-4990,29,-3367,0,0,0,0x50000, bhvExclamationBox,63),
		OBJECT_WITH_ACTS(101,2977,72,-464,0,270,0,0x0, bhvBoo,63),
		OBJECT_WITH_ACTS(65,1645,10,1227,0,90,0,0x0, bhvAnimatesOnFloorSwitchPress,63),
		OBJECT_WITH_ACTS(0,4695,379,-464,0,0,0,0x0, bhvHiddenStar,63),
		OBJECT_WITH_ACTS(217,-4990,-788,-1779,0,0,0,0x0, bhvPushableMetalBox,63),
		OBJECT_WITH_ACTS(116,-312,-76,1900,0,270,0,0x0, bhvOneCoin,63),
		OBJECT_WITH_ACTS(116,-312,-76,1700,0,0,0,0x0, bhvOneCoin,63),
		OBJECT_WITH_ACTS(116,-312,-76,1500,0,0,0,0x0, bhvOneCoin,63),
		OBJECT_WITH_ACTS(101,-1557,49,78,0,90,0,0x0, bhvBoo,63),
		OBJECT_WITH_ACTS(101,-2756,348,-93,0,90,0,0x0, bhvBoo,63),
		OBJECT_WITH_ACTS(223,5160,122,-454,0,270,0,0x0, bhvChuckya,63),
		OBJECT_WITH_ACTS(101,5777,591,-2920,0,270,0,0x0, bhvBoo,63),
		OBJECT_WITH_ACTS(116,6108,422,2375,0,0,0,0x0, bhvOneCoin,63),
		OBJECT_WITH_ACTS(116,5708,422,2375,0,0,0,0x0, bhvOneCoin,63),
		OBJECT_WITH_ACTS(116,5908,422,2375,0,0,0,0x0, bhvOneCoin,63),
		OBJECT_WITH_ACTS(212,7355,656,-1625,0,0,0,0x0, bhv1Up,63),
		OBJECT_WITH_ACTS(212,-2578,628,2317,0,0,0,0x0, bhv1Up,63),
		OBJECT_WITH_ACTS(0,3800,-77,-454,0,0,0,0x0, bhvGoombaTripletSpawner,63),
		OBJECT_WITH_ACTS(122,-2702,1959,-3291,0,0,0,0x5000000, bhvClockMinuteHand,31),
		OBJECT_WITH_ACTS(19,-1546,-131,70,0,0,0,0x0, bhvRrRotatingBridgePlatform,31),
		OBJECT_WITH_ACTS(19,-1987,18,-774,0,0,0,0x3000000, bhvRrRotatingBridgePlatform,31),
		OBJECT_WITH_ACTS(19,-2326,318,823,0,0,0,0x0, bhvRrRotatingBridgePlatform,31),
		OBJECT_WITH_ACTS(19,-2771,168,-90,0,0,0,0x0, bhvRrRotatingBridgePlatform,31),
		OBJECT_WITH_ACTS(19,-3663,18,-560,0,0,0,0x0, bhvRrRotatingBridgePlatform,31),
		OBJECT_WITH_ACTS(65,-714,10,-3240,0,0,0,0x0, bhvAnimatesOnFloorSwitchPress,31),
		OBJECT_WITH_ACTS(65,4695,207,-464,0,0,0,0x0, bhvAnimatesOnFloorSwitchPress,31),
		OBJECT_WITH_ACTS(65,6857,496,-3260,0,0,0,0x0, bhvAnimatesOnFloorSwitchPress,31),
		OBJECT_WITH_ACTS(65,4699,50,2332,0,0,0,0x0, bhvAnimatesOnFloorSwitchPress,31),
		OBJECT_WITH_ACTS(0,-707,131,-3244,0,0,0,0x0, bhvHiddenStarTrigger,31),
		OBJECT_WITH_ACTS(0,6837,645,-3258,0,0,0,0x0, bhvHiddenStarTrigger,31),
		OBJECT_WITH_ACTS(0,1654,208,1228,0,0,0,0x0, bhvHiddenStarTrigger,31),
		OBJECT_WITH_ACTS(0,4692,330,-469,0,0,0,0x0, bhvHiddenStarTrigger,31),
		OBJECT_WITH_ACTS(0,4688,258,2332,0,0,0,0x0, bhvHiddenStarTrigger,31),
		OBJECT_WITH_ACTS(0,-4763,-77,-128,0,0,0,0x0, bhvGoombaTripletSpawner,31),
		OBJECT_WITH_ACTS(192,1857,-77,2045,0,0,0,0x0, bhvGoomba,31),
		OBJECT_WITH_ACTS(192,-636,-77,-2680,0,0,0,0x0, bhvGoomba,31),
		OBJECT_WITH_ACTS(101,-713,27,1718,0,180,0,0x0, bhvBoo,31),
		OBJECT_WITH_ACTS(101,6733,526,1978,0,270,0,0x0, bhvBoo,31),
		// OBJECT_WITH_ACTS(0,0,0,0,0,0,0,0x0, bhvThiTinyIslandTop,31),
		WARP_NODE(10,9,1,10,0),
		WARP_NODE(11,9,1,12,0),
		WARP_NODE(12,9,1,11,0),
		WARP_NODE(13,9,1,14,0),
		WARP_NODE(14,9,1,13,0),
		WARP_NODE(240,16,1,42,0),
		WARP_NODE(241,16,1,43,0),
		WARP_NODE(0,9,1,10,0),
		WARP_NODE(1,9,1,10,0),
		WARP_NODE(2,9,1,10,0),
		WARP_NODE(3,9,1,10,0),
		WARP_NODE(4,9,1,10,0),
		WARP_NODE(5,9,1,10,0),
		WARP_NODE(6,9,1,10,0),
		/* Fast64 end persistent block [area commands] */
	END_AREA(),
	FREE_LEVEL_POOL(),
	MARIO_POS(1, 0, 0, 0, 0),
	CALL(0, lvl_init_or_update),
	CALL_LOOP(1, lvl_init_or_update),
	CLEAR_LEVEL(),
	SLEEP_BEFORE_EXIT(1),
	EXIT(),
};