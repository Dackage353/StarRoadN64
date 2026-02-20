#include "game/area.h"
#include "game/level_update.h"
#include "game/save_file.h"
#include "game/object_list_processor.h"
#include "options_menu.h"

extern void seq_player_play_sequence(u8 player, u8 seqId, u16 arg2);

/*
ADDIU SP, SP, $FFE0
SW RA, $0014 (SP)
LUI A0, $8034
LH A0, $B21A (A0)  -- gMarioStates+0xAA gMarioStates.numStars
ADDIU A1, R0, $0082=130
BNE A1, A0, $000C6FF8
ADDIU A0, R0, $0000
ADDIU A1, R0, $002F
JAL $00320544 -- seq_player_play_sequence
ADDIU A2, R0, $0000
LW RA, $0014 (SP)
ADDIU SP, SP, $0020
JR RA
NOP
*/
void bhv_star_road_ow_music()
{
    if (gMarioStates->numStars == 130) {
        seq_player_play_sequence(0, 0x2F, 0);
    }
}

/*
ADDIU SP, SP, $FFE8
SW RA, $0014 (SP)
LUI A0, $8033
LH A0, $DDF4 (A0) ; gCurrSaveFileNum
OR A1, R0, R0
ADDIU A2, R0, $0018
JAL $0027A010 ; save_file_get_total_star_count
ADDIU A0, A0, $FFFF
SLTI AT, V0, $0001
BNEZ AT, $000CC328
NOP
LUI T6, $8036
LW T6, $1160 (T6) ; gCurrentObject
SH R0, $0074 (T6) ; ->activeFlags
cc328: BEQ R0, R0, $000CC330
NOP
cc330: LW RA, $0014 (SP)
ADDIU SP, SP, $0018
JR RA
NOP
*/

void bhv_star_road_yoshi_init()
{
    int totalStars;
    totalStars = save_file_get_total_star_count(gCurrSaveFileNum - 1, 0, 0x18);
    if (totalStars >= 1) {
        gCurrentObject->activeFlags = 0;
    }

    return;
}

extern void print_text_fmt_int(s16 x, s16 y, const char *str, s32 value);
Gfx *geo_star_road_cull(s32 callContext, struct GraphNode *node, UNUSED Mat4 mtx)
{
    if (callContext == GEO_CONTEXT_RENDER) {
        struct GraphNodeGenerated *genNode = (struct GraphNodeGenerated *) node;
        struct GraphNodeDisplayList *graphNode = (struct GraphNodeDisplayList *) node->next;
        int param = genNode->parameter;
        int active = 0;
        switch (param)
        {
            // Castle courtyard
            case 0:
                active = gMarioStates->pos[1] < 0;
                break;
            case 1:
                active = gMarioStates->pos[1] >= 0 && gMarioStates->pos[1] < 3270;
                break;
            case 10:
                active = gMarioStates->pos[1] >= 3270;
                break;
            
            // CCCoral
            case 2:
                active = gMarioStates->pos[1] < 2500.f;
                break;
            case 3:
                active = gMarioStates->pos[1] >= 2500.f;
                break;
            
            case 4:
                active = 1;
                if (gMarioStates->pos[1] > -400)
                    active = 0;
                if (gMarioStates->pos[2] < -4000)
                    active = 0;
                break;
            
            // SR c15, oh boy
            case 5:
            case 6:
            case 7:
            case 11:
            case 12:
                // Rhombus checks for inside the dome
                f32 rhombusPlus  = gMarioStates->pos[0] + gMarioStates->pos[2];
                f32 rhombusMinus = gMarioStates->pos[0] - gMarioStates->pos[2];
                bool inDome = -3699.f < rhombusPlus  && rhombusPlus  < 3180.f
                           &&  4486.f < rhombusMinus && rhombusMinus < 11526.f
                           && -1853.f < gMarioStates->pos[1] && gMarioStates->pos[1] < 1166.f
                           && gMarioStates->ceilHeight < 1000.f;
                bool inTube = -294.f  < gMarioStates->pos[0] && gMarioStates->pos[0] < 1775.f
                           && -1705.f < gMarioStates->pos[1] && gMarioStates->pos[1] < -145.f
                           && -5265.f < gMarioStates->pos[2] && gMarioStates->pos[2] < -5265.f
                           && gMarioStates->ceilHeight < 100.f;

                // Basic regions
                bool aboveGround     = gMarioStates->pos[1] >= -1320;
                bool inHouseEntrance = (-6600 < gMarioStates->pos[2] && gMarioStates->pos[2] < -1689)
                                    && ( 1500 < gMarioStates->pos[0] && gMarioStates->pos[0] < 6327);
                bool inHouse         = (-6600 < gMarioStates->pos[2] && gMarioStates->pos[2] < -1689)
                                    && (-5347 < gMarioStates->pos[0] && gMarioStates->pos[0] < 6327);
                bool outOfHouse      = gMarioStates->pos[2] > 0.f && gMarioStates->pos[1] > -1954.f;
                bool veryHigh        = gMarioStates->pos[1] > 1166.f;

                bool skipInners = outOfHouse || veryHigh;
                bool wantAllInners = !aboveGround || inHouseEntrance;
                bool insideTinyLod = inDome || inTube;
                bool inside = insideTinyLod || !aboveGround;

                // Below the ground parts + cut parts above
                if (5 == param)
                {
                    active = !insideTinyLod && !skipInners && (inside && wantAllInners);
                }
                // Above the ground - condition flip for 5==param
                if (6 == param)
                {
                    active = !insideTinyLod && !skipInners && (!inside || !wantAllInners);
                }
                // Above the ground but very far from house
                if (11 == param)
                {
                    active = !insideTinyLod && skipInners;
                }
                // Tiny lod exactly inside the tube/dome
                if (12 == param)
                {
                    active = insideTinyLod;
                }

                // XLU bulb
                if (7 == param)
                {
                    active = aboveGround && inHouse && !inHouseEntrance && !insideTinyLod;
                }
                break;

            case 8:
                active = (gMarioStates->pos[1] < 1764)
                      && (gMarioStates->pos[0] - gMarioStates->pos[2] > 8829);
                break;
            case 9:
                active = (gMarioStates->pos[1] < 1764)
                      && (gMarioStates->pos[0] - gMarioStates->pos[2] > 8829);
                active = !active;
                break;

            case 13:
            case 14:
                f32 dx = gMarioStates->pos[0] + 5376.f;
                f32 dz = gMarioStates->pos[2] - 3536.f;

                f32 dr = dx * dx + dz * dz;
                f32 target = 800.f * 800.f;
                if (14 == param)
                    active = dr < target;
                else
                    active = dr >= target;
                break;
            
            case 15:
            case 16:
                active = gMarioStates->pos[2] > 2000.f && gMarioStates->pos[0] < -5500.f;
                if (16 == param)
                    active = !active;
                break;
                
            case 17:
            case 18:
                active = gMarioStates->pos[2] > 1700.f && gMarioStates->pos[0] < -6500.f;
                if (18 == param)
                    active = !active;
                break;
        }


        if (active) {
            graphNode->node.flags |= GRAPH_RENDER_ACTIVE;
        } else {
            graphNode->node.flags &= ~GRAPH_RENDER_ACTIVE;
        }
    }

    return NULL;
}

