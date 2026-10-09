#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_profile.h"
#include "game/ai_unit_action.h"
#include "game/map_object.h"
#include "game/terrain.h"
#include "game/unit_stats.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern u8 gTerrainFactoryKind[]; /* per terrain id: 1 base, 2 airport, 3 port, else 0 */

s32 HwDiv32(s32 numerator, s32 denominator);
int GetUnitMaxRange(int player, int unitType);
void Combat_CalcDamageToTarget(int unitIndex, int target, s16 *pos);
int func_020c09e0(int unitIndex, int tile);   /* 1: tile owned by the unit's team */
BOOL func_0209eca8(int x, int y);             /* TRUE: tile owned by unk_04 and not a repair tile */
MapObject *func_020db148(int x, int y);       /* object at (x, y) with hp left, or NULL */
int func_0209e424(MapUnit *unit, int target); /* chart damage against a target kind, best weapon */
BOOL func_0209eb38(s16 x, s16 y);
BOOL func_020a770c(s16 x, s16 y);
int AiAttack_Score(int target, s16 *pos);      /* score of attacking unit slot target (low byte) from pos */
void func_020a491c(void);                     /* clears actingUnit's AI target */
void AiUnit_PerformAction(int x, int y, int action, u8 arg3, u8 arg4); /* queues an action for the unit at (x, y) */

#define STATE (data_027c027c->unk_48[data_027c0284])
#define MAP (STATE->mapTiles)
#define TILE_INDEX(x, y) ((x) + MAP->rowStart[y])
#define UNIT_TILE(unit) (MAP->tiles[TILE_INDEX((unit)->x, (unit)->y)])

/* AiUnit_PerformAction actions */
#define AI_ACTION_ATTACK_UNIT 4 /* arg3: unit slot */
#define AI_ACTION_ATTACK_TILE 5 /* arg3, arg4: x, y */

/* Damage chart target kind of tiles with hp (tileHp); likely pipe seams. */
#define TARGET_KIND_TILE 6

/* Plans the attack of the AI's acting unit, an indirect unit, from where it
 * stands. Every tile in range is tried: a map object or a tile with hp when
 * gAiTurnState allows, otherwise an enemy unit there. The best unit (by
 * AiAttack_Score's score) and the best tile are each queued with
 * AiUnit_PerformAction; a tile is kept only if the damage reaches 10 or its hp. */

