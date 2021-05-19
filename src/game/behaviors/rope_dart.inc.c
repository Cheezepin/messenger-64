#define oInteracted oNumLootCoins
extern s32 gQuarterStepResult;
extern s32 gCloudstep;

#define MAX_TARGETS 6
u32 targets[MAX_TARGETS][3] = {
   /*Behavior         Radius          Height*/
    {bhvGoomba,       0xFFFFFFFF,         25},
    {bhvPiranhaPlant, 0xFFFFFFFF, 0xFFFFFFFF},
    {bhvBobomb,              200,        200},
    {bhvFlyGuy,       0xFFFFFFFF, 0xFFFFFFFF},
    {bhvStar,                200, 0xFFFFFFFF},
    {bhvSpawnedStar,         200, 0xFFFFFFFF},
};

struct Object *closestObject;
u32 objectType;
Vec3f oldPos;
void bhv_rope_dart_update(void) {
    f32 dist, closestDist, height, radius;
    u8 i, j;
    o->oWallHitboxRadius = 5.0f;
    if(gMarioObject == 0) {
        obj_mark_for_deletion(o);
        closestObject = 0;
    } else {
        if(dist_between_objects(o, gMarioObject) < 200.0f) {
            o->oIntangibleTimer = 5;
        }
        switch(o->oAction) {
            case 0:
                vec3f_set(oldPos, 0.0f, 0.0f, 0.0f);
                closestObject = 0;
                if(o->oNumLootCoins == 1) {
                    o->oTimer = 20;
                }
                if(o->oTimer < 20) {
                    // o->oForwardVel = 20.0f*(f32)(o->oTimer - 1);
                    // cur_obj_compute_vel_xz();
                    // cur_obj_move_xz_using_fvel_and_yaw();
                    o->oForwardVel = 2.0f;
                    for(i = 0; i < 30; i++) {
                        cur_obj_compute_vel_xz();
                        cur_obj_move_xz_using_fvel_and_yaw();
                        cur_obj_update_floor_and_walls();
                        if(o->oMoveFlags & OBJ_MOVE_HIT_WALL) {
                            o->oAction = 1;
                            vec3f_copy(oldPos, gMarioState->pos);
                            i = 30;
                        }
                        closestDist = 1500000.0f;
                        for(j = 0; j < MAX_TARGETS; j++) {
                            struct Object *cringe = cur_obj_find_nearest_object_with_behavior(targets[j][0], &dist);
                            if(dist < 250.0f && dist < closestDist) {
                                closestObject = cringe;
                                closestDist = dist;
                                objectType = j;
                            }
                        }
                        o->oInteracted = 0;
                        if(closestObject != 0) {
                            o->oAction = 1;
                            vec3f_copy(oldPos, gMarioState->pos);
                            o->oInteracted = 1;
                            i = 30;
                        }
                    }
                } else {
                    o->oForwardVel = 80.0f;
                    o->oMoveAngleYaw = obj_angle_to_object(o, gMarioObject);
                    if(o->oPosY > gMarioObject->oPosY + 125.0f) {
                        o->oPosY -= 50.0f;
                    }
                    if(o->oPosY < gMarioObject->oPosY + 75.0f) {
                        o->oPosY += 50.0f;
                    }
                    if(dist_between_objects(o, gMarioObject) < 150.0f) {
                        obj_mark_for_deletion(o);
                        closestObject = 0;
                    }
                    cur_obj_compute_vel_xz();
                    cur_obj_move_xz_using_fvel_and_yaw();
                }
                //o->oDistFromHome = sqrtf(POW2(o->oPosX - o->oHomeX) + POW2(o->oPosZ - o->oHomeZ));
                break;
            case 1:
                if(closestObject != 0) {
                    if(targets[objectType][2] != 0xFFFFFFFF) {
                        height = (f32)(targets[objectType][2]);
                    } else if(closestObject->hitboxHeight != 0.0f) {
                        height = closestObject->hitboxHeight;
                    } else {
                        height = 100.0f;
                    }
                }
                if(o->oInteracted == 1) {
                    o->oPosX = closestObject->oPosX;
                    o->oPosY = closestObject->oPosY + height;
                    o->oPosZ = closestObject->oPosZ;
                }
                set_mario_action(gMarioState, ACT_JUMP, 0);
                gMarioState->faceAngle[1] = obj_angle_to_object(gMarioObject, o);
                gMarioState->forwardVel = 50.0f;
                vec3f_set(gMarioState->vel, 0.0f, 0.0f, 0.0f);
                if(o->oInteracted == 0) {
                    height = 0.0f;
                }
                if(gMarioState->pos[1] + 150.0f < o->oPosY + height) {
                    gMarioState->pos[1] += 75.0f;
                }
                if(gMarioState->pos[1] + 50.0f > o->oPosY + height) {
                    gMarioState->pos[1] -= 75.0f;
                }
                if(absf((o->oPosY + height) - gMarioState->pos[1]) < 150.0f) {
                    gMarioState->pos[1] = o->oPosY;
                }
                if(o->oInteracted == 0 /*|| gQuarterStepResult == AIR_STEP_HIT_WALL*/) {
                    /* if(o->oActionTimer > 0) {
                        o->oActionTimer--;
                        if(o->oActionTimer == 0 && (gQuarterStepResult == AIR_STEP_HIT_WALL && o->oWall != gMarioState->wall)) {
                            o->oAction = 0;
                            o->oNumLootCoins = 1;
                        }
                    }
                    else  */if((gQuarterStepResult == AIR_STEP_HIT_WALL && gMarioState->wall != 0 && o->oWall != gMarioState->wall) || o->oTimer > 120 || sqrtf(POW2(gMarioState->pos[0] - oldPos[0]) + POW2(gMarioState->pos[1] - oldPos[1]) + POW2(gMarioState->pos[2] - oldPos[2])) < 2.0f) {
                        o->oAction = 0;
                        o->oNumLootCoins = 1;
                    }
                    else if(dist_between_objects(o, gMarioObject) < 150.0f) {
                        o->oAction = 2;
                    }
                }

                if(closestObject != 0) {
                    if(targets[objectType][1] != 0xFFFFFFFF) {
                        radius = (f32)(targets[objectType][1]);
                    } else if(closestObject->hitboxRadius != 0.0f) {
                        radius = closestObject->hitboxRadius;
                    } else {
                        radius = 100.0f;
                    }
                }
                if(o->oInteracted == 1 && dist_between_objects(o, gMarioObject) < radius) {
                    o->oAction = 2;
                    if(closestObject != 0) {
                        if(closestObject->oInteractType != INTERACT_BOUNCE_TOP && closestObject->oInteractType != INTERACT_STAR_OR_KEY) {
                            set_mario_action(gMarioState, ACT_JUMP, 0);
                            gMarioState->vel[1] = 60.0f;
                            gCloudstep = 1;
                        }
                    }
                }
                if(gMarioState->pos[1] - gMarioState->floorHeight < 100.0f) {
                    gMarioState->pos[1] = gMarioState->floorHeight + 100.0f;
                }
                gQuarterStepResult = 0;
                vec3f_copy(oldPos, gMarioState->pos);
                break;
            case 2:
                if(o->oTimer > 2) {
                    obj_mark_for_deletion(o);
                    closestObject = 0;
                }
        }
    o->oDistanceToMario = dist_between_objects(o, gMarioObject);
    }
}