extern const BehaviorScript bhvHiddenRedCoinStar[];
void bhv_red_coin_radar_init()
{
    if (gMarioStates->numStars >= 120)
        return cur_obj_hide();

    int act = o->oBehParams >> 24;
    if (0 == act)
    {
        f32 d;
        struct Object* redCoinStarSpawner = cur_obj_find_nearest_object_with_behavior(bhvHiddenRedCoinStar, &d);
        int id = redCoinStarSpawner->oBehParams >> 24;
        if (save_file_get_star_flags(gCurrSaveFileNum - 1, gCurrCourseNum - 1) & (1 << id))
        {
            cur_obj_hide();
        }

        return;
    }

    if (gCurrActNum != act)
    {
        cur_obj_hide();
    }
}

extern const BehaviorScript bhvRedCoin[];
static struct Object *cur_obj_find_nearest_object_with_behavior_y_biased(const BehaviorScript *behavior) {
    uintptr_t *behaviorAddr = behavior;
    struct ObjectNode *listHead = &gObjectLists[get_object_list_from_behavior(behaviorAddr)];
    struct Object *obj = (struct Object *) listHead->next;
    struct Object *closestObj = NULL;
    f32 minDist = 0x200000000;

    while (obj != (struct Object *) listHead) {
        if (obj->behavior == behaviorAddr
            && obj->activeFlags != ACTIVE_FLAG_DEACTIVATED
            && obj != o
        ) {
            f32 dx = o->oPosX - obj->oPosX;
            f32 dy = o->oPosY - obj->oPosY;
            f32 dz = o->oPosZ - obj->oPosZ;
            f32 objDist = dx*dx + 10*dy*dy + dz*dz;
            if (objDist < minDist) {
                closestObj = obj;
                minDist = objDist;
            }
        }

        obj = (struct Object *) obj->header.next;
    }

    return closestObj;
}

#include "game/game_init.h"
#include "engine/math_util.h"

extern u16 gExtraGuides[2];
void bhv_red_coin_radar_loop()
{
    int beingShotFromCannon = (gMarioStates->action == ACT_SHOT_FROM_CANNON || gMarioStates->action == ACT_IN_CANNON);
    if (configRedCoinRadar == 0 || beingShotFromCannon || gExtraGuides[0] == 255/16)
    {
        cur_obj_hide();
        return;
    };

    if (gPlayer1Controller->buttonPressed & L_TRIG)
    {
        if (obj_is_hidden(o))
        {
            cur_obj_unhide();
        }
        else
        {
            cur_obj_hide();
        }
    }
    
    obj_scale(o, 0.528f);
    struct Object* red = cur_obj_find_nearest_object_with_behavior_y_biased((void*) bhvRedCoin);
    if (!red)
    {
        o->activeFlags = 0;
        return;
    }

    o->oPosX = gMarioStates->pos[0];
    o->oPosY = gMarioStates->pos[1] + 150.f;
    o->oPosZ = gMarioStates->pos[2];

    f32 dx = o->oPosX - red->oPosX;
    f32 dy = o->oPosY - red->oPosY;
    f32 dz = o->oPosZ - red->oPosZ;
    f32 l = sqrtf(dx*dx + dz*dz);

    s16 yaw = atan2s(dz, dx) + 0x8000;
    s16 pitch = atan2s(l, dy);
    o->oFaceAngleYaw = yaw;
    o->oPosX += sins(yaw) * 80.f;
    o->oPosZ += coss(yaw) * 80.f;
    o->oFaceAnglePitch = pitch;
}

extern void load_object_collision_model();
void ssl_star_road_fff_pillar_loop()
{
    if (gMarioStates->pos[1] < 540)
    {
        cur_obj_hide();
    }
    else
    {
        cur_obj_unhide();
        load_object_collision_model();
    }
}

void bhv_checkmark_loop()
{
    o->oOpacity = CLAMP(o->oOpacity + 25, 0, 255);    
}

void bhv_sl_purple_box_init()
{
    if (gCurrLevelNum == LEVEL_SL && !configFasterObjects)
    {
        o->oPosY -= 100.f;
    }
}
