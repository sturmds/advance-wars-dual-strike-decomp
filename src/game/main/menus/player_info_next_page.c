#include "types.h"
#include "game/menu_button.h"
#include "game/menu_state.h"

extern MenuState gMenuState;
extern MenuButtonTemplate data_0215d330;
extern MenuButtonTemplate data_0215d380;

void Sound_PlaySe(u32 soundId);
void PlayerInfo_StartRedrawTask(void);

/* Turns the player info screen to the next page (0 to 3). The previous page
 * button (253) appears on page 1; the next page button (252) is replaced by
 * button 251 on the last page. Needed to match: the first read of the page is
 * volatile, because the original loads it again for the increment. */
void PlayerInfo_NextPage(MenuButton *button)
{
    if (*(volatile u8 *)&gMenuState.unk_0206d67c.page >= 3) {
        return;
    }
    gMenuState.unk_0206d67c.page++;
    Sound_PlaySe(0x10002);
    if (gMenuState.unk_0206d67c.page == 1) {
        MenuButton_Add(0, 0, &data_0215d330, 253);
    }
    if (gMenuState.unk_0206d67c.page == 3) {
        MenuButton_Close(252);
    }
    if (gMenuState.unk_0206d67c.page == 3) {
        MenuButton_Add(0, 0, &data_0215d380, 251);
    }
    PlayerInfo_StartRedrawTask();
}
