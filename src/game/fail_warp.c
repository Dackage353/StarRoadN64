#include "fail_warp.h"

#include "behavior_data.h"
#include "game/area.h"
#include "game/interaction.h"
#include "game/level_update.h"
#include "game/mario.h"
#include "options_menu.h"

extern void set_camera_mode_8_directions(struct Camera *c);
extern s16 s8DirModeYawOffset;

static int sSafePosArea = 0;
static int sSafePosLevel = 0;
static Vec3s sSafePos = {};
static s16 sSafePosAngle;
static s16 sSafePosCameraYaw;

static void fail_warp_set_safe_pos(struct MarioState *m)
{
    // print_text_fmt_int(20, 20, "X %d", (int) m->pos[0]);
    // print_text_fmt_int(20, 40, "Y %d", (int) m->pos[1]);
    // print_text_fmt_int(20, 60, "Z %d", (int) m->pos[2]);
    sSafePos[0] = m->pos[0];
    sSafePos[1] = m->pos[1];
    sSafePos[2] = m->pos[2];
    sSafePosAngle = m->faceAngle[1];
    sSafePosCameraYaw = s8DirModeYawOffset;
    sSafePosArea = gCurrAreaIndex;
    sSafePosLevel = gCurrLevelNum;
}

void fail_warp_mario_set_safe_pos(struct MarioState *m, struct Surface *floor)
{
    // print_text(20, 80, "POS CHECK");
    // print_text_fmt_int(20, 100, "FLOOR %x", ((int) floor) & 0xffffff);
    m->floorHeight = m->pos[1];
    if (!floor)
        return;

    // object generated floors might be unsafe so avoid them
    if (floor->object)
        return;

    if (sSafePosArea != gCurrAreaIndex || sSafePosLevel != gCurrLevelNum) 
    {
        return fail_warp_set_safe_pos(m);
    }
    
    bool slideTerrain = (m->area->terrainType & TERRAIN_MASK) == TERRAIN_SLIDE;
    if (slideTerrain)
        return;

    s16 type = floor->type;
    f32 maxAngle = 0.87f;
    if (type == SURFACE_SLIPPERY || type == SURFACE_HARD_SLIPPERY || type == SURFACE_NOISE_SLIPPERY || type == SURFACE_NO_CAM_COL_SLIPPERY)
        maxAngle = 0.94f;

    if (!SURFACE_IS_UNSAFE(type)
     && type != SURFACE_VERY_SLIPPERY
     && type != SURFACE_ICE
     && type != SURFACE_SUPER_SLIPPERY
     && type != SURFACE_HARD_VERY_SLIPPERY
     && type != SURFACE_DEATH_PLANE
     && type != SURFACE_HORIZONTAL_WIND
     && floor->normal.y >= maxAngle)
    {
        return fail_warp_set_safe_pos(m);
    }
}

static struct ObjectWarpNode sSpoofedWarpNode = { };
struct Object gFailWarpSpoofedWarpObject = { };
struct ObjectWarpNode* fail_warp_area_get_warp_node(void)
{
    sSpoofedWarpNode.node.id = WARP_NODE_FAIL_WARP;
    sSpoofedWarpNode.node.destLevel = gCurrLevelNum;
    sSpoofedWarpNode.node.destArea = gCurrAreaIndex;
    sSpoofedWarpNode.node.destNode = WARP_NODE_FAIL_WARP;
    sSpoofedWarpNode.next = NULL;
    gFailWarpSpoofedWarpObject.oPosX = sSafePos[0];
    gFailWarpSpoofedWarpObject.oPosY = sSafePos[1];
    gFailWarpSpoofedWarpObject.oPosZ = sSafePos[2];
    gFailWarpSpoofedWarpObject.oFaceAngleYaw = sSafePosAngle;
    gFailWarpSpoofedWarpObject.oMoveAngleYaw = sSafePosAngle;
    gFailWarpSpoofedWarpObject.behavior = bhvInstantActiveWarp;
    
    return &sSpoofedWarpNode;
}

