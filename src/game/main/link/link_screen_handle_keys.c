#include "types.h"
#include "game/keypad.h"
#include "game/menu_state.h"

extern MenuState gMenuState;

int func_0207bd6c(void);
void func_020729c4(void);
void LinkScreen_PushReady(void);
void LinkScreen_PushMoveDown(void);
void LinkScreen_PushMoveUp(void);

/* Keys of the link screen: up and down move this console's player, A is ready,
 * B backs out (queued as link messages, see LinkPhase_Update). Up, down and A
 * are ignored once ready. */
void LinkScreen_HandleKeys(void)
{
    if (LINK_READY_FLAG(gMenuState, func_0207bd6c()) == 0) {
        if (gKeypad->repeat & PAD_KEY_UP) {
            LinkScreen_PushMoveUp();
        }
        if (gKeypad->repeat & PAD_KEY_DOWN) {
            LinkScreen_PushMoveDown();
        }
        if (gKeypad->pressed & PAD_BUTTON_A) {
            LinkScreen_PushReady();
            return;
        }
    }
    if (gKeypad->pressed & PAD_BUTTON_B) {
        func_020729c4();
    }
}
