#include <ultra64.h>
#include <string.h>

#include "sm64.h"
#include "engine/behavior_script.h"
#include "behavior_data.h"
#include "randomizer.h"
#include "engine/surface_collision.h"
#include "engine/surface_load.h"
#include "memory.h"
#include "area.h"
#include "camera.h"
#include "mario.h"
#include "object_list_processor.h"
#include "engine/math_util.h"
#include "ingame_menu.h"
#include "print.h"
#include "menu/file_select.h"
#include "save_file.h"
#include "buffers/buffers.h"
#include "segment2.h"
#include "game/emutest.h"

u32 Randomizer_gGameSeed;

u8 Randomizer_gIsSetSeed = FALSE;

u8 Randomizer_gIgnoreCollisionDistance = FALSE; // hacky

u8 Randomizer_gNumDynamicAvoidancePoints = 0;
struct Randomizer_AvoidancePoint Randomizer_gDynamicAvoidancePoints[50];

s32 Randomizer_curPreset;
struct Randomizer_OptionsSettings Randomizer_gOptionsSettings;

#include "randomizer_data.h"

u8 Randomizer_gRandomSongs[19] = { SEQ_MENU_TITLE_SCREEN, SEQ_LEVEL_GRASS,          SEQ_LEVEL_INSIDE_CASTLE,    SEQ_LEVEL_WATER,
                      SEQ_LEVEL_HOT,         SEQ_LEVEL_BOSS_KOOPA,     SEQ_LEVEL_SNOW,             SEQ_LEVEL_SLIDE,
                      SEQ_LEVEL_SPOOKY,      SEQ_EVENT_PIRANHA_PLANT,  SEQ_LEVEL_UNDERGROUND,      SEQ_EVENT_POWERUP, 
                      SEQ_EVENT_METAL_CAP,   SEQ_LEVEL_KOOPA_ROAD,     SEQ_EVENT_MERRY_GO_ROUND,   SEQ_EVENT_BOSS,
                      SEQ_EVENT_ENDLESS_STAIRS, SEQ_LEVEL_BOSS_KOOPA_FINAL, SEQ_MENU_FILE_SELECT, 
};

struct Randomizer_nodeInfo Randomizer_gLevelWarps[] = {
    // Level        Area  0xF0  0xF1
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { LEVEL_CASTLE_GROUNDS, 1, 42, 43, }, // BBH
    { LEVEL_CASTLE_GROUNDS, 1, 27, 28, }, // CCM
    { LEVEL_CASTLE_GROUNDS, 1, 0, 245, }, // Inside Castle
    { LEVEL_CASTLE_COURTYARD, 1, 21, 22, }, // HMC
    { LEVEL_CASTLE_COURTYARD, 1, 5, 6, }, // SSL
    { LEVEL_CASTLE_GROUNDS, 1, 21, 22, }, // BoB
    { LEVEL_CASTLE_COURTYARD, 1, 12, 13, }, // SL
    { LEVEL_CASTLE_COURTYARD, 1, 36, 37, }, // WDW
    { LEVEL_CASTLE_GROUNDS, 1, 29, 30, }, // JRB
    { LEVEL_CASTLE_COURTYARD, 1, 42, 43, }, // THI
    { LEVEL_CASTLE, 1, 6, 7, }, // TTC
    { LEVEL_CASTLE, 1, 9, 11, }, // RR
    { LEVEL_CASTLE_GROUNDS, 1, 0, 201, }, // Castle Grounds
    { LEVEL_CASTLE_GROUNDS, 1, 39, 40, }, // BitDW
    { LEVEL_CASTLE_GROUNDS, 1, 42, 43, }, // VCutM
    { LEVEL_CASTLE_COURTYARD, 1, 27, 28, }, // BitFS
    { LEVEL_CASTLE_COURTYARD, 0x01, 15, 16, }, // SA
    { LEVEL_CASTLE, 1, 3, 4, }, // BitS
    { LEVEL_CASTLE_COURTYARD, 1, 24, 25, }, // LLL
    { LEVEL_CASTLE_COURTYARD, 1, 18, 19, }, // DDD
    { LEVEL_CASTLE_GROUNDS, 1, 36, 37, }, // WF
    { LEVEL_CASTLE_GROUNDS, 1, 9, 201, }, // Cake
    { LEVEL_CASTLE_GROUNDS, 1, 0, 201, }, // Castle Courtyard
    { LEVEL_CASTLE_GROUNDS, 1, 45, 46, }, // PSS
    { LEVEL_CASTLE_GROUNDS, 1, 27, 28, }, // CotMC
    { LEVEL_CASTLE_COURTYARD, 1, 39, 40, }, // TotWC
    { 0, 0, 0, 0, },
    { LEVEL_CASTLE_GROUNDS, 1, 9, 201, }, // WMotR
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { LEVEL_CASTLE_COURTYARD, 1, 39, 40, }, // TTM
};

char *presetStrings[] = {
    "Default",
    "", // default plus
    "Extreme",
    "Easy",
    "New Game",
    "Calm",
    "Sprint"
};

s32 Randomizer_curPreset = 0;

struct Randomizer_OptionsSettings Randomizer_gPresets[] = {
    {{{0, /* pad */ 0, 1, 0, 1, 0, 1, 1, 1, 1,  7, 0}}, {{0, 0, 0, 0, 0}}},
    {{{0, /* pad */ 0, 1, 0, 1, 0, 1, 1, 1, 1,  7, 0}}, {{1, 1, 1, 1, 1}}},
    {{{0, /* pad */ 1, 1, 1, 1, 0, 2, 1, 1, 1, 10, 0}}, {{2, 1, 1, 1, 2}}},
    {{{0, /* pad */ 0, 1, 0, 1, 1, 0, 0, 2, 0,  5, 0}}, {{0, 0, 1, 0, 0}}},
    {{{0, /* pad */ 1, 1, 1, 1, 1, 1, 0, 1, 0,  8, 0}}, {{1, 1, 0, 1, 3}}},
    {{{0, /* pad */ 1, 0, 0, 0, 0, 1, 0, 0, 0,  7, 0}}, {{0, 0, 0, 0, 0}}},
    {{{0, /* pad */ 0, 1, 0, 1, 2, 1, 1, 2, 1,  3, 0}}, {{1, 0, 1, 0, 1}}},
};

