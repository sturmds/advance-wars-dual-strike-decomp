#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/keypad.h"
#include "game/unit_action.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
int data_0229eac8;
void* data_0229eacc[2];

#define GAME (data_027c027c->unk_48[data_027c0284])

int Link_IsActive(void);
void Map_ScrollViewToTile(int arg);

/* Same wait as AiShowAction_Step8, then moves to step 5. */
void AiShowAction_Step9(void) {
  int skip = 0;

  if (Link_IsActive() == 0) {
    skip = gKeypad->held & PAD_BUTTON_A;
  }
  GAME->cursorX = gUnitAction.target;
  GAME->cursorY = gUnitAction.unk_07;
  Map_ScrollViewToTile(2);
  data_0229eac8++;
  if (data_0229eac8 > 30 || skip) {
    gAiTurnState->actionStep = 5;
  }
}
