#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Cells 21 and 22 are the two bridge pieces: func_ov006_0235fa7c places 21
 * across a horizontal gap and 22 across a vertical one. */
BOOL Ov006_IsBridgeCell(int x, int y)
{
    u16 cell = STATE->mapTiles->mapCells[x + STATE->mapTiles->rowStart[y]];

    return (u32)(cell - 21) <= 1;
}