static void spoof_warp(struct MarioState *m)
{
    m->usedObj = &gFailWarpSpoofedWarpObject;
    gFailWarpSpoofedWarpObject.oBehParams = WARP_NODE_FAIL_WARP << 16;
    gFailWarpSpoofedWarpObject.oBehParams2ndByte = WARP_NODE_FAIL_WARP;
}

void fail_warp_pre_level_trigger_warp(struct MarioState *m, s32* warpOp)
{
    if (!configFailWarp)
    {
        return;
    }

    int damage = 0;
    if (*warpOp != WARP_OP_DEATH && *warpOp != WARP_OP_WARP_FLOOR)
    {
        return;
    }

    damage = 0x400;
    if (m->health <= damage + 0x80)
        return;

    m->health = (m->health & (~0xff)) + 0x80;
    if ((m->action == ACT_BURNING_JUMP) || (m->action == ACT_BURNING_FALL) || (m->action == ACT_BURNING_GROUND))
    {
        drop_and_set_mario_action(m, ACT_FREEFALL, 0);
    }
    mario_stop_riding_and_holding(m);

    m->hurtCounter = damage / 0x40;
    *warpOp = WARP_OP_TELEPORT;
    spoof_warp(m);
}

extern void set_camera_mode_boss_fight(struct Camera *c);
void fail_warp_init_mario_after_quick_warp(struct MarioState *m)
{
    set_mario_action(m, ACT_IDLE, 0);
    struct Camera* camera = gCurrentArea->camera;
    if (gCurrLevelNum != LEVEL_BOWSER_3)
        set_camera_mode_8_directions(camera);
    else
        set_camera_mode_boss_fight(camera);

    reset_camera(camera);
    s8DirModeYawOffset = sSafePosCameraYaw & 0xe000;
    // makes paracam not trigger after escaping water
    // *((u32*) 0x80286d20) = 0x80286CC4U;

    bool slideTerrain = (m->area->terrainType & TERRAIN_MASK) == TERRAIN_SLIDE;
    if (slideTerrain)
        level_control_timer(TIMER_CONTROL_HIDE);
}

void fail_warp_init_mario_after_quick_warp_reset_camera(struct Object* spawnObject)
{
    if (spawnObject != &gFailWarpSpoofedWarpObject)
    {
        reset_camera(gCurrentArea->camera);
        if (gCurrLevelNum == LEVEL_LLL)
        {
            s8DirModeYawOffset = 0xC000;
        }
        else if (gCurrLevelNum == LEVEL_BITDW)
        {
            s8DirModeYawOffset = 0x2000;
        }
        else
        {
            s8DirModeYawOffset = (gMarioStates->faceAngle[1] + 0x9000) & 0xe000;
        }
        
        if (spawnObject->behavior == bhvFadingWarp && spawnObject->oBehParams2ndByte == 0xef)
        {
            switch (gCurrLevelNum)
            {
                case LEVEL_BOB:
                    s8DirModeYawOffset = 0xc000;
                    break;
                case LEVEL_JRB:
                    s8DirModeYawOffset = 0xc000;
                    break;
                case LEVEL_RR:
                    s8DirModeYawOffset = 0x000;
                    break;
            }
        }

        if (gCurrLevelNum == LEVEL_CASTLE_COURTYARD)
        {
            if (spawnObject->behavior == bhvFlyingWarp)
            {
                if (8 == spawnObject->oBehParams2ndByte || 9 == spawnObject->oBehParams2ndByte)
                {
                    s8DirModeYawOffset = 0x8000 + s8DirModeYawOffset;
                }
            }
            if (spawnObject->behavior == bhvWarp)
            {
                if (32 == spawnObject->oBehParams2ndByte)
                {
                    s8DirModeYawOffset = 0x8000 + s8DirModeYawOffset;
                }
            }
        }
    }
    else
    {
        gMarioState->invincTimer = 60;
    }
}

void fail_warp_trigger(struct MarioState* m)
{
    spoof_warp(m);
    level_trigger_warp(m, WARP_OP_TELEPORT);
}
