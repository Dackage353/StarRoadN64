// cannon.inc.c
#include "game/options_menu.h"
#include "game/print.h"

void bhv_cannon_base_unused_loop(void) {
    o->oPosY += o->oVelY;
}

extern const BehaviorScript bhvStarRoadSRCannon[];
void opened_cannon_act_idle(void) { // act 0
    if (o->oTimer == 0) {
        o->oInteractStatus = INT_STATUS_NONE;
        vec3f_copy(&o->oPosVec, &o->oHomeVec);
        o->oMoveAnglePitch = 0;
        o->oMoveAngleYaw   = o->oCannonBaseAngle;
        o->oCannonAngle    = 0;
        o->oCannonIsActive = FALSE;
        cur_obj_enable_rendering();
        cur_obj_become_tangible();
    }

    if (o->oDistanceToMario < 2000.0f) {
        cur_obj_become_tangible();
        cur_obj_enable_rendering();
        if (o->oInteractStatus & INT_STATUS_INTERACTED
         && (!(o->oInteractStatus & INT_STATUS_TOUCHED_BOB_OMB))) { // bob-omb explodes when it gets into a cannon
            if (0) // if (o->behavior == bhvStarRoadSRCannon)
            {
                // I am hoping that compiler is smart enough to optimize this out
                for (int i = 0; i <= 68; i++)
                {
                    o->oPosY += 5.0f;
                    f32 horizontalVel = (f32)(((i / 2) & 0x1) - 0.5f) * 2; 
                    o->oPosX += horizontalVel;
                    o->oPosZ += horizontalVel;

                    if (i > 67) {
                        horizontalVel *= 2;
                        o->oPosX += horizontalVel;
                        o->oPosZ += horizontalVel;
                    }
                }

                for (int i = 0; i <= 26; i++)
                {
                    if (i < 4) {
                        f32 horizontalVel = ((f32)(((i / 2) & 0x1) - 0.5f) * 4.0f);
                        o->oPosX += horizontalVel;
                        o->oPosZ += horizontalVel;
                    } else {
                        if (i >= 6) {
                            if (i < 22) {
                                o->oMoveAngleYaw =
                                    ((sins(o->oCannonAngle) * 0x4000) + o->oCannonBaseAngle);
                                o->oCannonAngle += 0x400;
                            } else if (i >= 26) {
                                o->oCannonAngle = 0;
                                o->oAction = 5;
                            }
                        }
                    }
                }

                for (int i = 0; i <= 25; i++)
                {
                    if (i >= 4) {
                        if (i < 20) {
                            o->oCannonAngle += 0x400;
                            o->oMoveAnglePitch = (sins(o->oCannonAngle) * 0x2000);
                        } else if (i >= 25) {
                            o->oAction = 1;
                        }
                    }
                }
            }
            else
            {
                o->oAction = 4;
            }
            o->oCannonIsActive = TRUE;
            o->oCannonTimeSinceActivated = 1;
        } else {
            o->oInteractStatus = INT_STATUS_NONE;
        }
    } else {
        cur_obj_become_intangible();
        cur_obj_disable_rendering();
        o->oCannonIsActive = FALSE;
    }
}

void opened_cannon_act_rising(void) { // act 4
    if (o->oTimer == 0) {
        cur_obj_play_sound_2(SOUND_OBJ_CANNON_RISE);
    }

    o->oPosY += 5.0f;
    f32 horizontalVel = (f32)(((o->oTimer / 2) & 0x1) - 0.5f) * 2; 
    o->oPosX += horizontalVel;
    o->oPosZ += horizontalVel;

    if (o->oTimer > 67) {
        horizontalVel *= 2;
        o->oPosX += horizontalVel;
        o->oPosZ += horizontalVel;
        o->oAction = 6;
    }
}

void opened_cannon_act_turning_yaw(void) { // act 6
    if (o->oTimer == 0) {
        cur_obj_play_sound_2(SOUND_OBJ_CANNON_TURN);
    }

    if (o->oTimer < 4) {
        f32 horizontalVel = ((f32)(((o->oTimer / 2) & 0x1) - 0.5f) * 4.0f);
        o->oPosX += horizontalVel;
        o->oPosZ += horizontalVel;
    } else {
        if (o->oTimer >= 6) {
            if (o->oTimer < 22) {
                o->oMoveAngleYaw =
                    ((sins(o->oCannonAngle) * 0x4000) + o->oCannonBaseAngle);
                o->oCannonAngle += 0x400;
            } else if (o->oTimer >= 26) {
                o->oCannonAngle = 0;
                o->oAction = 5;
            }
        }
    }
}

