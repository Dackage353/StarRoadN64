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
#include "levels/cotmc/header.h"

/* Fast64 begin persistent block [scripts] */
#include "levels/cotmc/custom_c/custom.collision_u.inc.c"
#define cotmc_area_1_collision col_cotmc_1_0xe061700_u
extern const GeoLayout Geo_cotmc_1_0x2701700[];
#define cotmc_area_1 Geo_cotmc_1_0x2701700
#define bhvClockMinuteHand bhvStarRoadStarReplica
/* Fast64 end persistent block [scripts] */

const LevelScript level_cotmc_entry[] = {
	INIT_LEVEL(),
	LOAD_MIO0(0x07, _cotmc_segment_7SegmentRomStart, _cotmc_segment_7SegmentRomEnd), 
	LOAD_MIO0(0xA,_bbh_skybox_mio0SegmentRomStart,_bbh_skybox_mio0SegmentRomEnd),
	LOAD_VANILLA_OBJECTS(0, ssl, generic),
	LOAD_MIO0(8,_common0_mio0SegmentRomStart,_common0_mio0SegmentRomEnd),
	LOAD_RAW(15,_common0_geoSegmentRomStart,_common0_geoSegmentRomEnd),
	LOAD_MIO0(5,_group8_mio0SegmentRomStart,_group8_mio0SegmentRomEnd),
	LOAD_RAW(12,_group8_geoSegmentRomStart,_group8_geoSegmentRomEnd),
	LOAD_MIO0(6,_group17_mio0SegmentRomStart,_group17_mio0SegmentRomEnd),
	LOAD_RAW(13,_group17_geoSegmentRomStart,_group17_geoSegmentRomEnd),

	ALLOC_LEVEL_POOL(),
	LOAD_MODEL_FROM_GEO(MODEL_SSL_PALM_TREE,           palm_tree_geo),
	LOAD_MODEL_FROM_GEO(22, warp_pipe_geo),
	LOAD_MODEL_FROM_GEO(27, palm_tree_geo),
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
	JUMP_LINK(script_func_global_18),
	MARIO(MODEL_MARIO, 0x00000001, bhvMario), 
	/* Fast64 begin persistent block [level commands] */
	/* Fast64 end persistent block [level commands] */

	AREA(1, cotmc_area_1),
		TERRAIN(cotmc_area_1_collision),
		MACRO_OBJECTS(cotmc_area_1_macro_objs),
		SET_BACKGROUND_MUSIC(0x00, SEQ_LEVEL_GRASS),
		TERRAIN_TYPE(TERRAIN_GRASS),
		/* Fast64 begin persistent block [area commands] */
		SET_BACKGROUND_MUSIC(0,44),
		TERRAIN_TYPE(3),

		OBJECT_WITH_ACTS(0,0xAD6,-0xEF4,-0x657,0,0,0,0xFF200000, bhvWarp,31),
		OBJECT_WITH_ACTS(0,2038,11272,4960,0,270,0,0xa0000, bhvFlyingWarp,31),
		OBJECT_WITH_ACTS(206,2210,5430,-1046,0,0,0,0x0, bhvSnufit,31),
		OBJECT_WITH_ACTS(206,-1011,5113,710,0,-153,0,0x0, bhvSnufit,31),
		OBJECT_WITH_ACTS(122,1625,11249,4976,0,270,0,0x4000000, bhvClockMinuteHand,31),
		OBJECT_WITH_ACTS(85,4191,5321,-294,0,270,0,0x10000, bhvCapSwitch,31),
		OBJECT_WITH_ACTS(180,-1429,5660,-1906,0,0,0,0x0, bhvFireSpitter,31),
		OBJECT_WITH_ACTS(215,-177,6343,-1618,0,0,0,0x0, bhvRedCoin,31),
		OBJECT_WITH_ACTS(0,1014,5403,541,0,0,0,0x0, bhvHiddenRedCoinStar,31),
		OBJECT_WITH_ACTS(137,2024,4908,-117,0,0,0,0x10000, bhvExclamationBox,31),
		OBJECT_WITH_ACTS(215,4938,4675,-322,0,0,0,0x0, bhvRedCoin,31),
		OBJECT_WITH_ACTS(0,-1162,9468,4991,0,270,0,0x0, bhvCoinFormation,31),
		OBJECT_WITH_ACTS(0,-5771,7049,642,0,340,0,0x0, bhvCoinFormation,31),
		OBJECT_WITH_ACTS(215,649,4607,639,0,0,0,0x0, bhvRedCoin,31),
		OBJECT_WITH_ACTS(215,4641,5532,145,0,0,0,0x0, bhvRedCoin,95),
		OBJECT_WITH_ACTS(215,2172,5094,-2619,0,0,0,0x0, bhvRedCoin,31),
		OBJECT_WITH_ACTS(215,4630,5532,-786,0,0,0,0x0, bhvRedCoin,31),
		OBJECT_WITH_ACTS(215,-269,5264,-2626,0,0,0,0x0, bhvRedCoin,31),
		OBJECT_WITH_ACTS(215,2207,5288,-1539,0,0,0,0x0, bhvRedCoin,31),
		OBJECT_WITH_ACTS(0,656,5620,-1202,0,225,0,0x0, bhvCoinFormation,31),
		OBJECT_WITH_ACTS(206,-771,5248,-3631,0,0,0,0x0, bhvSnufit,31),
		OBJECT_WITH_ACTS(212,-5094,6916,-506,0,0,0,0x0, bhv1Up,31),

		WARP_NODE(0x20,16,1,128,0),
		WARP_NODE(10,9,1,10,0),
		WARP_NODE(11,9,1,12,0),
		WARP_NODE(12,9,1,11,0),
		WARP_NODE(13,9,1,14,0),
		WARP_NODE(14,9,1,13,0),
		WARP_NODE(240,16,1,27,0),
		WARP_NODE(241,16,1,28,0),
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
	MARIO_POS(1, 0, 0, 10000, 0),
	CALL(0, lvl_init_or_update),
	CALL_LOOP(1, lvl_init_or_update),
	CLEAR_LEVEL(),
	SLEEP_BEFORE_EXIT(1),
	EXIT(),
};