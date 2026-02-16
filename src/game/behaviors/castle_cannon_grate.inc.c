// castle_cannon_grate.inc.c
#include "game/randomizer.h"

void bhv_castle_cannon_grate_init(void) {
    if (save_file_get_total_star_count(gCurrSaveFileNum - 1, COURSE_MIN - 1, COURSE_MAX - 1) >= Randomizer_gRequiredStars[Randomizer_STAR_REQ_OW3]) {
        o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
    }
}