unsigned char textVersion2[] = { "Randomizer 0.1" };

static void print_generic_text_ascii_buf(s16 x, s16 y, const char *str) {
    return print_text_aligned(x, y, str, TEXT_ALIGN_LEFT);
}

void Randomizer_print_seed_and_options_data(void) {
    char buf[20];
    s32 ypos = (gIsConsole ? 10 : 4);
    u32 i;
    gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
    gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, gDialogTextAlpha);
    
    sprintf(buf, "%s Seed", (Randomizer_gIsSetSeed ? "Set" : "Random"));
    print_generic_text_ascii_buf(8, ypos + 28, buf);
    sprintf(buf, "Seed\xE6 %05d", Randomizer_gGameSeed);
    print_generic_text_ascii_buf(8, ypos + 14, buf);
    
    for (i = 0; i < ARRAY_COUNT(Randomizer_gPresets); i++) {
        if (Randomizer_gOptionsSettings.gameplay.w == Randomizer_gPresets[i].gameplay.w) {
            sprintf(buf, "Preset\xE6 %s", presetStrings[i]);
            print_generic_text_ascii_buf(8,ypos,buf);
            goto presetFound; // don't kill me please
        }
    }
    
    sprintf(buf, "Settings ID\xE6 %d", Randomizer_gOptionsSettings.gameplay.w);
    print_generic_text_ascii_buf(8,ypos,buf);

presetFound:
    print_generic_string_aligned(310 / 2, ypos, buf, TEXT_ALIGN_CENTER);

    gSPDisplayList(gDisplayListHead++, dl_ia_text_end);
}

static s32 find_floor_slipperiness(struct Surface *floor) {
    s32 floorClass = SURFACE_CLASS_DEFAULT;

    if (floor) {
        switch (floor->type) {
            case SURFACE_NOT_SLIPPERY:
            case SURFACE_HARD_NOT_SLIPPERY:
                floorClass = SURFACE_CLASS_NOT_SLIPPERY;
                break;

            case SURFACE_SLIPPERY:
            case SURFACE_NOISE_SLIPPERY:
            case SURFACE_HARD_SLIPPERY:
            case SURFACE_NO_CAM_COL_SLIPPERY:
                floorClass = SURFACE_CLASS_SLIPPERY;
                break;

            case SURFACE_VERY_SLIPPERY:
            case SURFACE_ICE:
            case SURFACE_HARD_VERY_SLIPPERY:
            case SURFACE_NOISE_VERY_SLIPPERY:
                floorClass = SURFACE_CLASS_VERY_SLIPPERY;
                break;
        }
    }

    return floorClass;
}

static u8 rando_floors_general(TerrainData type)
{
    switch (type)
    {
        case SURFACE_NULL:
        case SURFACE_DEFAULT:                  // Environment default
        case SURFACE_0004:                     // Unused, has no function and has parameters
        case SURFACE_VERY_SLIPPERY:            // Very slippery, mostly used for slides
        case SURFACE_SLIPPERY:                 // Slippery
        case SURFACE_NOT_SLIPPERY:             // Non-slippery, climbable
        case SURFACE_HARD:                     // Hard floor (Always has fall damage)
        case SURFACE_HARD_SLIPPERY:            // Hard and slippery (Always has fall damage)
        case SURFACE_HARD_VERY_SLIPPERY:       // Hard and very slippery (Always has fall damage)
        case SURFACE_HARD_NOT_SLIPPERY:        // Hard and Non-slippery (Always has fall damage)
        case SURFACE_ICE:                      // Slippery Ice, in snow levels and THI's water floor
        case SURFACE_HORIZONTAL_WIND:          // Horizontal wind, has parameters
        case SURFACE_FLOWING_WATER:            // Water (flowing), has parameters
        case SURFACE_MGR_MUSIC:                // Plays the Merry go round music, see handle_merry_go_round_music in bbh_merry_go_round.inc.c for more details
        case SURFACE_NOISE_DEFAULT:            // Default floor with noise
        case SURFACE_NOISE_SLIPPERY:           // Slippery floor with noise
        case SURFACE_NOISE_VERY_SLIPPERY:      // Very slippery floor with noise, used in CCM
        case SURFACE_NOISE_VERY_SLIPPERY_73:   // Very slippery floor with noise, unused
        case SURFACE_NOISE_VERY_SLIPPERY_74:   // Very slippery floor with noise, unused
        case SURFACE_CLOSE_CAMERA:             // Close camera
        case SURFACE_WATER:                    // Water, has no action, used on some waterboxes below
        case SURFACE_SHALLOW_QUICKSAND:        // Shallow Quicksand (depth of 10 units)
        case SURFACE_LOOK_UP_WARP:             // Look up and warp (Wing cap entrance)
        case SURFACE_TIMER_START:              // Timer start (Peach's secret slide)
        case SURFACE_TIMER_END:                // Timer stop (Peach's secret slide)
        case SURFACE_BOSS_FIGHT_CAMERA:        // Wide camera for BOB and WF bosses
        case SURFACE_CAMERA_FREE_ROAM:         // Free roam camera for THI and TTC
        case SURFACE_THI3_WALLKICK:            // Surface where there's a wall kick section in THI 3rd area, has no action defined
        case SURFACE_CAMERA_8_DIR:             // Surface that enables far camera for platforms, used in THI
        case SURFACE_CAMERA_MIDDLE:            // Surface camera that returns to the middle, used on the 4 pillars of SSL
        case SURFACE_CAMERA_ROTATE_RIGHT:      // Surface camera that rotates to the right (Bowser 1 & THI)
        case SURFACE_CAMERA_ROTATE_LEFT:       // Surface camera that rotates to the left (BOB & TTM)
        case SURFACE_NO_CAM_COLLISION:         // Surface with no cam collision flag
        case SURFACE_NO_CAM_COLLISION_77:      // Surface with no cam collision flag, unused
        case SURFACE_NO_CAM_COL_VERY_SLIPPERY: // Surface with no cam collision flag, very slippery with noise (THI)
        case SURFACE_NO_CAM_COL_SLIPPERY:      // Surface with no cam collision flag, slippery with noise (CCM, PSS and TTM slides)
        case SURFACE_TTM_VINES:                // TTM vines, has no action defined
        case SURFACE_SWITCH:                   // Surface with no cam collision flag, non-slippery with noise, used by switches and Dorrie
        case SURFACE_VANISH_CAP_WALLS:         // Vanish cap walls, pass through them with Vanish Cap
        case SURFACE_WALL_MISC:                // Used for some walls, Cannon to adjust the camera, and some objects like Warp Pipe
        case SURFACE_HANGABLE:                 // Ceiling that Mario can climb on
        case SURFACE_SLOW:                     // Slow down Mario, unused
            return 1;
        default:
            return 0;
    }
}

