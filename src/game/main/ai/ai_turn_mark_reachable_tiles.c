#include "types.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

void func_01ffb200(s8 *base);       /* points GAME->rangeMap rows into a per-tile map */
void func_01ffb3a0(int value);      /* fills the range map with value */
void RangeMap_FillMoves(int x, int y, int unitType, s16 movement, int arg4);

/* Floods the map on foot from each entry of the unk_6088 table (infantry
 * with 110 movement, so effectively unlimited), setting unk_2d48 to 1 on
 * every tile reached. An entry on a tile already marked is skipped, so
 * propertyRegions counts the separate land regions that hold an entry. */
void AiTurn_MarkReachableTiles(void)
{
    int count;
    int x;
    int y;
    Unk_02080440_6088 *entry;

    count = 0;
    entry = GAME->unk_6088;
    func_01ffb200(GAME->mapTiles->unk_2d48);
    func_01ffb3a0(0);
    func_01ffb200(GAME->mapTiles->unk_2840);
    for (; entry->unk_00 != 0xff; entry++) {
        if (GAME->mapTiles->unk_2d48[entry->x + GAME->mapTiles->rowStart[entry->y]] != 0) {
            continue;
        }
        RangeMap_FillMoves(entry->x, entry->y, UNIT_INFANTRY, 110, 0);
        for (y = 0; y < GAME->unk_1c4->mapHeight; y++) {
            for (x = 0; x < GAME->unk_1c4->mapWidth; x++) {
                if (GAME->rangeMap[y][x] >= 0) {
                    GAME->mapTiles->unk_2d48[x + GAME->mapTiles->rowStart[y]] = 1;
                }
            }
        }
        count++;
    }
    gAiTurnState->propertyRegions = count;
}
