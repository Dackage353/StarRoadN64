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
#include "levels/wdw/header.h"

/* Fast64 begin persistent block [scripts] */
#include "levels/wdw/custom_c/custom.collision_noinviwalls.inc.c"
#define wdw_area_1_collision col_wdw_1_0xe090ee8
extern const GeoLayout Geo_wdw_1_0x1821700[];
#define wdw_area_1 Geo_wdw_1_0x1821700

extern const BehaviorScript bhvStarRoadYoshiCoin[]; // 56 = 0x38 = MODEL_TTM_YELLOW_SMILEY
#define bhvWfBreakableWallLeft bhvStarRoadYoshiCoin

extern const BehaviorScript bhvStarRoadCCCandyStar[];
#define Bhv_Custom_0x13004cc8 bhvStarRoadCCCandyStar

extern const BehaviorScript bhvStarRoadChainChomp[];
#define bhvDDDPole bhvStarRoadChainChomp

extern const BehaviorScript bhvStarRoadShyguy[];
#define bhvKlepto bhvStarRoadShyguy

extern const BehaviorScript bhvStarRoadCCCandyDomino[];
#define bhvBeginningPeach bhvStarRoadCCCandyDomino

extern const GeoLayout shyguy_geo[];
extern const GeoLayout star_road_cccandy_domino[];
/* Fast64 end persistent block [scripts] */