static u8 rando_floors_hard(TerrainData type)
{
    switch (type)
    {
        case SURFACE_NULL:
        case SURFACE_DEFAULT:                  // Environment default
        case SURFACE_0004:                     // Unused, has no function and has parameters
        case SURFACE_VERY_SLIPPERY:            // Very slippery, mostly used for slides
        case SURFACE_SLIPPERY:                 // Slippery
        case SURFACE_NOT_SLIPPERY:             // Non-slippery, climbable
        case SURFACE_HARD:                     // Hard floor (Always has fall damage)
        case SURFACE_HARD_SLIPPERY:            // Hard and slippery (Always has fall damage)
        case SURFACE_HARD_VERY_SLIPPERY:       // Hard and very slippery (Always has fall damage)
        case SURFACE_HARD_NOT_SLIPPERY:        // Hard and Non-slippery (Always has fall damage)
        case SURFACE_ICE:                      // Slippery Ice, in snow levels and THI's water floor
        case SURFACE_HORIZONTAL_WIND:          // Horizontal wind, has parameters
        case SURFACE_FLOWING_WATER:            // Water (flowing), has parameters
        case SURFACE_MGR_MUSIC:                // Plays the Merry go round music, see handle_merry_go_round_music in bbh_merry_go_round.inc.c for more details
        case SURFACE_NOISE_DEFAULT:            // Default floor with noise
        case SURFACE_NOISE_SLIPPERY:           // Slippery floor with noise
        case SURFACE_NOISE_VERY_SLIPPERY:      // Very slippery floor with noise, used in CCM
        case SURFACE_NOISE_VERY_SLIPPERY_73:   // Very slippery floor with noise, unused
        case SURFACE_NOISE_VERY_SLIPPERY_74:   // Very slippery floor with noise, unused
        case SURFACE_CLOSE_CAMERA:             // Close camera
        case SURFACE_WATER:                    // Water, has no action, used on some waterboxes below
        case SURFACE_SHALLOW_QUICKSAND:        // Shallow Quicksand (depth of 10 units)
        case SURFACE_LOOK_UP_WARP:             // Look up and warp (Wing cap entrance)
        case SURFACE_TIMER_START:              // Timer start (Peach's secret slide)
        case SURFACE_TIMER_END:                // Timer stop (Peach's secret slide)
        case SURFACE_BOSS_FIGHT_CAMERA:        // Wide camera for BOB and WF bosses
        case SURFACE_CAMERA_FREE_ROAM:         // Free roam camera for THI and TTC
        case SURFACE_THI3_WALLKICK:            // Surface where there's a wall kick section in THI 3rd area, has no action defined
        case SURFACE_CAMERA_8_DIR:             // Surface that enables far camera for platforms, used in THI
        case SURFACE_CAMERA_MIDDLE:            // Surface camera that returns to the middle, used on the 4 pillars of SSL
        case SURFACE_CAMERA_ROTATE_RIGHT:      // Surface camera that rotates to the right (Bowser 1 & THI)
        case SURFACE_CAMERA_ROTATE_LEFT:       // Surface camera that rotates to the left (BOB & TTM)
        case SURFACE_NO_CAM_COLLISION:         // Surface with no cam collision flag
        case SURFACE_NO_CAM_COLLISION_77:      // Surface with no cam collision flag, unused
        case SURFACE_NO_CAM_COL_VERY_SLIPPERY: // Surface with no cam collision flag, very slippery with noise (THI)
        case SURFACE_NO_CAM_COL_SLIPPERY:      // Surface with no cam collision flag, slippery with noise (CCM, PSS and TTM slides)
        case SURFACE_TTM_VINES:                // TTM vines, has no action defined
        case SURFACE_SWITCH:                   // Surface with no cam collision flag, non-slippery with noise, used by switches and Dorrie
        case SURFACE_VANISH_CAP_WALLS:         // Vanish cap walls, pass through them with Vanish Cap
        case SURFACE_WALL_MISC:                // Used for some walls, Cannon to adjust the camera, and some objects like Warp Pipe
        case SURFACE_HANGABLE:                 // Ceiling that Mario can climb on
        case SURFACE_SLOW:                     // Slow down Mario, unused

        case SURFACE_BURNING:                  // Lava / Frostbite (in SL), but is used mostly for Lava
        case SURFACE_DEEP_QUICKSAND:           // Quicksand (lethal, slow, depth of 160 units)
        case SURFACE_INSTANT_QUICKSAND:        // Quicksand (lethal, instant)
        case SURFACE_DEEP_MOVING_QUICKSAND:    // Moving quicksand (flowing, depth of 160 units)
        case SURFACE_SHALLOW_MOVING_QUICKSAND: // Moving quicksand (flowing, depth of 25 units)
        case SURFACE_QUICKSAND:                // Moving quicksand (60 units)
        case SURFACE_MOVING_QUICKSAND:         // Moving quicksand (flowing, depth of 60 units)
        case SURFACE_INSTANT_MOVING_QUICKSAND: // Quicksand (lethal, flowing)
            return 1;
        default:
            return 0;
    }
}

