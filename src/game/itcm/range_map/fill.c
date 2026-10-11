#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Sets every tile rangeMap points to. s8 value matches the one-byte store. */
void RangeMap_Fill(s8 value)
{
    int col;
    int row;

    for (row = 0; row < STATE->unk_1c4->mapHeight; row++) {
        for (col = 0; col < STATE->unk_1c4->mapWidth; col++) {
            STATE->rangeMap[row][col] = value;
        }
    }
}