const LevelScript level_wdw_entry[] = {
	INIT_LEVEL(),
	LOAD_MIO0(0x07, _wdw_segment_7SegmentRomStart, _wdw_segment_7SegmentRomEnd), 
	LOAD_MIO0(0xA,_SkyboxCustom26050240_skybox_mio0SegmentRomStart,_SkyboxCustom26050240_skybox_mio0SegmentRomEnd),
	LOAD_VANILLA_OBJECTS(0, ttm, mountain),
	LOAD_MIO0(8,_common0_mio0SegmentRomStart,_common0_mio0SegmentRomEnd),
	LOAD_RAW(15,_common0_geoSegmentRomStart,_common0_geoSegmentRomEnd),
	LOAD_MIO0(5,_group7_mio0SegmentRomStart,_group7_mio0SegmentRomEnd),
	LOAD_RAW(12,_group7_geoSegmentRomStart,_group7_geoSegmentRomEnd),
	LOAD_MIO0(6,_group14_mio0SegmentRomStart,_group14_mio0SegmentRomEnd),
	LOAD_RAW(13,_group14_geoSegmentRomStart,_group14_geoSegmentRomEnd),
	ALLOC_LEVEL_POOL(),
	MARIO(MODEL_MARIO, 0x00000001, bhvMario), 
	JUMP_LINK(script_func_vo_ttm),
	LOAD_MODEL_FROM_GEO(22, warp_pipe_geo),
	LOAD_MODEL_FROM_GEO(23, bubbly_tree_geo),
	LOAD_MODEL_FROM_GEO(24, spiky_tree_geo),
	LOAD_MODEL_FROM_GEO(25, snow_tree_geo),
	LOAD_MODEL_FROM_GEO(31, metal_door_geo),
	LOAD_MODEL_FROM_GEO(32, shyguy_geo),
	LOAD_MODEL_FROM_GEO(34, castle_door_0_star_geo),
	LOAD_MODEL_FROM_GEO(35, castle_door_1_star_geo),
	LOAD_MODEL_FROM_GEO(36, castle_door_3_stars_geo),
	LOAD_MODEL_FROM_GEO(37, key_door_geo),
	LOAD_MODEL_FROM_GEO(38, castle_door_geo),
	JUMP_LINK(script_func_global_1),
	JUMP_LINK(script_func_global_8),
	JUMP_LINK(script_func_global_15),
	LOAD_MODEL_FROM_GEO(0x7b, star_road_cccandy_domino),
	/* Fast64 begin persistent block [level commands] */
	/* Fast64 end persistent block [level commands] */

	AREA(1, wdw_area_1),
		TERRAIN(wdw_area_1_collision),
		MACRO_OBJECTS(wdw_area_1_macro_objs),
		SET_BACKGROUND_MUSIC(0x00, SEQ_LEVEL_GRASS),
		TERRAIN_TYPE(TERRAIN_GRASS),
		/* Fast64 begin persistent block [area commands] */
		SET_BACKGROUND_MUSIC(0,6),
		TERRAIN_TYPE(0),

		OBJECT_WITH_ACTS(0,2055,-763,-6037,0,270,0,0xa0000, bhvSpinAirborneWarp,63),
		OBJECT_WITH_ACTS(87,3216,22,-5908,0,270,0,0x0, bhvRacingPenguin,1),
		OBJECT_WITH_ACTS(0,-12511,710,9007,0,0,0,0x0, bhvPenguinRaceShortcutCheck,63),
		OBJECT_WITH_ACTS(0,-12387,1024,8905,0,0,0,0x0, bhvPlaysMusicTrackWhenTouched,63),
		OBJECT_WITH_ACTS(0,-3742,-4454,1279,0,0,0,0x0, bhvPenguinRaceFinishLine,63),
		OBJECT_WITH_ACTS(0,-3010,-1596,-175,0,0,0,0x0, bhvTree,63),
		OBJECT_WITH_ACTS(0,-3966,-696,1370,0,0,0,0x0, bhvTree,63),
		OBJECT_WITH_ACTS(0,-2579,-696,2170,0,0,0,0x0, bhvTree,63),
		OBJECT_WITH_ACTS(0,-2129,-696,987,0,0,0,0x0, bhvTree,63),
		OBJECT_WITH_ACTS(0,1928,-696,4221,0,0,0,0x0, bhvTree,63),
		OBJECT_WITH_ACTS(0,-915,-696,4160,0,0,0,0x0, bhvTree,63),
		OBJECT_WITH_ACTS(0,669,-696,2895,0,0,0,0x0, bhvTree,63),
		OBJECT_WITH_ACTS(0,1484,-996,-3625,0,0,0,0x8c0000, bhvPoleGrabbing,63),
		OBJECT_WITH_ACTS(0,4257,653,-2688,0,0,0,0x500000, bhvPoleGrabbing,63),
		OBJECT_WITH_ACTS(22,-4002,-4514,2165,0,180,0,0xd0000, bhvWarpPipe,63),
		OBJECT_WITH_ACTS(22,2741,1222,-6578,0,0,0,0xe0000, bhvWarpPipe,63),
		OBJECT_WITH_ACTS(22,2059,1949,1836,0,90,0,0xb0000, bhvWarpPipe,63),
		OBJECT_WITH_ACTS(22,-3591,2603,868,0,90,0,0xc0000, bhvWarpPipe,63),
		OBJECT_WITH_ACTS(56,571,2003,5692,0,0,0,0x0, bhvWfBreakableWallLeft,63),
		OBJECT_WITH_ACTS(56,-4917,-3285,1207,0,0,0,0x0, bhvWfBreakableWallLeft,63),
		OBJECT_WITH_ACTS(56,2735,1804,-6591,0,0,0,0x0, bhvWfBreakableWallLeft,63),
		OBJECT_WITH_ACTS(0,-840,2041,-2682,0,0,0,0xc80000, bhvPoleGrabbing,63),
		OBJECT_WITH_ACTS(0,4128,-696,3128,0,0,0,0x0, bhvHiddenStarTrigger,63),
		OBJECT_WITH_ACTS(0,-3325,-696,2850,0,0,0,0x0, bhvHiddenStarTrigger,63),
		OBJECT_WITH_ACTS(0,-5128,201,-1407,0,0,0,0x0, bhvHiddenStarTrigger,63),
		OBJECT_WITH_ACTS(0,-3919,-1596,-449,0,0,0,0x0, bhvHiddenStarTrigger,63),
		OBJECT_WITH_ACTS(0,-1643,-1596,-1083,0,0,0,0x0, bhvHiddenStarTrigger,63),
		OBJECT_WITH_ACTS(0,-5127,727,2374,0,0,0,0x2000000, bhvHiddenStar,63),
		OBJECT_WITH_ACTS(122,4769,5411,4322,0,0,0,0x1000000, bhvStar,63),
		OBJECT_WITH_ACTS(122,574,2235,5377,0,0,0,0x3000000, Bhv_Custom_0x13004cc8,63),
		OBJECT_WITH_ACTS(122,1267,4484,-899,0,0,0,0x4000000, bhvStar,63),
		OBJECT_WITH_ACTS(0,-4431,-239,-5416,0,0,0,0x5000000, bhvHiddenRedCoinStar,63),
		OBJECT_WITH_ACTS(215,5811,-141,-2649,0,0,0,0x0, bhvRedCoin,63),
		OBJECT_WITH_ACTS(215,-3309,800,4413,0,0,0,0x0, bhvRedCoin,63),
		OBJECT_WITH_ACTS(215,-1468,-1386,-4574,0,0,0,0x0, bhvRedCoin,63),
		OBJECT_WITH_ACTS(215,2888,22,-4878,0,0,0,0x0, bhvRedCoin,63),
		OBJECT_WITH_ACTS(215,3577,1648,3898,0,0,0,0x0, bhvRedCoin,63),
		OBJECT_WITH_ACTS(215,-4898,-396,-1675,0,0,0,0x0, bhvRedCoin,63),
		OBJECT_WITH_ACTS(215,6870,-696,3117,0,0,0,0x0, bhvRedCoin,63),
		OBJECT_WITH_ACTS(215,-29,353,2223,0,0,0,0x0, bhvRedCoin,63),
		OBJECT_WITH_ACTS(56,-3050,3573,3343,0,0,0,0x0, bhvWfBreakableWallLeft,63),
		OBJECT_WITH_ACTS(56,-5129,1427,-1550,0,0,0,0x0, bhvWfBreakableWallLeft,63),
		OBJECT_WITH_ACTS(0,-3034,2342,3419,0,0,0,0x0, bhvHidden1upInPoleSpawner,63),
		OBJECT_WITH_ACTS(195,5067,3353,4259,0,0,0,0x0, bhvBobombBuddyOpensCannon,63),
		OBJECT_WITH_ACTS(201,-26,1104,1022,0,0,0,0xbe0000, bhvCannonClosed,63),
		OBJECT_WITH_ACTS(0,587,2004,1023,0,0,0,0xef0000, bhvFadingWarp,63),
		OBJECT_WITH_ACTS(137,122,188,4903,0,0,0,0x50000, bhvExclamationBox,63),
		OBJECT_WITH_ACTS(0,3137,3007,-3962,0,0,0,0x70000, bhvExclamationBox,63),
		OBJECT_WITH_ACTS(102,4968,1105,2396,0,0,0,0x0, bhvDDDPole,63),
		OBJECT_WITH_ACTS(0,2612,-2708,4155,0,90,0,0x0, bhvCoinFormation,63),
		OBJECT_WITH_ACTS(223,2879,2042,-2838,0,0,0,0x0, bhvChuckya,63),
		OBJECT_WITH_ACTS(223,-4998,1100,3422,0,0,0,0x0, bhvChuckya,63),
		OBJECT_WITH_ACTS(220,-1858,3073,1479,0,0,0,0x10000, bhvFlyGuy,63),
		OBJECT_WITH_ACTS(220,-2462,3073,889,0,0,0,0x10000, bhvFlyGuy,63),
		OBJECT_WITH_ACTS(0,-2657,-3958,2463,0,0,0,0x0, bhvCoinFormation,63),
		OBJECT_WITH_ACTS(0,-5132,201,705,0,0,0,0x0, bhvCoinFormation,63),
		OBJECT_WITH_ACTS(140,-481,-696,749,0,0,0,0x0, bhvBlueCoinSwitch,63),
		OBJECT_WITH_ACTS(118,-477,-846,-706,0,0,0,0x0, bhvHiddenBlueCoin,63),
		OBJECT_WITH_ACTS(118,-853,-846,-1975,0,0,0,0x0, bhvHiddenBlueCoin,63),
		OBJECT_WITH_ACTS(118,-2355,-846,-1997,0,0,0,0x0, bhvHiddenBlueCoin,63),
		OBJECT_WITH_ACTS(220,5151,4011,4010,0,0,0,0x10000, bhvFlyGuy,63),
		OBJECT_WITH_ACTS(0,4965,1105,2397,0,0,0,0x20000, bhvCoinFormation,63),
		OBJECT_WITH_ACTS(129,203,-1446,-5805,0,0,0,0x0, bhvJumpingBox,63),
		OBJECT_WITH_ACTS(0,-638,-696,3988,0,0,0,0x10000, bhvGoombaTripletSpawner,63),
		OBJECT_WITH_ACTS(85,-4723,-485,-5433,0,0,0,0x0, bhvMrBlizzard,63),
		OBJECT_WITH_ACTS(0,3150,365,-3619,0,0,0,0x10000, bhvGoombaTripletSpawner,63),
		OBJECT_WITH_ACTS(116,736,2342,-3664,0,0,0,0x0, bhvOneCoin,63),
		OBJECT_WITH_ACTS(116,433,2342,-3815,0,0,0,0x0, bhvOneCoin,63),
		OBJECT_WITH_ACTS(116,-769,2342,-3659,0,0,0,0x0, bhvOneCoin,63),
		OBJECT_WITH_ACTS(116,-1065,2342,-3820,0,0,0,0x0, bhvOneCoin,63),
		OBJECT_WITH_ACTS(116,-5365,-963,-5427,0,0,0,0x0, bhvOneCoin,63),
		OBJECT_WITH_ACTS(116,-4434,-963,-6366,0,0,0,0x0, bhvOneCoin,63),
		OBJECT_WITH_ACTS(116,-4976,-963,-6197,0,0,0,0x0, bhvOneCoin,63),
		OBJECT_WITH_ACTS(116,-4975,-963,-4686,0,0,0,0x0, bhvOneCoin,63),
		OBJECT_WITH_ACTS(116,-4430,-963,-4496,0,0,0,0x0, bhvOneCoin,63),
		OBJECT_WITH_ACTS(116,-3854,-963,-6202,0,0,0,0x0, bhvOneCoin,63),
		OBJECT_WITH_ACTS(116,-3868,-963,-4664,0,0,0,0x0, bhvOneCoin,63),
		OBJECT_WITH_ACTS(116,-3508,-963,-5398,0,0,0,0x0, bhvOneCoin,63),
		OBJECT_WITH_ACTS(32,-1796,-1596,-5814,0,0,0,0x0, bhvKlepto,63),
		OBJECT_WITH_ACTS(32,-2780,-1596,-5213,0,0,0,0x0, bhvKlepto,63),
		OBJECT_WITH_ACTS(32,-2147,-1596,-388,0,0,0,0x0, bhvKlepto,63),
		OBJECT_WITH_ACTS(32,-3182,-1596,-1164,0,0,0,0x0, bhvKlepto,63),
		OBJECT_WITH_ACTS(32,-212,-1596,-5977,0,0,0,0x0, bhvKlepto,63),
		OBJECT_WITH_ACTS(32,689,-1596,-5830,0,0,0,0x0, bhvKlepto,63),
		OBJECT_WITH_ACTS(137,-1990,518,4722,0,0,0,0x60000, bhvExclamationBox,63),
		OBJECT_WITH_ACTS(118,4620,-846,1155,0,90,0,0x0, bhvMovingBlueCoin,63),
		OBJECT_WITH_ACTS(116,-2488,-21,4495,0,0,359,0x0, bhvOneCoin,63),
		OBJECT_WITH_ACTS(116,-5371,-21,4501,0,0,0,0x0, bhvOneCoin,63),
		OBJECT_WITH_ACTS(116,-4956,-471,4956,0,0,0,0x0, bhvOneCoin,63),
		OBJECT_WITH_ACTS(116,-2895,-471,4948,0,0,0,0x0, bhvOneCoin,63),
		OBJECT_WITH_ACTS(180,-4550,307,4345,0,0,0,0x0, bhvFireSpitter,63),
		OBJECT_WITH_ACTS(180,-3299,307,4345,0,0,0,0x0, bhvFireSpitter,63),
		OBJECT_WITH_ACTS(118,4620,-846,5088,0,90,0,0x0, bhvMovingBlueCoin,63),
		OBJECT_WITH_ACTS(129,566,-695,1641,0,0,0,0x30000, bhvBreakableBox,63),
		OBJECT_WITH_ACTS(129,566,-695,1641,0,0,0,0x0, bhvBreakableBox,63),
		OBJECT_WITH_ACTS(130,566,-695,1641,0,0,0,0x0, bhvBreakableBoxSmall,63),
		OBJECT_WITH_ACTS(103,1418,-1296,-4524,0,270,0,0x0, bhvSmallWhomp,63),
		OBJECT_WITH_ACTS(103,637,-996,-3588,0,90,0,0x0, bhvSmallWhomp,63),
		OBJECT_WITH_ACTS(32,-5133,203,-250,0,0,0,0x0, bhvKlepto,63),
		OBJECT_WITH_ACTS(116,4780,3053,3725,0,0,0,0x0, bhvOneCoin,63),
		OBJECT_WITH_ACTS(116,4475,3953,4018,0,0,0,0x0, bhvOneCoin,63),
		OBJECT_WITH_ACTS(116,5671,4553,4028,0,0,0,0x0, bhvOneCoin,63),
		OBJECT_WITH_ACTS(0,1773,69,1625,0,0,0,0x0, bhvCoinFormation,63),
		OBJECT_WITH_ACTS(0,-3207,-696,1680,0,0,0,0x10000, bhvGoombaTripletSpawner,63),
		OBJECT_WITH_ACTS(116,2945,1798,1845,0,0,0,0x0, bhvOneCoin,63),
		OBJECT_WITH_ACTS(87,-4972,-4524,1148,0,90,0,0x3b0000, bhvSLWalkingPenguin,62),
		OBJECT_WITH_ACTS(123,-1824,2453,1039,0,180,0,0x0, bhvBeginningPeach,31),
		OBJECT_WITH_ACTS(123,-1824,2453,1539,0,180,0,0x0, bhvBeginningPeach,31),
		OBJECT_WITH_ACTS(123,-2857,2453,871,0,270,0,0x0, bhvBeginningPeach,31),
		OBJECT_WITH_ACTS(123,-2357,2453,871,0,270,0,0x0, bhvBeginningPeach,31),
		OBJECT_WITH_ACTS(0,2734,1162,-6430,321,0,0,0x0, bhvWarpPipe,63),
		OBJECT_WITH_ACTS(MODEL_RED_COIN_RADAR,0,0,0,0,0,0,6<<24, bhvRedCoinRadar, 31),

		WARP_NODE(10,9,1,10,0),
		WARP_NODE(11,11,1,12,0),
		WARP_NODE(12,11,1,11,0),
		WARP_NODE(13,11,1,14,0),
		WARP_NODE(14,11,1,13,0),
		WARP_NODE(240,26,1,36,0),
		WARP_NODE(241,26,1,37,0),
		WARP_NODE(0,11,1,13,0),
		WARP_NODE(1,9,1,10,0),
		WARP_NODE(2,9,1,10,0),
		WARP_NODE(3,9,1,10,0),
		WARP_NODE(4,9,1,10,0),
		WARP_NODE(5,9,1,10,0),
		WARP_NODE(6,9,1,10,0),
		WARP_NODE(0xef,LEVEL_WDW,1,0xef,0),
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