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
#include "levels/bbh/header.h"

/* Fast64 begin persistent block [scripts] */
extern const Collision col_bbh_1_0xe03c6f0[];
#define bbh_area_1_collision col_bbh_1_0xe03c6f0
extern const GeoLayout Geo_bbh_1_0x1201700[];
#define bbh_area_1 Geo_bbh_1_0x1201700

extern const BehaviorScript bhvStarRoadGGBigRotatingPlatform[];
extern const BehaviorScript bhvStarRoadGGSquarePlatform[];
extern const BehaviorScript bhvStarRoadGGTwistyPlatform[];
extern const BehaviorScript bhvStarRoadGGGrave[];
extern const BehaviorScript bhvStarRoadGGLamp[];
extern const BehaviorScript bhvStarRoadGGDonutPlatform[];
extern const BehaviorScript bhvStarRoadGGBoo[];

#define bhvSlidingPlatform2 bhvStarRoadGGBigRotatingPlatform // 56 = 0x38 = MODEL_RR_OCTAGONAL_PLATFORM
#define Bhv_Custom_0x1300012c bhvStarRoadGGSquarePlatform // 54 = 0x36 = MODEL_RR_SLIDING_PLATFORM
#define bhvGiantPole bhvStarRoadGGTwistyPlatform // 4
#define bhvKickableBoard bhvStarRoadGGGrave // 5
#define Bhv_Custom_0x13000684 bhvStarRoadGGLamp // 6
#define bhvBitfsTiltingInvertedPyramid bhvStarRoadGGDonutPlatform // 7

extern Gfx sr_rr_geo_0008A8[];
extern Gfx sr_rr_geo_0008C0[];
extern Gfx sr_rr_geo_000678[];
extern Gfx sr_rr_geo_000690[];
extern Gfx sr_rr_geo_0006A8[];
extern Gfx sr_rr_geo_0006C0[];
/* Fast64 end persistent block [scripts] */

