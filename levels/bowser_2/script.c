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
#include "levels/bowser_2/header.h"

/* Fast64 begin persistent block [scripts] */
#include "levels/bowser_2/custom_c/custom.collision.inc.c"
#define bowser_2_area_1_collision col_bowser_2_1_0xe00f718
extern const GeoLayout Geo_bowser_2_1_0x2a81700[];
#define bowser_2_area_1 Geo_bowser_2_1_0x2a81700

extern const BehaviorScript bhvStarRoadBowserStub[];
#define bhvBowser bhvStarRoadBowserStub

extern const BehaviorScript bhvStarRoadB2FightPlatform[];
#define bhvWdwExpressElevator bhvStarRoadB2FightPlatform

extern const GeoLayout star_road_b2_platform[]; 
/* Fast64 end persistent block [scripts] */

const LevelScript level_bowser_2_entry[] = {
	INIT_LEVEL(),
	LOAD_MIO0(0x07, _bowser_2_segment_7SegmentRomStart, _bowser_2_segment_7SegmentRomEnd), 
	LOAD_VANILLA_OBJECTS(0, lll, fire),
	LOAD_MIO0(        /*seg*/ 0x0B, _effect_mio0SegmentRomStart, _effect_mio0SegmentRomEnd),
	LOAD_MIO0(0xA,_bitfs_skybox_mio0SegmentRomStart,_bitfs_skybox_mio0SegmentRomEnd),
	LOAD_MIO0(8,_common0_mio0SegmentRomStart,_common0_mio0SegmentRomEnd),
	LOAD_RAW(15,_common0_geoSegmentRomStart,_common0_geoSegmentRomEnd),
	LOAD_MIO0(5,_group2_mio0SegmentRomStart,_group2_mio0SegmentRomEnd),
	LOAD_RAW(12,_group2_geoSegmentRomStart,_group2_geoSegmentRomEnd),
	LOAD_MIO0(6,_group12_mio0SegmentRomStart,_group12_mio0SegmentRomEnd),
	LOAD_RAW(13,_group12_geoSegmentRomStart,_group12_geoSegmentRomEnd),
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
	LOAD_MODEL_FROM_GEO(9, star_road_b2_platform),
	JUMP_LINK(script_func_global_1),
	JUMP_LINK(script_func_global_3),
	JUMP_LINK(script_func_global_13),
	/* Fast64 begin persistent block [level commands] */
	/* Fast64 end persistent block [level commands] */

	AREA(1, bowser_2_area_1),
		TERRAIN(bowser_2_area_1_collision),
		MACRO_OBJECTS(bowser_2_area_1_macro_objs),
		SET_BACKGROUND_MUSIC(0x00, SEQ_LEVEL_GRASS),
		TERRAIN_TYPE(TERRAIN_GRASS),
		/* Fast64 begin persistent block [area commands] */
		SET_BACKGROUND_MUSIC(0,34),
		TERRAIN_TYPE(0),

		OBJECT_WITH_ACTS(0,-48,1360,-3192,0,0,0,0xa0000, bhvAirborneWarp,31),
		OBJECT_WITH_ACTS(0,85,-100+1076,1364,0,270,0,0x0, bhvBowser,31),
		OBJECT_WITH_ACTS(9,-48,1492,-135,0,0,0,0x0, bhvWdwExpressElevator,31),
		OBJECT_WITH_ACTS(9,-48,1492,1875,0,0,0,0x0, bhvWdwExpressElevator,31),
		OBJECT_WITH_ACTS(87,-46,1843,1843,0,270,0,0x0, bhvBigBully,31),
		OBJECT_WITH_ACTS(86,159,1799,-346,0,270,0,0x0, bhvSmallBully,31),
		OBJECT_WITH_ACTS(86,-48,1799,27,0,270,0,0x0, bhvSmallBully,31),
		OBJECT_WITH_ACTS(86,-260,1799,-341,0,270,0,0x0, bhvSmallBully,31),
		OBJECT_WITH_ACTS(180,-48,1903,-135,0,0,0,0x0, bhvFireSpitter,31),
		OBJECT_WITH_ACTS(180,-48,1903,1875,0,0,0,0x0, bhvFireSpitter,31),
		OBJECT_WITH_ACTS(116,-43,1198,-2415,0,0,0,0x0, bhvOneCoin,31),
		OBJECT_WITH_ACTS(116,-43,1598,-1313,0,0,0,0x0, bhvOneCoin,31),
		OBJECT_WITH_ACTS(116,-43,1398,-1858,0,0,0,0x0, bhvOneCoin,31),
		OBJECT_WITH_ACTS(212,5430,2236,2560,0,0,0,0x0, bhv1Up,31),
		OBJECT_WITH_ACTS(137,-6755,2944,-2350,0,0,0,0x30000, bhvExclamationBox,31),
		OBJECT_WITH_ACTS(0,-5,-8854,260,0,0,0,0xa020000, bhvWarp,31),

		WARP_NODE(10,9,1,10,0),
		WARP_NODE(11,9,1,12,0),
		WARP_NODE(12,9,1,11,0),
		WARP_NODE(13,9,1,14,0),
		WARP_NODE(14,9,1,13,0),
		WARP_NODE(240,26,1,27,0),
		WARP_NODE(241,19,1,12,0),
		WARP_NODE(0,9,1,10,0),
		WARP_NODE(1,9,1,10,0),
		WARP_NODE(2,19,1,12,0),
		WARP_NODE(3,33,1,2,0),
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