static u8 is_floor_safe(struct Surface *floor, u8 floorSafeLevel,
                        u32 randPosFlags) { // Checks if floor triangle can be spawned on
    s32 slipperiness;
    f32 norm;

    if (floor->flags & SURFACE_FLAG_DYNAMIC)
        return FALSE; // grounded objects / DDD objects can't spawn on platforms

    switch(Randomizer_gOptionsSettings.gameplay.s.safeSpawns){
        case Randomizer_SPAWN_SAFETY_SAFE:
            norm = 0.85f;
            break;
        case Randomizer_SPAWN_SAFETY_HARD:
            norm = 0.3f;
            break;
        default:
            norm = 0.7f;
    }

    if ((floorSafeLevel == Randomizer_FLOOR_SAFETY_HIGH) || (randPosFlags & RAND_TYPE_SAFE)) {
        norm = 0.95f;
    }

    if (floor->normal.y <= norm) {
        return FALSE;
    }

    slipperiness = find_floor_slipperiness(floor);
    if ((randPosFlags & RAND_TYPE_SAFE) && (floorSafeLevel == Randomizer_FLOOR_SAFETY_HIGH)
        && ((slipperiness == SURFACE_CLASS_SLIPPERY) || (slipperiness == SURFACE_CLASS_VERY_SLIPPERY))) {

        // This code kills some spawns, assuming the most slippery case. This code would
        // probably be better to refactor based off slipperiness in general.
        if (floor->normal.y <= 0.99f) {
            return FALSE; // Don't spawn on slippery surfaces if you are a warp or safe object
        }
    }

    if (rando_floors_general(floor->type)) {
        return TRUE;
    }

    if ((Randomizer_gOptionsSettings.gameplay.s.safeSpawns == Randomizer_SPAWN_SAFETY_HARD) && (floorSafeLevel == Randomizer_FLOOR_SAFETY_LOW) && !(randPosFlags & RAND_TYPE_SAFE)) {
        if (rando_floors_hard(floor->type)) {
            return TRUE;
        }
    }

    return FALSE;
}

// Checks if near a specific avoidance point
static u32 check_avoidance_point(Vec3s pos, struct Object *obj, const struct Randomizer_AvoidancePoint *avoidancePoint) {
    void *behavior = segmented_to_virtual(avoidancePoint->behavior);
        
    if(((avoidancePoint->safety == Randomizer_AVOIDANCE_SAFETY_ALL) 
    || ((avoidancePoint->safety == Randomizer_AVOIDANCE_SAFETY_MED) && (Randomizer_gOptionsSettings.gameplay.s.safeSpawns == Randomizer_SPAWN_SAFETY_SAFE))
    || ((avoidancePoint->safety == Randomizer_AVOIDANCE_SAFETY_HARD) && (Randomizer_gOptionsSettings.gameplay.s.safeSpawns != Randomizer_SPAWN_SAFETY_HARD)))){
    } else {
        return FALSE;
    }

    if (behavior != segmented_to_virtual(bhvStub) && obj->behavior != behavior) {
        return FALSE;
    }

    if ((sqr(pos[0] - avoidancePoint->pos[0]) + sqr(pos[2] - avoidancePoint->pos[2]) < sqr(avoidancePoint->radius))
        && (pos[1] > avoidancePoint->pos[1]) && (pos[1] < avoidancePoint->pos[1] + avoidancePoint->height)) {
        return TRUE;
    }
    return FALSE;
}

// Checks if near any avoidance point
static u32 is_in_avoidance_point(Vec3s pos, const struct Randomizer_AreaParams *areaParams, struct Object *obj) {
    const struct Randomizer_AvoidancePoint *avoidancePoint;

    for (u32 i = 0; i < areaParams->numAvoidancePoints; i++) {
        avoidancePoint = &(*areaParams->avoidancePoints)[i];
        if (check_avoidance_point(pos, obj, avoidancePoint)) {
            return TRUE;
        }
    }
    for (u32 i = 0; i < Randomizer_gNumDynamicAvoidancePoints; i++) {
        if (check_avoidance_point(pos, obj, &Randomizer_gDynamicAvoidancePoints[i])) {
            return TRUE;
        }
    }

    return FALSE;
}

u32 Randomizer_raycast_wall_check(Vec3s pos) {
    s16 yaw = 0;
    struct Surface *surf;
    Vec3f checkPos;
    vec3_copy_y_off(checkPos, pos, 50.0f);
    Vec3f hitPos;

    for (u32 i = 0; i < 8; i++) {
        Vec3f rayDir;
        vec3f_set(rayDir, 0x10000 * sins(yaw), 0, 0x10000 * coss(yaw));

        find_surface_on_ray(checkPos, rayDir, &surf, hitPos,
            (RAYCAST_FIND_FLOOR | RAYCAST_FIND_WALL | RAYCAST_FIND_CEIL));

        if (surf != NULL) {
            f32 det = vec3f_dot(&surf->normal.x, rayDir);
            if (det > 0) {
                return FALSE;
            }
        }

        yaw += 0x2000;
    }
    return TRUE;
}

static void vec3s_resolve_wall_collisions(Vec3s pos, f32 radius) {
    Vec3f pos2;
    
    vec3s_to_vec3f(pos2, pos);
    f32_find_wall_collision(&pos2[0], &pos2[1], &pos2[2], 0.0f, radius);
    vec3f_to_vec3s(pos, pos2);
}

void Randomizer_create_dynamic_avoidance_point(Vec3f pos, f32 radius, f32 height, f32 downOffset) {
    struct Randomizer_AvoidancePoint *newPoint = &Randomizer_gDynamicAvoidancePoints[Randomizer_gNumDynamicAvoidancePoints];
    newPoint->pos[0] = pos[0];
    newPoint->pos[1] = pos[1] - downOffset;
    newPoint->pos[2] = pos[2];
    newPoint->radius = radius;
    newPoint->height = height;
    newPoint->safety = Randomizer_AVOIDANCE_SAFETY_ALL;
    newPoint->behavior = bhvStub;
    Randomizer_gNumDynamicAvoidancePoints++;
}