const LevelScript level_bbh_entry[] = {
	INIT_LEVEL(),
	LOAD_MIO0(0x07, _bbh_segment_7SegmentRomStart, _bbh_segment_7SegmentRomEnd), 
	LOAD_MIO0(0xA,_bbh_skybox_mio0SegmentRomStart,_bbh_skybox_mio0SegmentRomEnd),
	LOAD_VANILLA_OBJECTS(0, rr, sky),
	LOAD_MIO0(8,_common0_mio0SegmentRomStart,_common0_mio0SegmentRomEnd),
	LOAD_RAW(15,_common0_geoSegmentRomStart,_common0_geoSegmentRomEnd),
	LOAD_MIO0(5,_group9_mio0SegmentRomStart,_group9_mio0SegmentRomEnd),
	LOAD_RAW(12,_group9_geoSegmentRomStart,_group9_geoSegmentRomEnd),
	LOAD_MIO0(6,_group17_mio0SegmentRomStart,_group17_mio0SegmentRomEnd),
	LOAD_RAW(13,_group17_geoSegmentRomStart,_group17_geoSegmentRomEnd),
	ALLOC_LEVEL_POOL(),
	MARIO(MODEL_MARIO, 0x00000001, bhvMario), 
	LOAD_MODEL_FROM_GEO(3, spiky_tree_geo),
	LOAD_MODEL_FROM_GEO(22, warp_pipe_geo),
	// LOAD_MODEL_FROM_DL(132,0x08025f08,4),
	// LOAD_MODEL_FROM_DL(158,0x0302c8a0,4),
	// LOAD_MODEL_FROM_DL(159,0x0302bcd0,4),
	// LOAD_MODEL_FROM_DL(161,0x0301cb00,4),
	// LOAD_MODEL_FROM_DL(164,0x04032a18,4),
	// LOAD_MODEL_FROM_DL(201,0x080048e0,4),
	// LOAD_MODEL_FROM_DL(218,0x08024bb8,4),
	JUMP_LINK(script_func_global_1),
	JUMP_LINK(script_func_global_10),
	JUMP_LINK(script_func_global_18),
	JUMP_LINK(script_func_vo_rr),

	LOAD_MODEL_FROM_GEO(MODEL_RR_OCTAGONAL_PLATFORM, sr_rr_geo_0008A8),
	LOAD_MODEL_FROM_GEO(MODEL_RR_SLIDING_PLATFORM, sr_rr_geo_0008C0),
	LOAD_MODEL_FROM_GEO(4, sr_rr_geo_000678),
	LOAD_MODEL_FROM_GEO(5, sr_rr_geo_000690),
	LOAD_MODEL_FROM_GEO(6, sr_rr_geo_0006A8),
	LOAD_MODEL_FROM_GEO(7, sr_rr_geo_0006C0),

	/* Fast64 begin persistent block [level commands] */
	/* Fast64 end persistent block [level commands] */

	AREA(1, bbh_area_1),
		TERRAIN(bbh_area_1_collision),
		MACRO_OBJECTS(bbh_area_1_macro_objs),
		SET_BACKGROUND_MUSIC(0x00, SEQ_LEVEL_GRASS),
		TERRAIN_TYPE(TERRAIN_GRASS),
		/* Fast64 begin persistent block [area commands] */
		TERRAIN(col_bbh_1_0xe03c6f0),
		SET_BACKGROUND_MUSIC(0,12),
		OBJECT_WITH_ACTS(0,-6318,-102,2153,0,180,0,0xa0000, bhvAirborneWarp,63),
		OBJECT_WITH_ACTS(0,-3172,-752,1810,0,90,0,0x5010000, bhvMrI,32),
		OBJECT_WITH_ACTS_SPECIAL(84,4398,2632,-4532,0,0,0,0x0, bhvBalconyBigBoo,1, ACT_FORCE_ACTIVE),
		OBJECT_WITH_ACTS(122,-5199,1749,-5566,0,0,0,0x1000000, bhvStar,63),
		OBJECT_WITH_ACTS_SPECIAL(56,4385,2330,-3141,0,0,0,0x1010000, bhvSlidingPlatform2,1, ACT_FORCE_ACTIVE),
		OBJECT_WITH_ACTS(0,869,-1021,3555,0,180,0,0xe0000, bhvWarp,63),
		OBJECT_WITH_ACTS(137,-7669,-471,-6639,0,0,0,0x40000, bhvExclamationBox,63),
		OBJECT_WITH_ACTS(122,4280,-1771,-4401,0,0,0,0x2000000, bhvStar,63),
		OBJECT_WITH_ACTS(84,5059,-1898,-294,0,270,0,0x6b000000, bhvStarRoadGGBoo,63),
		OBJECT_WITH_ACTS(122,5676,935,5841,0,0,0,0x3000000, bhvStar,63),
		OBJECT_WITH_ACTS_SPECIAL(0,-1160,1981,-6161,0,0,0,0x4000000, bhvHiddenRedCoinStar,16, ACT_FORCE_ACTIVE),
		OBJECT_WITH_ACTS_SPECIAL(215,4338,709,-3449,0,0,0,0x0, bhvRedCoin, 16, ACT_FORCE_ACTIVE),
		OBJECT_WITH_ACTS(0,-1152,2241,5659,0,180,0,0xb0000, bhvAirborneWarp,31),
		OBJECT_WITH_ACTS_SPECIAL(215,-1155,1668,4607,0,0,0,0x0, bhvRedCoin, 16, ACT_FORCE_ACTIVE),
		OBJECT_WITH_ACTS(54,-3173,-1638,1809,0,0,0,0x0, Bhv_Custom_0x1300012c,32),
		OBJECT_WITH_ACTS_SPECIAL(4,-6267,-752,-1461,0,90,0,0x0, bhvGiantPole,5, 21),
		OBJECT_WITH_ACTS(22,823,-1983,-392,0,90,0,0xd0000, bhvWarpPipe,63),
		OBJECT_WITH_ACTS_SPECIAL(4,-6267,-752,-190,0,90,0,0x30000, bhvGiantPole,5, 21),
		OBJECT_WITH_ACTS(22,6090,-1975,-1636,0,0,0,0x60000, bhvWarpPipe,63),
		OBJECT_WITH_ACTS(0,856,-709,3555,0,0,0,0x20000, bhvCoinFormation,63),
		OBJECT_WITH_ACTS(0,924,-761,3138,0,0,0,0x0, bhvGoombaTripletSpawner,63),
		OBJECT_WITH_ACTS(89,4325,32,-3482,0,270,270,0x0, bhvFlyingBookend,63),
		OBJECT_WITH_ACTS(89,-1144,667,-1614,0,0,90,0x0, bhvFlyingBookend,63),
		OBJECT_WITH_ACTS_SPECIAL(89,-737,-700,2147,0,270,90,0x0, bhvFlyingBookend,15, ACT_FORCE_ACTIVE),
		OBJECT_WITH_ACTS_SPECIAL(202,-451,642,4457,0,180,0,0x10000, bhvPlatformOnTrack, 16, ACT_FORCE_ACTIVE),
		OBJECT_WITH_ACTS(3,5131,32,-2928,0,93,0,0x0, bhvTree,63),
		OBJECT_WITH_ACTS(3,4387,875,755,0,0,0,0x0, bhvTree,63),
		OBJECT_WITH_ACTS(84,3168,84,-606,0,188,0,0x0, bhvBoo,63),
		OBJECT_WITH_ACTS(3,3511,32,-3997,0,0,0,0x0, bhvTree,63),
		OBJECT_WITH_ACTS(3,6016,32,-1490,0,0,0,0x0, bhvTree,63),
		OBJECT_WITH_ACTS(3,5560,32,-4392,0,0,0,0x0, bhvTree,63),
		OBJECT_WITH_ACTS(3,6495,32,-2282,0,0,0,0x0, bhvTree,63),
		OBJECT_WITH_ACTS(3,2343,-624,-2652,0,0,0,0x0, bhvTree,63),
		OBJECT_WITH_ACTS(3,19,-761,2932,0,0,0,0x0, bhvTree,63),
		OBJECT_WITH_ACTS(3,1727,-761,3944,0,0,0,0x0, bhvTree,63),
		OBJECT_WITH_ACTS(3,-5628,-748,-3275,0,0,0,0x0, bhvTree,63),
		OBJECT_WITH_ACTS(3,3078,32,648,0,0,0,0x0, bhvTree,63),
		OBJECT_WITH_ACTS_SPECIAL(84,-6267,-596,-716,0,0,0,0x0, bhvBoo,15, ACT_FORCE_ACTIVE),
		OBJECT_WITH_ACTS(84,-1522,-679,-2943,0,270,0,0x0, bhvBoo,63),
		OBJECT_WITH_ACTS(84,3275,84,-1485,0,175,0,0x0, bhvBoo,63),
		OBJECT_WITH_ACTS(84,2564,84,-1056,0,161,0,0x0, bhvBoo,63),
		OBJECT_WITH_ACTS(84,-4252,120,-3220,0,180,0,0x0, bhvBoo,63),
		OBJECT_WITH_ACTS(84,6163,84,-3286,0,270,0,0x0, bhvBoo,63),
		OBJECT_WITH_ACTS_SPECIAL(84,-2693,-596,2015,0,270,0,0x0, bhvBoo,15, ACT_FORCE_ACTIVE),
		OBJECT_WITH_ACTS(192,-5949,-748,-3027,0,0,0,0x0, bhvGoomba,63),
		OBJECT_WITH_ACTS(192,-2534,-731,-6621,0,0,0,0x0, bhvGoomba,63),
		OBJECT_WITH_ACTS(192,2538,-624,-3390,0,0,0,0x0, bhvGoomba,63),
		OBJECT_WITH_ACTS_SPECIAL(56,1920,-573,1952,0,0,0,0x10000, bhvSlidingPlatform2,23, ACT_FORCE_ACTIVE),
		OBJECT_WITH_ACTS(0,4410,32,-5274,0,90,0,0x0, bhvCoinFormation,63),
		OBJECT_WITH_ACTS_SPECIAL(4,-6267,-752,-1461,180,90,0,0x0, bhvGiantPole,10, 26),
		OBJECT_WITH_ACTS_SPECIAL(54,-1160,1666,-4984,0,0,0,0x0, Bhv_Custom_0x1300012c, 16, ACT_FORCE_ACTIVE),
		OBJECT_WITH_ACTS(101,-5243,1020,-5358,0,0,0,0x0, bhvScuttlebug,63),
		OBJECT_WITH_ACTS(101,4570,2427,-1439,0,0,0,0x0, bhvScuttlebug,63),
		OBJECT_WITH_ACTS_SPECIAL(56,2544,-625,3937,0,0,0,0x10000, bhvSlidingPlatform2,8, ACT_FORCE_ACTIVE),
		OBJECT_WITH_ACTS_SPECIAL(137,-7875,328,-4631,0,0,0,0x60000, bhvExclamationBox,217, ACT_FORCE_ACTIVE),
		OBJECT_WITH_ACTS_SPECIAL(56,3228,-215,4611,0,0,0,0x10000, bhvSlidingPlatform2,8, ACT_FORCE_ACTIVE),
		OBJECT_WITH_ACTS_SPECIAL(137,-7341,299,2475,0,0,0,0x70000, bhvExclamationBox,255, ACT_FORCE_ACTIVE),
		OBJECT_WITH_ACTS_SPECIAL(56,4231,195,5204,0,0,0,0x10000, bhvSlidingPlatform2,8, ACT_FORCE_ACTIVE),
		OBJECT_WITH_ACTS_SPECIAL(56,4551,400,5853,0,0,0,0x10000, bhvSlidingPlatform2,8, ACT_FORCE_ACTIVE),
		OBJECT_WITH_ACTS(192,-1174,668,961,0,0,0,0x0, bhvGoomba,63),
		OBJECT_WITH_ACTS(188,-7150,-751,2470,0,90,0,0x320000, bhvBobombBuddy,63),
		OBJECT_WITH_ACTS(212,-6737,419,-713,0,0,0,0x0, bhv1Up,63),
		OBJECT_WITH_ACTS(140,-1162,667,4546,0,0,0,0x0, bhvBlueCoinSwitch,63),
		OBJECT_WITH_ACTS(118,-1506,768,3192,0,0,0,0x0, bhvHiddenBlueCoin,63),
		OBJECT_WITH_ACTS(118,-808,768,1602,0,0,0,0x0, bhvHiddenBlueCoin,63),
		OBJECT_WITH_ACTS(118,-1505,768,1,0,0,0,0x0, bhvHiddenBlueCoin,63),
		OBJECT_WITH_ACTS(118,-800,768,-1616,0,0,0,0x0, bhvHiddenBlueCoin,63),
		OBJECT_WITH_ACTS(118,-1505,768,-3195,0,0,0,0x0, bhvHiddenBlueCoin,63),
		OBJECT_WITH_ACTS(0,4398,1250,-3242,0,0,0,0x0, bhvCoinFormation,63),
		OBJECT_WITH_ACTS(101,6266,32,-1925,0,0,0,0x0, bhvScuttlebug,63),
		OBJECT_WITH_ACTS_SPECIAL(4,-6267,-752,-190,180,90,0,0x0, bhvGiantPole,10, 26),
		OBJECT_WITH_ACTS(116,-5180,589,-2356,0,0,0,0x0, bhvOneCoin,63),
		OBJECT_WITH_ACTS(116,-5180,395,-1770,0,0,0,0x0, bhvOneCoin,63),
		OBJECT_WITH_ACTS(116,-5180,804,-2900,0,0,0,0x0, bhvOneCoin,63),
		OBJECT_WITH_ACTS(137,823,-903,-392,0,0,0,0x60000, bhvExclamationBox,63),
		OBJECT_WITH_ACTS_SPECIAL(56,2948,-625,5143,0,0,0,0x10000, bhvSlidingPlatform2,8, ACT_FORCE_ACTIVE),
		OBJECT_WITH_ACTS_SPECIAL(56,4478,-215,4622,0,0,0,0x10000, bhvSlidingPlatform2,8, ACT_FORCE_ACTIVE),
		OBJECT_WITH_ACTS_SPECIAL(215,1912,316,2503,0,90,0,0x0, bhvRedCoin, 16, ACT_FORCE_ACTIVE),
		OBJECT_WITH_ACTS_SPECIAL(215,4387,1387,755,0,0,0,0x0, bhvRedCoin, 16, ACT_FORCE_ACTIVE),
		OBJECT_WITH_ACTS_SPECIAL(215,-1623,-395,3492,0,0,0,0x0, bhvRedCoin, 16, ACT_FORCE_ACTIVE),
		OBJECT_WITH_ACTS_SPECIAL(215,6495,544,-2288,0,0,0,0x0, bhvRedCoin, 16, ACT_FORCE_ACTIVE),
		OBJECT_WITH_ACTS_SPECIAL(215,-3406,94,160,0,0,0,0x0, bhvRedCoin, 16, ACT_FORCE_ACTIVE),
		OBJECT_WITH_ACTS_SPECIAL(215,-1149,-54,-2954,0,0,0,0x0, bhvRedCoin, 16, ACT_FORCE_ACTIVE),
		OBJECT_WITH_ACTS(54,-5180,1065,-2900,0,0,0,0x0, Bhv_Custom_0x1300012c,63),
		OBJECT_WITH_ACTS(54,-5180,850,-2356,0,0,0,0x0, Bhv_Custom_0x1300012c,63),
		OBJECT_WITH_ACTS(54,-5180,656,-1770,0,0,0,0x0, Bhv_Custom_0x1300012c,63),
		OBJECT_WITH_ACTS_SPECIAL(54,-1323,-453,2147,0,0,0,0x0, Bhv_Custom_0x1300012c,15, ACT_FORCE_ACTIVE),
		OBJECT_WITH_ACTS_SPECIAL(54,-1160,1368,-4542,0,0,0,0x0, Bhv_Custom_0x1300012c, 16, ACT_FORCE_ACTIVE),
		OBJECT_WITH_ACTS_SPECIAL(54,-737,-453,2147,0,0,0,0x0, Bhv_Custom_0x1300012c,15, ACT_FORCE_ACTIVE),
		OBJECT_WITH_ACTS(5,865,-754,3579,0,90,0,0x0, bhvKickableBoard,63),
		OBJECT_WITH_ACTS_SPECIAL(6,4062,-1570,-1669,0,90,0,0x0, Bhv_Custom_0x13000684,15, ACT_FORCE_ACTIVE),
		OBJECT_WITH_ACTS_SPECIAL(6,4055,-1570,-2369,0,90,0,0x20000, Bhv_Custom_0x13000684,15, ACT_FORCE_ACTIVE),
		OBJECT_WITH_ACTS_SPECIAL(6,3861,-1570,-4327,0,90,0,0x0, Bhv_Custom_0x13000684,15, ACT_FORCE_ACTIVE),
		OBJECT_WITH_ACTS(7,-3173,-806,1809,0,0,0,0x0, bhvBitfsTiltingInvertedPyramid,63),
		OBJECT_WITH_ACTS(7,-1997,-806,2152,0,0,0,0x0, bhvBitfsTiltingInvertedPyramid,63),
		OBJECT_WITH_ACTS(7,-2292,-806,3384,0,0,0,0x0, bhvBitfsTiltingInvertedPyramid,63),
		WARP_NODE(10,9,1,10,0),
		WARP_NODE(11,9,1,12,0),
		WARP_NODE(12,9,1,11,0),
		WARP_NODE(13,4,1,14,0),
		WARP_NODE(14,4,1,13,0),
		WARP_NODE(240,16,1,42,0),
		WARP_NODE(241,16,1,43,0),
		WARP_NODE(0,9,1,10,0),
		WARP_NODE(1,9,1,10,0),
		WARP_NODE(2,9,1,10,0),
		WARP_NODE(3,9,1,10,0),
		WARP_NODE(4,9,1,10,0),
		WARP_NODE(5,4,1,11,0),
		WARP_NODE(6,18,1,10,0),
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