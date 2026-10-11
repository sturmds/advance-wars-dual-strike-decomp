#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern Unk_027c0280 *data_027c0280;

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Clears the 2 by 2 characters of map tile (x, y) on layer 1 (the unit layer).
 * Not matched yet (97.6%): the first store addresses [rowOff*2, p] in the original
 * and [p, rowOff*2] here. */
void MapView_ClearUnitTile(int x, int y)
{
    int col = x - GAME->unk_1c4->unk_26;
    int row = y - GAME->unk_1c4->unk_28;
    u16 colOff = (col & 15) * 2;
    u16 rowOff = (row & 15) << 6;
    u16 block = (col & 16) ? 0x400 : 0;
    u16 *p = data_027c0280->layer1Map + block + colOff;
    u16 *entry;

    p[rowOff] = 0;
    entry = p + rowOff;
    entry[1] = 0;
    entry[32] = 0;
    entry[33] = 0;
}
