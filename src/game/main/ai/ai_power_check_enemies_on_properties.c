#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

BOOL Player_IsInGame(u8 player);
int func_020995a4(void);
BOOL func_0209f040(u8 terrain); /* a property other than a missile silo */

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

/* An AI profile power hook (AiProfile.unk_00, see AiPlayer_TryUsePower).
 * For a Super CO Power (powerKind 2) it agrees only when func_020995a4
 * does (gAiTurnState->phase is 3). For a CO Power it agrees when enemy units stand on properties:
 * at least 5 of them, or at least a fifth of all enemy units (slots 1-50,
 * not Oozium or carried units). */
BOOL AiPowerCheck_EnemiesOnProperties(int powerKind)
{
    int total = 0;
    int onProperty = 0;
    int player;
    int i;
    MapUnit *unit;

    if (powerKind == 2) {
        if (func_020995a4() == 1) {
            return TRUE;
        }
    } else {
        for (player = 1; player < 5; player++) {
            if (Player_IsInGame(player) && STATE->players[STATE->currentPlayer].team != STATE->players[player].team) {
                for (i = Ov000_gPlayerFirstUnitSlot[player] + 1; i < Ov000_gPlayerFirstUnitSlot[player] + 51; i++) {
                    unit = &STATE->units[i];
                    if (unit->type != UNIT_NONE && unit->type != UNIT_OOZIUM && !(unit->flags & MAP_UNIT_CARRIED)) {
                        if (func_0209f040(MAP_TILE_TERRAIN(STATE->mapTiles->tiles[TILE(unit->x, unit->y)])) == TRUE) {
                            onProperty++;
                        }
                        total++;
                    }
                }
            }
        }
        if (onProperty == 0) {
            return FALSE;
        }
        if (onProperty * 5 >= total || onProperty >= 5) {
            return TRUE;
        }
    }
    return FALSE;
}
