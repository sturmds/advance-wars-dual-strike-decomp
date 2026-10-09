#include "types.h"
#include "game/Unk_02080440.h"

BOOL func_020f5540(int *x, int *y);

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Object at offset 0x50 of the global state, one per screen like unk_48. */
typedef struct Unk_02080440_50 {
    u8 pad_00[0x18];
    void (*callback)(void);            /* name is a guess */
} Unk_02080440_50;

#define STATE_50 (((Unk_02080440_50 **)&data_027c027c->unk_48[2])[data_027c0284])

/* Takes a position through func_020f5540 (which reads x and y by address, as a
 * variadic call), stores it times 16 in the map state at unk_1e and unk_20, then runs
 * the callback of the state at offset 0x50 if there is one. */
void func_020f53e8(int x, int y, ...)
{
    if (!func_020f5540(&x, &y)) {
        return;
    }
    GAME->unk_1c4->unk_1e = x << 4;
    GAME->unk_1c4->unk_20 = y << 4;
    if (STATE_50->callback == NULL) {
        return;
    }
    STATE_50->callback();
}
