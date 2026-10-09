#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE_INDEX(x, y) ((x) + STATE->mapTiles->rowStart[y])

int Player_GetUnitMoveRange(int player, int unitType);
void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
int Unit_IsTileOwnedByTeam(int unitIndex, int tile);
BOOL Terrain_IsHqBaseAirportPortOrTower(u8 tile);
int func_0209f890(int x1, int y1, s16 x2, s16 y2);   /* distance in tiles, Manhattan */
void func_0208c46c(s8 *map, int value);

/* AI queue routine. For each unk_6088 entry whose tile the current player's
 * team does not own, floods the map with an infantry move range and updates
 * MapTiles.unk_3c60 on the tiles reached: bit 7 set, bit 6 set when the
 * entry is within a Mech's move range (straight-line tile distance), and the
 * weight in bits 0-5 raised by 8 for an HQ, base, airport, port or Com
 * Tower and by 1 otherwise, up to 63. The names are guesses. */
void AiTurn_BuildCaptureMap(void)
{
    Unk_02080440_6088 *entry;
    int infantryRange;
    int weight;
    int mechRange;
    int x;
    int y;

    entry = STATE->unk_6088;
    infantryRange = Player_GetUnitMoveRange(STATE->currentPlayer, UNIT_INFANTRY);
    mechRange = Player_GetUnitMoveRange(STATE->currentPlayer, UNIT_MECH);
    func_0208c46c(STATE->mapTiles->unk_3c60, 0);
    /* needed to match: the pointer starts one entry back and moves first */
    entry--;
    for (;;) {
        if ((++entry)->unk_00 == 0xff) {
            break;
        }
        if (Unit_IsTileOwnedByTeam((s16)STATE->currentUnitBase, STATE->mapTiles->tiles[TILE_INDEX(entry->x, entry->y)]) == 1) {
            continue;
        }
        RangeMap_FillMoves(entry->x, entry->y, UNIT_INFANTRY, infantryRange, 0);
        if (Terrain_IsHqBaseAirportPortOrTower(STATE->mapTiles->tiles[TILE_INDEX(entry->x, entry->y)]) == 1) {
            weight = 8;
        } else {
            weight = 1;
        }
        for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
            for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
                if (STATE->rangeMap[y][x] < 0) {
                    continue;
                }
                STATE->mapTiles->unk_3c60[TILE_INDEX(x, y)] |= 0x80;
                if (func_0209f890(entry->x, entry->y, x, y) <= mechRange) {
                    STATE->mapTiles->unk_3c60[TILE_INDEX(x, y)] |= 0x40;
                }
                if (((u8)STATE->mapTiles->unk_3c60[TILE_INDEX(x, y)] & 0x3f) + weight < 63) {
                    STATE->mapTiles->unk_3c60[TILE_INDEX(x, y)] += weight;
                } else {
                    STATE->mapTiles->unk_3c60[TILE_INDEX(x, y)] |= 0x3f;
                }
            }
        }
    }
}
