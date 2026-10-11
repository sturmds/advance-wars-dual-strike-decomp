#include "types.h"
#include "game/map_select.h"
#include "game/menu_button.h"

int Link_GetOwnPlayer(void);
u32 Link_GetConnectedSlotMask(void);

extern MenuButtonTemplate data_0215ce18;

static inline BOOL IsFirstLinkPlayer(void)
{
    return Link_GetOwnPlayer() == 0;
}

/* Closes the Vs setup buttons of a link game for player slot `player` once
 * it is ready: buttons 16 + player * 4 to + 3, the four buttons above and
 * below its COs (32 + player * 4 to + 3) and buttons 252 and 253. The first
 * link player also closes the CO buttons of each slot not set in the
 * Link_GetConnectedSlotMask mask; the other players get button 254 back. */
void VsSetup_CloseLinkButtons(int player)
{
    int other;

    MenuButtons_RemoveSelectable();
    MenuButton_Close(player * 4 + 16);
    MenuButton_Close(player * 4 + 17);
    MenuButton_Close(player * 4 + 18);
    MenuButton_Close(player * 4 + 19);
    MenuButton_Close(player * 4 + 32);
    MenuButton_Close(player * 4 + 33);
    MenuButton_Close(player * 4 + 34);
    MenuButton_Close(player * 4 + 35);
    MenuButton_Close(252);
    MenuButton_Close(253);
    if (IsFirstLinkPlayer() == 1) {
        for (other = 0; other < MENU->setup.unk_2e3; other++) {
            if ((Link_GetConnectedSlotMask() & (1 << other)) == 0) {
                MenuButton_Close(other * 4 + 32);
                MenuButton_Close(other * 4 + 33);
                MenuButton_Close(other * 4 + 34);
                MenuButton_Close(other * 4 + 35);
            }
        }
    }
    if (IsFirstLinkPlayer() == 0) {
        MenuButton_Add(0, 0, &data_0215ce18, 254);
    }
}
