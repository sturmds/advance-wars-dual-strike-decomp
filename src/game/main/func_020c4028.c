#include "types.h"
#include "game/Unk_02080440.h"

void func_020ef664(void);
void MapCursor_UpdateFromTouch(void);
extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
#define STATE (data_027c027c->unk_48[data_027c0284])

void func_020c4028(void)
{
    if ((STATE->unk_1c4->scrollFlags & 0xf) != 0) {
        return;
    }
    func_020ef664();
    MapCursor_UpdateFromTouch();
}
