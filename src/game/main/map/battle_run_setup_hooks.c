#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define HOOKS (data_027c027c->unk_50[data_027c0284])

void func_020baf9c(void);
void func_020f757c(void);

/* Battle start step: two setup calls, then the screen config's unk_54 hook
 * with 1, as Battle_SetupWithPresetCos does. */
void Battle_RunSetupHooks(void)
{
    func_020baf9c();
    func_020f757c();
    if (HOOKS->unk_54 != NULL) {
        HOOKS->unk_54(1);
    }
}
