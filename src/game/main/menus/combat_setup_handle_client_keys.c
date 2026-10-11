#include "types.h"
#include "game/keypad.h"
#include "game/menu_state.h"
#include "game/Unk_0229fb30.h"

void func_0208502c(void);
void func_02085040(void);
void func_02085054(void);
void func_02085068(void);

extern MenuState gMenuState;

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState *)&gMenuState)

/* Keys of the link client on the Combat setup screen. While the local
 * player's row is not confirmed, up and down send the previous and next
 * CO messages (18 and 19) and A sends the confirm message (16). B sends
 * the message that reopens the row (17). */
void CombatSetup_HandleClientKeys(void)
{
    if (MENU->combat.rows[MENU->combat.players[data_0229fb30.unk_45a].row - 1].unk_08 == 0) {
        if (gKeypad->repeat & PAD_KEY_UP) {
            func_02085068();
        }
        if (gKeypad->repeat & PAD_KEY_DOWN) {
            func_02085054();
        }
        if (gKeypad->pressed & PAD_BUTTON_A) {
            func_02085040();
            return;
        }
    }
    if (gKeypad->pressed & PAD_BUTTON_B) {
        func_0208502c();
    }
}
