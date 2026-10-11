#include "types.h"
#include "game/Unk_02080440.h"

void func_020ef7f0(void);
void MapCursor_UpdateFromTouchFree(void);
extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
#define STATE (data_027c027c->unk_48[data_027c0284])

void func_020c407c(void)
{
    if ((STATE->unk_1c4->scrollFlags & 0xf) != 0) {
        return;
    }
    func_020ef7f0();
    MapCursor_UpdateFromTouchFree();
}
