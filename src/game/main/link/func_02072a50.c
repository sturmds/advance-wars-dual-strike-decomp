#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/menu_state.h"

void Screen_SetCurrent(void *state);
void Screen_SetCurrentGfx(void *state);
void func_0202c50c(void);

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern Unk_027c0288 data_027c0288;
extern MenuState gMenuState;
extern u8 gLinkSlotPlayers[4];

/* Task callback of the link screen (best attempt, 15 of 44 instructions differ):
 * destroys the screen's tasks, then stores each panel's player + 1 back into
 * gLinkSlotPlayers. */
void func_02072a50(void)
{
    Unk_027c0280 *saved0280;
    Unk_02080440 *saved027c;
    int i;
    u8 *dst;

    func_0202c50c();
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    func_0202c50c();
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
    for (i = 0, dst = gLinkSlotPlayers; i < gMenuState.link.playerCount; i++, dst++) {
        *dst = LINK_PANEL_PLAYER(gMenuState, i) + 1;
    }
}