void Randomizer_get_safe_position(struct Object *obj, Vec3s pos, f32 minHeightRange, f32 maxHeightRange, tinymt32_t *randomState,
                       u8 floorSafeLevel, u32 randPosFlags) {
    const struct Randomizer_AreaParams *areaParams = &(*Randomizer_sLevelParams[gCurrLevelNum - 4])[gCurrAreaIndex - 1];
    f32 minX, maxX, minY, maxY, minZ, maxZ, minHeight, maxHeight, waterLevel, lowFloorHeight, cHeight,
        highFloorHeight;
    u32 objCanBeUnderwater;
    struct Surface *lowFloor, *ceil, *highFloor;

    f32 wallRadius = 50.f;
    if (randPosFlags & RAND_TYPE_SPAWN_FAR_FROM_WALLS) {
        wallRadius = 300.f;
    }

    if (areaParams == NULL) {
        pos[0] = 0;
        pos[1] = 5000;
        pos[2] = 0;
        return;
    }

    minX = areaParams->minX;
    maxX = areaParams->maxX;
    minY = areaParams->minY;
    maxY = areaParams->maxY;
    minZ = areaParams->minZ;
    maxZ = areaParams->maxZ;

    if (Randomizer_gOptionsSettings.gameplay.s.nonstopMode == 1) {
        if ((obj->behavior == segmented_to_virtual(bhvStar))
         || (obj->behavior == segmented_to_virtual(bhvStarSpawnCoordinates))
         || (obj->behavior == segmented_to_virtual(bhvHiddenRedCoinStar))
         || (obj->behavior == segmented_to_virtual(bhvHiddenStar))) {
            floorSafeLevel = Randomizer_FLOOR_SAFETY_MEDIUM;
            randPosFlags |= RAND_TYPE_SAFE;
        }
    }

    while (TRUE) {
        u32 dangerShiftedOverHighFloor = FALSE;

        // Generate random position
        pos[0] = Randomizer_get_val_in_range_uniform(minX, maxX, randomState);
        pos[1] = Randomizer_get_val_in_range_uniform(minY, maxY, randomState);
        pos[2] = Randomizer_get_val_in_range_uniform(minZ, maxZ, randomState);

        lowFloorHeight = find_floor(pos[0], pos[1] + 20, pos[2], &lowFloor);

        if (lowFloor == NULL)
            continue;

        int lowDiff = 800;
        if ((pos[1] - lowFloorHeight) > lowDiff)
            continue;

        if (lowFloorHeight + 20 <= maxY) {
            pos[1] = lowFloorHeight + 20;
        }

        // Move out of any walls. This has to be done here because otherwise
        // there's the possibility of being pushed out of the wall into OoB or a ceiling
        vec3s_resolve_wall_collisions(
            pos, wallRadius);

        lowFloorHeight = find_floor(pos[0], pos[1], pos[2], &lowFloor);

        if ((pos[1] - lowFloorHeight) > lowDiff)
            continue;

        pos[1] = lowFloorHeight;

        if (lowFloor == NULL)
            continue;

        if (!is_floor_safe(lowFloor, floorSafeLevel, randPosFlags))
            continue;

        // Snap to ground and check if safe
        objCanBeUnderwater =
            (randPosFlags
                 & (RAND_TYPE_CAN_BE_UNDERWATER | RAND_TYPE_MUST_BE_UNDERWATER));
        waterLevel = find_water_level(pos[0], pos[2]);
        minHeight = pos[1] + minHeightRange;
        maxHeight = pos[1] + maxHeightRange;

        // Let objects spawn anywhere in water
        if (floorSafeLevel != Randomizer_FLOOR_SAFETY_HIGH
            || (randPosFlags & RAND_TYPE_MUST_BE_UNDERWATER)) {
            if ((objCanBeUnderwater && (waterLevel > maxHeight))
                || (randPosFlags & RAND_TYPE_MUST_BE_UNDERWATER))
                maxHeight = waterLevel;
        }

        // For the start warp, always spawn above the water
        if ((obj->behavior == segmented_to_virtual(bhvSpinAirborneWarp)) && (waterLevel > pos[1])) {
            minHeight = waterLevel + minHeightRange;
            maxHeight = waterLevel + maxHeightRange;
        }

#if 0
        // Prevent objects from spawning too high above water in BBH
        if ((gCurrCourseNum == COURSE_BBH) && (pos[1] < waterLevel) && (maxHeight > waterLevel))
            maxHeight = waterLevel + 100.f;
#endif

        // Check if max height has gone above the level bounds
        if (maxHeight > maxY) {
            maxHeight = maxY;
        }

        pos[1] = Randomizer_get_val_in_range_uniform(minHeight, maxHeight, randomState);

        if ((Randomizer_gOptionsSettings.gameplay.s.safeSpawns == Randomizer_SPAWN_SAFETY_HARD) &&
            (floorSafeLevel == Randomizer_FLOOR_SAFETY_LOW) && !(randPosFlags & RAND_TYPE_SAFE)) {
            Vec3f oldPos;
            vec3s_to_vec3f(oldPos, pos);
            pos[0] += Randomizer_get_val_in_range_uniform(-200, 200, randomState);
            pos[2] += Randomizer_get_val_in_range_uniform(-200, 200, randomState);

            vec3s_resolve_wall_collisions(
                pos, wallRadius);

            // Make sure it doesnt shift through a surface
            Vec3f rayDir;
            vec3s_to_vec3f(rayDir, pos);
            vec3f_sub(rayDir, oldPos);
            struct Surface *surf;
            Vec3f hitPos;
            find_surface_on_ray(oldPos, rayDir, &surf, hitPos,
            (RAYCAST_FIND_FLOOR | RAYCAST_FIND_WALL | RAYCAST_FIND_CEIL));

            if (surf != NULL) {
                continue;
            }
            
            waterLevel = find_water_level(pos[0], pos[2]);

            lowFloorHeight = find_floor(pos[0], pos[1], pos[2], &lowFloor);
            if (lowFloor == NULL) {
                continue;
            }
            if ((pos[1] - lowFloorHeight) > 800.f) {
                dangerShiftedOverHighFloor = TRUE;
            }
        }

        // Start checking if position is valid

        // Ceiling check
        cHeight = find_ceil(pos[0], lowFloorHeight + 80, pos[2], &ceil);

        if (pos[1] > cHeight - 100.f) // If in a ceiling, cancel spawn
            continue;

        if (dangerShiftedOverHighFloor & (pos[1] > cHeight - 200.f)) // If no ground nearby and too close to the ceiling
            continue;

        // Floor Check
        highFloorHeight = find_floor(pos[0], cHeight - 80, pos[2],
                                     &highFloor); // Find floor under object assuming 80 units of space

        if ((highFloorHeight > (pos[1] + 20))
            && ((highFloorHeight - pos[1])
                < 1500)) // If under floor and not large distance, deny height
            continue;

        if ((pos[1] - highFloorHeight) < (minHeightRange - 50.f)) {
            continue;
        }

        if (!objCanBeUnderwater && (waterLevel > pos[1]))
            continue;

        if ((randPosFlags & RAND_TYPE_MUST_BE_UNDERWATER) && (waterLevel < pos[1]))
            continue;

#if 0
        if (randPosFlags & RAND_TYPE_LIMITED_BBH_HMC_SPAWNS) {
            if ((gCurrCourseNum == COURSE_BBH) && (lowFloor->room == 9)) {
                continue;
            } else if ((gCurrCourseNum == COURSE_HMC) && (lowFloor->room == 8)) {
                continue;
            }
        }
#endif

        if (is_in_avoidance_point(pos, areaParams, obj))
            continue;

        // Wall Check
        if (!Randomizer_raycast_wall_check(pos))
            continue;

        // Spawn avoidance point if needed
        if ((randPosFlags & RAND_TYPE_CREATE_AVOIDANCE_POINT) && (Randomizer_gNumDynamicAvoidancePoints < 50)) {
            Vec3f fpos;
            vec3_copy(fpos, pos);
            Randomizer_create_dynamic_avoidance_point(fpos, 100.f, 200.f, 50.f);
        }

        return;
    }
}

