#include "types.h"
#include "game/Unk_02080440.h"

void func_020ef8d8(void);
void func_020f138c(void);

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Runs func_020ef8d8 and func_020f138c unless the view is still scrolling
 * (any of the four direction bits in scrollFlags; func_02080440 makes the
 * same test before AiTurn_ShowAction). Their purpose is unknown. */
void func_020c40d0(void)
{
    if ((STATE->unk_1c4->scrollFlags & 0xf) != 0) {
        return;
    }
    func_020ef8d8();
    func_020f138c();
}
