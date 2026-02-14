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

#include "make_const_nonconst.h"
#include "levels/ded/header.h"

/* Fast64 begin persistent block [scripts] */
#include "levels/ded/custom_c/custom.collision_d.inc.c"
#define ded_area_1_collision col_cotmc_1_0xe061700_d
extern const GeoLayout Geo_cotmc_1_0x2701700_d[];
#define ded_area_1 Geo_cotmc_1_0x2701700_d

#define UNDEGREES(x) ((x) / 0x2000 * 45)
/* Fast64 end persistent block [scripts] */

const LevelScript level_ded_entry[] = {
	INIT_LEVEL(),
	LOAD_MIO0(0x07, _ded_segment_7SegmentRomStart, _ded_segment_7SegmentRomEnd), 
	LOAD_MIO0(0x0A, _bbh_skybox_mio0SegmentRomStart, _bbh_skybox_mio0SegmentRomEnd), 
	LOAD_MIO0(5,_group9_mio0SegmentRomStart,_group9_mio0SegmentRomEnd),
	LOAD_RAW(12,_group9_geoSegmentRomStart,_group9_geoSegmentRomEnd),
	ALLOC_LEVEL_POOL(),
	MARIO(MODEL_MARIO, 0x00000001, bhvMario), 
	JUMP_LINK(script_func_global_10),
	/* Fast64 begin persistent block [level commands] */
	/* Fast64 end persistent block [level commands] */

	AREA(1, ded_area_1),
		WARP_NODE(0x0A, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		TERRAIN(ded_area_1_collision),
		MACRO_OBJECTS(ded_area_1_macro_objs),
		SET_BACKGROUND_MUSIC(0x00, 0x30),
		TERRAIN_TYPE(TERRAIN_GRASS),
		OBJECT_WITH_ACTS(0, 2791,-4218,-1630,0,30,0,0x7f7f7f7f, bhvWarp,63),
		OBJECT_WITH_ACTS(84,-2455,-3559,1090,0,UNDEGREES(48059),0,0x0, bhvBoo,63),
		OBJECT_WITH_ACTS(84,2227,-2544,3333,0,UNDEGREES(40595),0,0x0, bhvBoo,63),
		OBJECT_WITH_ACTS(84,2700,-3177,-570,0,UNDEGREES(63169),0,0x0, bhvBoo,63),
		OBJECT_WITH_ACTS(84,2516,-3333,1167,0,UNDEGREES(53156),0,0x0, bhvBoo,63),
		OBJECT_WITH_ACTS(84,-2703,-3563,1506,0,UNDEGREES(38047),0,0x0, bhvBoo,63),
		WARP_NODE(0x7f,18,1,10,0),
		/* Fast64 begin persistent block [area commands] */
		/* Fast64 end persistent block [area commands] */
	END_AREA(),
	FREE_LEVEL_POOL(),
	MARIO_POS(1,135,-4728,-3125,5),
	CALL(0, lvl_init_or_update),
	CALL_LOOP(1, lvl_init_or_update),
	CLEAR_LEVEL(),
	SLEEP_BEFORE_EXIT(1),
	EXIT(),
};