// Only uniform if used for floats. [min, max)
f32 Randomizer_get_val_in_range_uniform(f32 min, f32 max, tinymt32_t *randomState) {
    if (min > max)
        return min;
    return (tinymt32_generate_float(randomState) * (max - min)) + min;
}

static u16 calulate_star_total(u32 level) {
    switch (Randomizer_gWarpDestinations[level]) {
        case LEVEL_SA:
        case LEVEL_PSS:
            return 2;
        
        case LEVEL_WMOTR:
            return 1;
        
        case LEVEL_BBH:
            return 7 + calulate_star_total(LEVEL_VCUTM);
        case LEVEL_CCM:
            return 7 + calulate_star_total(LEVEL_COTMC);
        case LEVEL_TTM:
            return 7 + calulate_star_total(LEVEL_TOTWC);

        case LEVEL_BITDW:
        case LEVEL_TOTWC:
        case LEVEL_COTMC:
        case LEVEL_BITFS:
        case LEVEL_VCUTM:
        case LEVEL_BITS:
        case LEVEL_ENDING:
            return 1;

        default:
            return 7;
    }
}

// Get the maximum requirement for a star door based on:
// BitS requirement
// If no keep structure, layer (0 for easily accessible doors, 1 for doors behind other doors)
// If keep structure, section (0 for lobby, 1 for basement, 2 for upstairs)
// Min factor (usually / 2 for important doors and / 3 for most )
// Maximum stars available at this point, if lower than regular max
static u8 get_star_requirement(u8 layer, u8 section, u8 maxAvailable, u8 factor, tinymt32_t *randomState) {
    u8 bitsStars = Randomizer_gStarDoorReqLUT[Randomizer_gOptionsSettings.gameplay.s.starDoorRequirement];
    u8 maxStars = 0;
    u8 starReq;

    if (bitsStars == 0) bitsStars = 80;

    if (Randomizer_gOptionsSettings.gameplay.s.keepStructure) {
        switch (section) {
        case 0:
            maxStars = (u8)(bitsStars*0.2f); // lobby
            break;
        case 1:
            maxStars = (u8)(bitsStars*0.4f); // basement
            break;
        case 2:
            maxStars = (u8)(bitsStars*0.7f); // upstairs
        }
    } else {
        if (layer == 0) {
            maxStars = (u8)(bitsStars*0.35f); // layer 1
        } else {
            maxStars = (u8)(bitsStars*0.8f); // layer 2
        }
    }
    starReq = Randomizer_get_val_in_range_uniform(maxStars / factor, maxStars, randomState);
    return MIN(starReq, Randomizer_get_val_in_range_uniform(MAX(maxAvailable - 5, 0), maxAvailable, randomState));
}

static void randomize_star_doors() {
    tinymt32_t randomState;
    tinymt32_init(&randomState, Randomizer_gGameSeed);

    u16 starTotal = calulate_star_total(LEVEL_BOB) + calulate_star_total(LEVEL_JRB) + calulate_star_total(LEVEL_WF) + calulate_star_total(LEVEL_PSS);
    Randomizer_gRequiredStars[Randomizer_STAR_REQ_CH] = get_star_requirement(0, 0, starTotal, 3, NULL);
    Randomizer_gRequiredStars[Randomizer_STAR_REQ_GG] = get_star_requirement(0, 0, starTotal, 2, NULL);

    starTotal += calulate_star_total(LEVEL_CCM);
    starTotal += calulate_star_total(LEVEL_BBH);

    Randomizer_gRequiredStars[Randomizer_STAR_REQ_B1] = get_star_requirement(1, 0, starTotal, 2, NULL);

    starTotal += calulate_star_total(LEVEL_BITDW);
    
    Randomizer_gRequiredStars[Randomizer_STAR_REQ_K1] = get_star_requirement(1, 0, starTotal, 3, NULL);

    starTotal += calulate_star_total(LEVEL_SSL);
    starTotal += calulate_star_total(LEVEL_SL);
    
    Randomizer_gRequiredStars[Randomizer_STAR_REQ_MMM]      = get_star_requirement(0, 1, starTotal, 2, NULL);

    starTotal += calulate_star_total(LEVEL_DDD);

    starTotal += calulate_star_total(LEVEL_HMC);

    Randomizer_gRequiredStars[Randomizer_STAR_REQ_KC] = get_star_requirement(1, 1, starTotal, 2, NULL);

    starTotal += calulate_star_total(LEVEL_LLL);
    
    Randomizer_gRequiredStars[Randomizer_STAR_REQ_B2] = get_star_requirement(1, 1, starTotal, 2, NULL);

    starTotal += calulate_star_total(LEVEL_BITFS);

    Randomizer_gRequiredStars[Randomizer_STAR_REQ_K2] = get_star_requirement(1, 1, starTotal, 2, NULL);

    starTotal += calulate_star_total(LEVEL_TTM);
    starTotal += calulate_star_total(LEVEL_THI);
    starTotal += calulate_star_total(LEVEL_WDW);
    
    Randomizer_gRequiredStars[Randomizer_STAR_REQ_OW3] = get_star_requirement(0, 2, starTotal, 2, NULL);

    if (Randomizer_gOptionsSettings.gameplay.s.keepStructure) {
        Randomizer_gRequiredStars[Randomizer_STAR_REQ_K1] = 0;
        Randomizer_gRequiredStars[Randomizer_STAR_REQ_K2] = 0;
    }
}

