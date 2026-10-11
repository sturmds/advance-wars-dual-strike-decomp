#include "types.h"
#include "game/menu_state.h"

int Sound_PlaySe(int);
int Link_FindSlotOfPlayer(int player);
int func_0206b5cc(int player);
void func_0206b748(int slot, u16 x, int y);
int func_0207bd6c(void);
void func_02066a40(int slot);

extern MenuState gMenuState;

/* Marks a slot ready on the team screen (once): plays a sound, sets its flag
 * in setup.unk_2d0 and calls func_0206b748 at the slot position, y 72 (a
 * ready marker, a guess). On the own slot of this machine it also removes
 * the team arrow buttons (func_02066a40). Shifted view: see VsSetup_NextTeam. */
void VsSetup_SetSlotReady(int slot)
{
    MenuState *shifted = (MenuState *)((u8 *)&gMenuState + slot);

    if (shifted->setup.unk_2d0[0] != 0) {
        return;
    }
    Sound_PlaySe(0x10006);
    shifted->setup.unk_2d0[0] = 1;
    func_0206b748(slot, func_0206b5cc(slot) * 8 + 16, 72);
    if (slot != Link_FindSlotOfPlayer(func_0207bd6c()) - 1) {
        return;
    }
    func_02066a40(slot);
}