void Ai_PlanIndirectAttack(void)
{
    int maxY;
    int hp;
    int bestUnitScore;
    int bestUnitSlot;
    int bestTileHp;
    int distance;
    int flags;
    int minY;
    int x;
    s16 pos[2];
    int y;
    int minX;
    int maxX;
    int score;
    s16 bestTileX;
    MapUnit *target;
    int damage;
    int kind;
    s16 bestTileY;
    MapObject *object;

    bestUnitScore = 0;
    bestUnitSlot = 0;
    flags = 0;
    bestTileHp = 0xff; /* none yet */
    if (STATE->actingUnit->aiOrder != 7) {
        /* flags for AiAttack_Score: 0x40000 unit above 80 hp on its team's base,
         * 0x10000 on its team's base, airport or port, or a ground unit at
         * 70 hp or less not on a tile where func_0209eca8 holds */
        if (func_020c09e0(STATE->actingUnitIndex, UNIT_TILE(STATE->actingUnit)) == 1
            && MAP_TILE_TERRAIN(UNIT_TILE(STATE->actingUnit)) == TERRAIN_BASE && STATE->actingUnit->hp > 80) {
            flags |= 0x40000;
        } else if ((func_020c09e0(STATE->actingUnitIndex, UNIT_TILE(STATE->actingUnit)) == 1
                    && gTerrainFactoryKind[MAP_TILE_TERRAIN(UNIT_TILE(STATE->actingUnit))] != 0)
                   || ((Ov000_gUnitStats[STATE->actingUnit->type].moveGroup & (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD)) != 0
                       && STATE->actingUnit->hp <= 70
                       && func_0209eca8(STATE->actingUnit->x, STATE->actingUnit->y) == 0)) {
            flags |= 0x10000;
        }
    }
    pos[0] = STATE->actingUnit->x;
    pos[1] = STATE->actingUnit->y;
    if (STATE->actingUnit->x - GetUnitMaxRange(STATE->currentPlayer, STATE->actingUnit->type) < 0) {
        minX = 0;
    } else {
        minX = STATE->actingUnit->x - GetUnitMaxRange(STATE->currentPlayer, STATE->actingUnit->type);
    }
    if (STATE->actingUnit->y - GetUnitMaxRange(STATE->currentPlayer, STATE->actingUnit->type) < 0) {
        minY = 0;
    } else {
        minY = STATE->actingUnit->y - GetUnitMaxRange(STATE->currentPlayer, STATE->actingUnit->type);
    }
    maxX = STATE->unk_1c4->mapWidth - 1;
    if (STATE->actingUnit->x + GetUnitMaxRange(STATE->currentPlayer, STATE->actingUnit->type) < maxX) {
        maxX = STATE->actingUnit->x + GetUnitMaxRange(STATE->currentPlayer, STATE->actingUnit->type);
    }
    maxY = STATE->unk_1c4->mapHeight - 1;
    if (STATE->actingUnit->y + GetUnitMaxRange(STATE->currentPlayer, STATE->actingUnit->type) < maxY) {
        maxY = STATE->actingUnit->y + GetUnitMaxRange(STATE->currentPlayer, STATE->actingUnit->type);
    }
    for (y = minY; y <= maxY; y++) {
        for (x = minX; x <= maxX; x++) {
            distance = 0;
            if (STATE->actingUnit->x - x > 0) {
                distance += STATE->actingUnit->x - x;
            } else {
                distance += x - STATE->actingUnit->x;
            }
            if (STATE->actingUnit->y - y > 0) {
                distance += STATE->actingUnit->y - y;
            } else {
                distance += y - STATE->actingUnit->y;
            }
            if (distance < Ov000_gUnitStats[STATE->actingUnit->type].minRange) {
                continue;
            }
            if (distance > GetUnitMaxRange(STATE->currentPlayer, STATE->actingUnit->type)) {
                continue;
            }
            if (AI_TARGET_FLAGS->attackObjects) {
                object = func_020db148(x, y);
                if (object != NULL) {
                    hp = object->hp;
                    kind = object->kind;
                    goto attack_tile;
                }
            }
            if (AI_TARGET_FLAGS->attackTiles) {
                kind = TARGET_KIND_TILE;
                hp = STATE->tileHp[TILE_INDEX(x, y)] & 0x7f;
                if (hp != 0 && MAP->unitAt[TILE_INDEX(x, y)] == 0) {
                    goto attack_tile;
                }
            }
            goto attack_unit;
attack_tile:
            damage = HwDiv32(func_0209e424(STATE->actingUnit, kind) * STATE->actingUnit->hp, 100);
            if (damage < hp && damage < 10) {
                continue;
            }
            Combat_CalcDamageToTarget(STATE->actingUnitIndex, kind, pos);
            if (STATE->combatants[0].weapon == COMBAT_WEAPON_NONE) {
                continue;
            }
            /* order between tiles; meaning unclear */
            if (damage >= bestTileHp) {
                if (damage < hp || bestTileHp > hp) {
                    continue;
                }
            } else if (bestTileHp < hp) {
                continue;
            }
            if (STATE->players[STATE->currentPlayer].aiBehaviour == AI_BEHAVIOUR_DEFENSE && damage < hp) {
                if (MAP->unk_3250[TILE_INDEX(pos[0], pos[1])] >= gAiActiveUnitProfile->unk_01) {
                    continue;
                }
                if (MAP->unk_3758[TILE_INDEX(pos[0], pos[1])] >= gAiActiveUnitProfile->unk_02) {
                    continue;
                }
            }
            if (func_020a770c(pos[0], pos[1]) == 0) {
                continue;
            }
            bestTileX = x;
            bestTileY = y;
            bestTileHp = hp;
            continue;
attack_unit:
            target = &STATE->units[MAP->unitAt[TILE_INDEX(x, y)]];
            if (target->type == UNIT_NONE) {
                continue;
            }
            if (STATE->players[(STATE->actingUnitIndex >> 6) + 1].team
                == STATE->players[MAP_UNIT_OWNER(STATE, target)].team) {
                continue;
            }
            if (func_0209eb38(x, y) == 0) {
                continue;
            }
            score = AiAttack_Score(flags | MAP->unitAt[TILE_INDEX(target->x, target->y)], pos);
            if (score != 0 && score >= bestUnitScore) {
                bestUnitScore = score;
                bestUnitSlot = MAP->unitAt[TILE_INDEX(x, y)];
            }
        }
    }
    if (bestUnitScore != 0) {
        func_020a491c();
        AiUnit_PerformAction(STATE->actingUnit->x, STATE->actingUnit->y, AI_ACTION_ATTACK_UNIT, bestUnitSlot, 0);
    }
    if (bestTileHp == 0xff) {
        return;
    }
    func_020a491c();
    AiUnit_PerformAction(STATE->actingUnit->x, STATE->actingUnit->y, AI_ACTION_ATTACK_TILE, bestTileX, bestTileY);
}