static void init_required_stars() {
    u32 i;
    switch (Randomizer_gOptionsSettings.gameplay.s.randomStarDoorCounts) {
        case 1:
            randomize_star_doors();
            break;
        case 0:
            for (i = 0; i < sizeof(Randomizer_gRequiredStars); i++) {
                Randomizer_gRequiredStars[i] = sDefaultStarReqs[i];
            }
            break;
        case 2:
            for (i = 0; i < sizeof(Randomizer_gRequiredStars); i++) {
                Randomizer_gRequiredStars[i] = 0;
            }
            break;
    }
    Randomizer_gRequiredStars[Randomizer_STAR_REQ_B3] = Randomizer_gStarDoorReqLUT[Randomizer_gOptionsSettings.gameplay.s.starDoorRequirement]; // Final Bowser Door is special.
}

static u8 pick_random_u8(const u8* arr, size_t arrSize, tinymt32_t *randomState)
{
    u8 index = Randomizer_get_val_in_range_uniform(0, arrSize, randomState);
    return arr[index];
}

static void shuffle_warp_pool(const u8* warpPool, size_t warpPoolSize, tinymt32_t *randomState)
{
    for (int i = warpPoolSize - 1; i > 0; i--) {
        u8 i1 = warpPool[i];
        u8 i2 = pick_random_u8(warpPool, i + 1, randomState);

        u8 tmp = Randomizer_gWarpDestinations[i1];
        Randomizer_gWarpDestinations[i1] = Randomizer_gWarpDestinations[i2];
        Randomizer_gWarpDestinations[i2] = tmp;
    }
}

u8 Randomizer_get_nonrandom_level(u8 currLevel)
{
    if (currLevel == LEVEL_BOWSER_1) {
        currLevel = LEVEL_BITDW;
    } else if (currLevel == LEVEL_BOWSER_2) {
        currLevel = LEVEL_BITFS;
    }

    for (int i = 0; i < ARRAY_COUNT(Randomizer_gWarpDestinations); i++) {
        if (Randomizer_gWarpDestinations[i] == currLevel) {
            return i;
        }
    }

    return 0;
}

static int arr_have(const u8* arr, size_t arrSize, u8 val)
{
    for (size_t i = 0; i < arrSize; i++) {
        if (arr[i] == val) {
            return 1;
        }
    }
    return 0;
}

static void fixup_warps(u8 forLevel, const u8* restrictions, size_t restrictionsSize, tinymt32_t *randomState) {
    u8 lvl = Randomizer_get_nonrandom_level(forLevel);
    if (!arr_have(restrictions, restrictionsSize, lvl)) {
        u8 newLvl = pick_random_u8(restrictions, restrictionsSize, randomState);
        u8 tmp = Randomizer_gWarpDestinations[newLvl];
        Randomizer_gWarpDestinations[newLvl] = Randomizer_gWarpDestinations[lvl];
        Randomizer_gWarpDestinations[lvl] = tmp;
    }
}

static void init_warp_scramble() {
    tinymt32_t randomState;
    tinymt32_init(&randomState, Randomizer_gGameSeed);
    for (int i = 0; i < ARRAY_COUNT(Randomizer_gWarpDestinations); i++)
        Randomizer_gWarpDestinations[i] = gWarpDestinationsStatic[i];

    shuffle_warp_pool(sWarpPool0, ARRAY_SIZE(sWarpPool0), &randomState);
    shuffle_warp_pool(sWarpPool1, ARRAY_SIZE(sWarpPool1), &randomState);
    if (Randomizer_gOptionsSettings.gameplay.s.keepStructure && !Randomizer_gOptionsSettings.gameplay.s.adjustedExits)
    {
        fixup_warps(LEVEL_BOWSER_1, sWarpsPreB1, ARRAY_SIZE(sWarpsPreB1), &randomState);
        fixup_warps(LEVEL_BOWSER_2, sWarpsPreB2, ARRAY_SIZE(sWarpsPreB2), &randomState);
    }
}

extern void save_main_menu_data(void);
void Randomizer_init_randomizer(s32 fileNum) {
    save_main_menu_data();
    save_file_set_seed_and_options(fileNum);
    init_warp_scramble();
    init_required_stars();
}

// stolen from stackoverflow
f32 hue_to_rgb(f32 p, f32 q, f32 t) {
    if (t < 0.f)
        t += 1.f;
    if (t > 1.f)
        t -= 1.f;

    if (t < 1/6.f)
        return p + (q - p) * 6 * t;
    if (t < 1/2.f)
        return q;
    if (t < 2/3.f)
        return p + (q - p) * (2/3.f - t) * 6;
    return p;
}

void hsl_to_rgb(u8 h, u8 s, u8 l, u8 *RGB) {
    f32 r,g,b;
    f32 hf = h / 255.0f;
    f32 sf = s / 255.0f;
    f32 lf = l / 255.0f;

    if (s == 0.f) {
        r = g = b = lf;
    } else {
        f32 q = (lf < 1/2.f) ? (lf * (1 + sf)) : (lf + sf - lf * sf);
        f32 p = 2 * lf - q;
        r = hue_to_rgb(p, q, hf + 1/3.f);
        g = hue_to_rgb(p, q, hf);
        b = hue_to_rgb(p, q, hf - 1/3.f);
    }

    RGB[0] = r * 255;
    RGB[1] = g * 255;
    RGB[2] = b * 255;
}