void opened_cannon_act_pitch_up(void) { // act 5
    if (o->oTimer == 0) {
        cur_obj_play_sound_2(SOUND_OBJ_CANNON_BARREL_PITCH);
    }
    if (o->oTimer >= 4) {
        if (o->oTimer < 20) {
            o->oCannonAngle += 0x400;
            o->oMoveAnglePitch = (sins(o->oCannonAngle) * 0x2000);
        } else if (o->oTimer >= 25) {
            o->oAction = 1;
        }
    }
}

void opened_cannon_act_ready(void) { // act 1
    cur_obj_become_intangible();
    cur_obj_disable_rendering();

    o->oCannonIsActive = FALSE;
    gMarioShotFromCannon = TRUE;
}

void opened_cannon_act_shoot(void) { // act 2
    o->oAction = 3;
}

void opened_cannon_act_resetting(void) { // act 3
    if (o->oTimer > 3) {
        o->oAction = 0;
    }
}

ObjActionFunc sOpenedCannonActions[] = {
    opened_cannon_act_idle,
    opened_cannon_act_ready,
    opened_cannon_act_shoot,
    opened_cannon_act_resetting,
    opened_cannon_act_rising,
    opened_cannon_act_pitch_up,
    opened_cannon_act_turning_yaw,
};

extern u8 gWantCameraResetAfterWarp;
void bhv_cannon_base_init()
{
    o->oCannonBaseAngle = (s16)(o->oBehParams2ndByte << 8);
}

extern const char* gExtraTexts[2];
extern u16 gExtraGuides[2];

void bhv_cannon_base_loop(void) {
    int canFastWarp = configFasterObjects && (gCurrLevelNum == LEVEL_BOB || gCurrLevelNum == LEVEL_JRB || gCurrLevelNum == LEVEL_RR || gCurrLevelNum == LEVEL_WDW || gCurrLevelNum == LEVEL_DDD);
    if (canFastWarp)
    {
        if (o->oAction >= 4 || o->oAction == 1)
        {
            o->oCannonQuickWarpActive = 100;
        }
        else if (o->oCannonQuickWarpActive)
        {
            int beingShotFromCannon = (gMarioStates->action == ACT_SHOT_FROM_CANNON || gMarioStates->action == ACT_IN_CANNON);
            if (!beingShotFromCannon)
            {
                o->oCannonQuickWarpActive--;
            }

            if (gMarioStates->action == ACT_STAR_DANCE_NO_EXIT
             || gMarioStates->action == ACT_STAR_DANCE_WATER
             || gMarioStates->action == ACT_STAR_DANCE_EXIT
             || gMarioStates->action == ACT_FALL_AFTER_STAR_GRAB)
            {
                o->oCannonQuickWarpActive = 0;
            }

            if (o->oCannonQuickWarpActive)
            {
                gExtraTexts[0] = "Press L to warp back";
                gExtraGuides[0] = 255/16;
                if (gPlayer1Controller->buttonDown & L_TRIG)
                {
                    gMarioObject->oIntangibleTimer = -1;
                    gMarioStates->interactObj = o;
                    gMarioStates->usedObj = o;
                    o->oBehParams = 0xef << 16;
                    o->oBehParams2ndByte = 0xef;
                    level_trigger_warp(gMarioStates, WARP_OP_TELEPORT);
                    gWantCameraResetAfterWarp = 1;
                    o->oCannonQuickWarpActive = 0;
                }
            }
        }
    }

    cur_obj_call_action_function(sOpenedCannonActions);

    if (o->oCannonTimeSinceActivated) {
        o->oCannonTimeSinceActivated++;
    }

    o->oInteractStatus = INT_STATUS_NONE;
}

void bhv_cannon_barrel_loop(void) {
    struct Object *parent = o->parentObj;

    if (parent->header.gfx.node.flags & GRAPH_RENDER_ACTIVE) {
        cur_obj_enable_rendering();
        obj_copy_pos(o, o->parentObj);
        o->oMoveAngleYaw = o->parentObj->oMoveAngleYaw;
        o->oFaceAnglePitch = o->parentObj->oMoveAnglePitch;
    } else {
        cur_obj_disable_rendering();
    }
}
