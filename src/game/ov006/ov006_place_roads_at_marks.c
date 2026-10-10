#include "types.h"
#include "game/Unk_02080440.h"

void Ov006_SetMapCell(int x, int y, int cell);
int func_ov006_0235b300(int x, int y);
int func_ov006_0235b868(int x, int y);

extern Unk_02080440 *data_027c027c;
extern u32 data_027c0284;
extern s8 data_ov006_0236d744[][30];

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Sets road (cell 97) on every tile marked 2 in data_ov006_0236d744, except where
 * func_ov006_0235b868 or func_ov006_0235b300 returns 1. */
void Ov006_PlaceRoadsAtMarks(void)
{
    int x;
    int y;

    for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
        for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
            if (data_ov006_0236d744[y][x] == 2 && func_ov006_0235b868(x, y) != 1
                && func_ov006_0235b300(x, y) != 1) {
                Ov006_SetMapCell(x, y, 97);
            }
        }
    }
}
