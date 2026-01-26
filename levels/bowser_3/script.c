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
#include "levels/bowser_3/header.h"

/* Fast64 begin persistent block [scripts] */
/* Fast64 end persistent block [scripts] */

const LevelScript level_bowser_3_entry[] = {
	INIT_LEVEL(),
	LOAD_MIO0(0x07, _bowser_3_segment_7SegmentRomStart, _bowser_3_segment_7SegmentRomEnd), 
	LOAD_YAY0(0x06, _group12_yay0SegmentRomStart, _group12_yay0SegmentRomEnd), 
	LOAD_RAW(0x0D, _group12_geoSegmentRomStart, _group12_geoSegmentRomEnd), 
	LOAD_MIO0(0xA,_SkyboxCustom46235328_skybox_mio0SegmentRomStart,_SkyboxCustom46235328_skybox_mio0SegmentRomEnd),
	ALLOC_LEVEL_POOL(),
	MARIO(MODEL_MARIO, 0x00000001, bhvMario), 
	JUMP_LINK(script_func_global_13), 
	/* Fast64 begin persistent block [level commands] */
	/* Fast64 end persistent block [level commands] */

	AREA(1, bowser_3_area_1),
		WARP_NODE(0, LEVEL_ENDING, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xf1, LEVEL_BITS, 0x01, 101, WARP_NO_CHECKPOINT),
		WARP_NODE(0x0A, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		OBJECT(MODEL_BOWSER_BOMB, 3292, 328, 0, 0, 0, 0, 0x00000000, bhvBowserBomb),
		OBJECT(MODEL_BOWSER_BOMB, 1029, 328, 3198, 0, 0, 0, 0x00000000, bhvBowserBomb),
		OBJECT(MODEL_BOWSER_BOMB, -2695, 328, 1949, 0, 0, 0, 0x00000000, bhvBowserBomb),
		OBJECT(MODEL_BOWSER_BOMB, -2712, 328, -1969, 0, 0, 0, 0x00000000, bhvBowserBomb),
		OBJECT(MODEL_BOWSER_BOMB, 1034, 328, -3166, 0, 0, 0, 0x00000000, bhvBowserBomb),
		OBJECT(MODEL_BOWSER, 13, 307, -1024, 0, 0, 0, 0x00000000, bhvBowser),
		OBJECT(MODEL_NONE, 0, 1307, 0, 0, 183, 0, 0x000a0000, bhvSpinAirborneCircleWarp),
		TERRAIN(bowser_3_area_1_collision),
		MACRO_OBJECTS(bowser_3_area_1_macro_objs),
		SET_BACKGROUND_MUSIC(0x00, 0x2e),
		TERRAIN_TYPE(TERRAIN_GRASS),
		/* Fast64 begin persistent block [area commands] */
		/* Fast64 end persistent block [area commands] */
	END_AREA(),
	FREE_LEVEL_POOL(),
	MARIO_POS(1, 0, 0, 1000, 0),
	CALL(0, lvl_init_or_update),
	CALL_LOOP(1, lvl_init_or_update),
	CLEAR_LEVEL(),
	SLEEP_BEFORE_EXIT(1),
	EXIT(),
};