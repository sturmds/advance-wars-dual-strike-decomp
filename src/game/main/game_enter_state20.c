#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

void func_020c0820(void);
int MapScript_RunList14(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Runs func_020c0820 and the map script's list 14, then switches unk_18 to 20. */
void Game_EnterState20(void)
{
    func_020c0820();
    MapScript_RunList14();
    STATE->unk_18 = 20;
}
