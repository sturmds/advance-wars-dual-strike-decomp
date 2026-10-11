#include "types.h"
#include "game/menu_button.h"
#include "game/menu_state.h"

int func_0207bd80(void);
int func_0207bd6c(void);
int Link_FindSlotOfPlayer(int player);
int VsSetup_GetSlotTileX(int player);
u32 Link_GetConnectedSlotMask(void);

extern MenuState gMenuState;
extern MenuButtonTemplate data_0215cc38;

static inline BOOL IsFirstLinkPlayer(void)
{
    return func_0207bd6c() == 0;
}

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState *)&gMenuState)

/* Buttons 48 and up, one per player slot of the Vs setup screen, 8 pixels
 * apart per column from VsSetup_GetSlotTileX. In a link game the own slot gets one;
 * player 0 also gets one for each slot not set in the mask Link_GetConnectedSlotMask
 * returns. The name is a guess. */
void VsSetup_AddPlayerButtons(void)
{
    int i;
    int slot;

    if (func_0207bd80() == 1) {
        slot = Link_FindSlotOfPlayer(func_0207bd6c()) - 1;
        MenuButton_Add(VsSetup_GetSlotTileX(slot) * 8 + 8, 64, &data_0215cc38, slot + 48);
        if (IsFirstLinkPlayer() != 1) {
            return;
        }
        for (i = 0; i < MENU->setup.unk_2e3; i++) {
            if ((Link_GetConnectedSlotMask() & (1 << i)) == 0) {
                MenuButton_Add(VsSetup_GetSlotTileX(i) * 8 + 8, 64, &data_0215cc38, i + 48);
            }
        }
    } else {
        for (i = 0; i < MENU->setup.unk_2e3; i++) {
            MenuButton_Add(VsSetup_GetSlotTileX(i) * 8 + 8, 64, &data_0215cc38, i + 48);
        }
    }
}