void get_random_color(u8 *RGB, tinymt32_t *randomState) {
    u32 rand = tinymt32_generate_u32(randomState);
    hsl_to_rgb(rand & 0xFF,
               0xFF,
               ((rand >> 8) & 0x7F) + ((rand >> 16) & 0x7F),
               RGB);
}

void Randomizer_init_star_color(struct Object *star, s32 courseID, s32 starID) {
    s32 index;
    switch (Randomizer_gOptionsSettings.cosmetic.s.starColors) {
        case Randomizer_STAR_COLOR_OFF:
            star->oStarColor = 0xFFFF29;
            return;
        case Randomizer_STAR_COLOR_PER_STAR:
            index = courseID * 8 + starID;
            break;
        case Randomizer_STAR_COLOR_PER_LEVEL:
            index = courseID;
            break;
        case Randomizer_STAR_COLOR_GLOBAL:
            index = 0;
            break;
    }
    tinymt32_t randomState;
    tinymt32_init(&randomState, index * 0x20000 + Randomizer_gGameSeed);

    u8 RGB[3];
    get_random_color(RGB, &randomState);
    star->oStarColor = (RGB[0] << 16) | (RGB[1] << 8) | RGB[2];
}

void set_mario_light(Lights1 *light, u8 r, u8 g, u8 b) {
    light->a.l.col[0] = r / 2;
    light->a.l.col[1] = g / 2;
    light->a.l.col[2] = b / 2;
    light->a.l.colc[0] = light->a.l.col[0];
    light->a.l.colc[1] = light->a.l.col[1];
    light->a.l.colc[2] = light->a.l.col[2];
    light->l[0].l.col[0] = r;
    light->l[0].l.col[1] = g;
    light->l[0].l.col[2] = b;
    light->l[0].l.colc[0] = light->l[0].l.col[0];
    light->l[0].l.colc[1] = light->l[0].l.col[1];
    light->l[0].l.colc[2] = light->l[0].l.col[2];
}

void set_mario_light_random(Lights1 *light, tinymt32_t *randomState) {
    u8 RGB[3];
    get_random_color(RGB, randomState);
    u8 r = RGB[0];
    u8 g = RGB[1];
    u8 b = RGB[2];
    set_mario_light(light, r, g, b);
}

extern Lights1 mario_blue_lights_group;
extern Lights1 mario_red_lights_group;
extern Lights1 mario_white_lights_group;
extern Lights1 mario_brown1_lights_group;
extern Lights1 mario_beige_lights_group;
extern Lights1 mario_brown2_lights_group;
// 4 vertex colors each
extern Vtx coin_seg3_vertex_yellow[];
extern Vtx coin_seg3_vertex_red[];
extern Vtx coin_seg3_vertex_blue[];

void set_coin_color(u8 r, u8 g, u8 b, Vtx *d) {
    Vtx *a = segmented_to_virtual(d);
    u32 i;
    for (i = 0; i < 4; i++) {
        a[0].v.cn[0] = r;
        a[1].v.cn[0] = r;
        a[2].v.cn[0] = r;
        a[3].v.cn[0] = r;
        a[0].v.cn[1] = g;
        a[1].v.cn[1] = g;
        a[2].v.cn[1] = g;
        a[3].v.cn[1] = g;
        a[0].v.cn[2] = b;
        a[1].v.cn[2] = b;
        a[2].v.cn[2] = b;
        a[3].v.cn[2] = b;
    }
}

f32 RMSE(u8 r1, u8 r2, u8 g1, u8 g2, u8 b1, u8 b2) {
    f32 r, g, b;
    r = r1 - r2;
    g = g1 - g2;
    b = b1 - b2;
    return sqrtf(r * r + g * g + b * b);
}

#define MINDIFF 140.f
void Randomizer_set_mario_rando_colors(void) {
    tinymt32_t randomState;

    if (Randomizer_gOptionsSettings.cosmetic.s.marioColors) {
        if (Randomizer_gGameSeed == 2401) {
            set_mario_light(segmented_to_virtual(&mario_red_lights_group), 0, 255, 0);
        } else {
            tinymt32_init(&randomState, Randomizer_gGameSeed);

            set_mario_light_random(segmented_to_virtual(&mario_blue_lights_group), &randomState);
            set_mario_light_random(segmented_to_virtual(&mario_red_lights_group), &randomState);
            set_mario_light_random(segmented_to_virtual(&mario_white_lights_group), &randomState);
            set_mario_light_random(segmented_to_virtual(&mario_brown1_lights_group), &randomState);
            if (Randomizer_gOptionsSettings.cosmetic.s.marioColors == 2) {
                set_mario_light_random(segmented_to_virtual(&mario_beige_lights_group), &randomState);
                set_mario_light_random(segmented_to_virtual(&mario_brown2_lights_group), &randomState);
            }
        }
    }

    if (Randomizer_gOptionsSettings.cosmetic.s.coinsOn) {
        u8 yellows[3];
        u8 reds[3];
        u8 blues[3];
        tinymt32_init(&randomState, Randomizer_gGameSeed + 1);

        get_random_color(yellows, &randomState);
        set_coin_color(yellows[0], yellows[1], yellows[2], coin_seg3_vertex_yellow);

        get_random_color(reds, &randomState);
        while (RMSE(yellows[0], reds[0], yellows[1], reds[1], yellows[2], reds[2]) < MINDIFF) {
            get_random_color(reds, &randomState);
        }
        set_coin_color(reds[0], reds[1], reds[2], coin_seg3_vertex_red);

        get_random_color(blues, &randomState);
        while ((RMSE(yellows[0], reds[0], yellows[1], reds[1], yellows[2], reds[2]) < MINDIFF)
               || (RMSE(yellows[0], blues[0], yellows[1], blues[1], yellows[2], blues[2]) < MINDIFF)
               || (RMSE(reds[0], blues[0], reds[1], blues[1], reds[2], blues[2]) < MINDIFF)) {
            get_random_color(blues, &randomState);
        }
        set_coin_color(blues[0], blues[1], blues[2], coin_seg3_vertex_blue);
    }
}
