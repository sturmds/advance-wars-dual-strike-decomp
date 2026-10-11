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

/* Like AiShowAction_Step8, but gUnitAction.target is a unit slot here:
 * the step points Map_ScrollViewToTile at that unit when its tile is nonzero in
 * unk_2338, then moves to step 3 after 30 frames or A. */
void AiShowAction_Step7(void)
{
    MapUnit *unit = &GAME->units[gUnitAction.target];
    int skip = 0;

    if (Link_IsActive() == 0) {
        skip = gKeypad->held & PAD_BUTTON_A;
    }
    if (GAME->mapTiles->unk_2338[GAME->mapTiles->rowStart[unit->y] + unit->x] != 0) {
        GAME->cursorX = unit->x;
        GAME->cursorY = unit->y;
        Map_ScrollViewToTile(2);
    }
    data_0229eac8++;
    if (data_0229eac8 > 30 || skip) {
        gAiTurnState->actionStep = 3;
    }
}
