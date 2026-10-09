#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"
#include "game/terrain.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern u8 data_021619cc[];             /* per unit type; nonzero: ground units a Lander carries, without Piperunner */

BOOL func_02098590(void);              /* gAiProfile->unk_08[9] has bit 2 or 3 set */
int Ai_CountUnitsOfType(int type);
BOOL func_020a3294(void);
void func_020ab70c(void);              /* boards an own Lander or Black Boat in reach, if there is one */
int UnitType_GetAttackKind(int type);
void func_01ffb200(s8 *buffer);        /* points rangeMap rows into buffer */
void func_01ffb3a0(int value);         /* fills every rangeMap tile with value */
void RangeMap_FillMoves(int x, int y, int unitType, s16 movement, int arg4);
BOOL UnitSlot_OwnsTile(s16 slot, s16 x, s16 y);
void func_020b2b70(int x, int y);      /* moves the acting unit toward (x, y) */

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

/* RangeMap_FillMoves movement; guess: high enough to act as no limit. */
#define ANY_DISTANCE 110

/* MapTiles.unk_2d48 here: lowest move cost of an own transport to the
 * tile; NO_TRANSPORT where none can get there. */
#define NO_TRANSPORT 127

/* Sends a ground unit with over 60 hp to meet an own Lander, or a Black
 * Boat for Infantry and Mech, on a shoal or port. Each transport with
 * room is checked; one that already carries a unit counts only for an
 * armed unit that can reach it. An armed unit only meets on tiles it can
 * reach; an unarmed one may meet anywhere. The meeting tile with the
 * lowest sum of the unit's and the transport's move costs is chosen.
 * With no transport that can get to a tile the unit reaches, the unit
 * heads for the nearest port its player owns. */
void AiUnit_SeekSeaTransport(void)
{
    /* needed to match: this declaration order and one pair of loop
     * variables per loop give the original register use */
    int reach;
    int portX;
    int x;
    int meetX;
    int portY;
    int meetY;
    int bestX;
    MapUnit *unit;
    int terrain;
    int bestY;
    int type;
    int y;
    int best;
    BOOL found;
    BOOL unarmed;
    int i;

    best = 0x7fff;
    bestX = -1;
    bestY = -1;
    if (!(gAiTurnState->unk_0a_3 & 4)) {
        return;
    }
    if (!func_02098590() && Ai_CountUnitsOfType(UNIT_LANDER) == 0 && Ai_CountUnitsOfType(UNIT_BLACK_BOAT) == 0) {
        return;
    }
    if (STATE->actingUnit->unk_0c_7) {
        return;
    }
    if (STATE->actingUnit->hp < 61) {
        return;
    }
    if (data_021619cc[STATE->actingUnit->type] == 0) {
        return;
    }
    if (func_020a3294() == TRUE) {
        return;
    }
    func_020ab70c();
    unarmed = UnitType_GetAttackKind(STATE->actingUnit->type) == ATTACK_KIND_NONE;
    STATE->actingUnit->unk_0a_14 = 2;
    if (!unarmed) {
        func_01ffb200(STATE->mapTiles->unk_3c60);
        RangeMap_FillMoves(STATE->actingUnit->x, STATE->actingUnit->y, STATE->actingUnit->type, ANY_DISTANCE, 0);
        func_01ffb200(STATE->mapTiles->unk_2840);
    }
    func_01ffb200(STATE->mapTiles->unk_2d48);
    func_01ffb3a0(NO_TRANSPORT);
    func_01ffb200(STATE->mapTiles->unk_2840);

    found = FALSE;
    for (i = Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 1; i < Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 51; i++) {
        unit = &STATE->units[i];
        type = unit->type;
        if (type == UNIT_NONE) {
            continue;
        }
        if (STATE->actingUnit->type <= UNIT_MECH) {
            if (type != UNIT_LANDER && type != UNIT_BLACK_BOAT) {
                continue;
            }
        } else if (type != UNIT_LANDER) {
            continue;
        }
        if (unit->cargo[1] != 0) {
            continue;
        }
        if (unit->cargo[0] != 0) {
            if (unarmed == TRUE) {
                continue;
            }
            if (STATE->mapTiles->unk_3c60[TILE(unit->x, unit->y)] < 0) {
                continue;
            }
        }
        RangeMap_FillMoves(unit->x, unit->y, type, ANY_DISTANCE, 0);
        for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
            for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
                reach = STATE->rangeMap[y][x];
                if (reach < 0) {
                    continue;
                }
                terrain = MAP_TILE_TERRAIN(STATE->mapTiles->tiles[TILE(x, y)]);
                if (terrain != TERRAIN_SHOAL && terrain != TERRAIN_PORT) {
                    continue;
                }
                if (reach >= STATE->mapTiles->unk_2d48[TILE(x, y)]) {
                    continue;
                }
                if (!unarmed && STATE->mapTiles->unk_3c60[TILE(x, y)] < 0) {
                    continue;
                }
                STATE->mapTiles->unk_2d48[TILE(x, y)] = reach;
                found = TRUE;
            }
        }
    }
    RangeMap_FillMoves(STATE->actingUnit->x, STATE->actingUnit->y, STATE->actingUnit->type, ANY_DISTANCE, 0);

    if (!found) {
        for (portY = 0; portY < STATE->unk_1c4->mapHeight; portY++) {
            for (portX = 0; portX < STATE->unk_1c4->mapWidth; portX++) {
                if (STATE->rangeMap[portY][portX] < 0) {
                    continue;
                }
                if (MAP_TILE_TERRAIN(STATE->mapTiles->tiles[TILE(portX, portY)]) != TERRAIN_PORT) {
                    continue;
                }
                if (!UnitSlot_OwnsTile(STATE->currentUnitBase, portX, portY)) {
                    continue;
                }
                reach = STATE->rangeMap[portY][portX];
                if (best >= reach) {
                    bestX = portX;
                    bestY = portY;
                    best = reach;
                }
            }
        }
        if (bestX == -1) {
            return;
        }
        func_020b2b70(bestX, bestY);
        return;
    }

    for (meetY = 0; meetY < STATE->unk_1c4->mapHeight; meetY++) {
        for (meetX = 0; meetX < STATE->unk_1c4->mapWidth; meetX++) {
            if (STATE->rangeMap[meetY][meetX] < 0) {
                continue;
            }
            terrain = MAP_TILE_TERRAIN(STATE->mapTiles->tiles[TILE(meetX, meetY)]);
            if (terrain != TERRAIN_SHOAL && terrain != TERRAIN_PORT) {
                continue;
            }
            if (STATE->mapTiles->unk_2d48[TILE(meetX, meetY)] == NO_TRANSPORT) {
                continue;
            }
            reach = STATE->rangeMap[meetY][meetX] + STATE->mapTiles->unk_2d48[TILE(meetX, meetY)];
            if (best >= reach) {
                bestX = meetX;
                bestY = meetY;
                best = reach;
            }
        }
    }
    if (bestX == -1) {
        return;
    }
    func_020b2b70(bestX, bestY);
}
