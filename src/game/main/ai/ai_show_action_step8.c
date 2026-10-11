#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/keypad.h"
#include "game/unit_action.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern int data_0229eac8;

#define GAME (data_027c027c->unk_48[data_027c0284])

int Link_IsActive(void);
void Map_ScrollViewToTile(int arg);

/* Holds this step for 30 frames, or until A is held when Link_IsActive
 * returns 0, then moves to step 4. data_0229eac8 counts the frames. */
void AiShowAction_Step8(void)
{
    int skip = 0;

    if (Link_IsActive() == 0) {
        skip = gKeypad->held & PAD_BUTTON_A;
    }
    GAME->cursorX = gUnitAction.target;
    GAME->cursorY = gUnitAction.unk_07;
    Map_ScrollViewToTile(2);
    data_0229eac8++;
    if (data_0229eac8 > 30 || skip) {
        gAiTurnState->actionStep = 4;
    }
}
