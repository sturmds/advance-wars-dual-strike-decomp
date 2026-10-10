#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

void func_ov006_023590c0(int x, int y);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Calls func_ov006_023590c0 on every tile of the map, row by row. */
void func_ov006_0235cc6c(void)
{
    int x;
    int y;

    for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
        for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
            func_ov006_023590c0(x, y);
        }
    }
}
