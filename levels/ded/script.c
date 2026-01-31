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

/* Fast64 end persistent block [scripts] */

const LevelScript level_ded_entry[] = {
	INIT_LEVEL(),
	LOAD_MIO0(0x07, _ded_segment_7SegmentRomStart, _ded_segment_7SegmentRomEnd), 
	LOAD_MIO0(0x0A, _bbh_skybox_mio0SegmentRomStart, _bbh_skybox_mio0SegmentRomEnd), 
	ALLOC_LEVEL_POOL(),
	MARIO(MODEL_MARIO, 0x00000001, bhvMario), 
	/* Fast64 begin persistent block [level commands] */
	/* Fast64 end persistent block [level commands] */

	AREA(1, ded_area_1),
		WARP_NODE(0x0A, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		TERRAIN(ded_area_1_collision),
		MACRO_OBJECTS(ded_area_1_macro_objs),
		SET_BACKGROUND_MUSIC(0x00, 0x30),
		TERRAIN_TYPE(TERRAIN_GRASS),
		OBJECT_WITH_ACTS(0, 2791,-4218,-1630,0,30,0,0x7f7f7f7f, bhvWarp,63),
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