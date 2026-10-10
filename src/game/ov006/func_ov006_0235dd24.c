#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

int func_ov006_02351238(int x, int y);   /* guess: the cell at x, y turned into its final tile */
void Ov006_StoreMapCell(int x, int y, int value);

/* Rewrites the cell at x, y and the one to its right with the tile that
 * func_ov006_02351238 returns for them. */
void func_ov006_0235dd24(int x, int y)
{
    Ov006_StoreMapCell(x, y, func_ov006_02351238(x, y));
    if (x < data_027c027c->unk_48[data_027c0284]->unk_1c4->mapWidth - 2) {
        Ov006_StoreMapCell(x + 1, y, func_ov006_02351238(x + 1, y));
    }
}
