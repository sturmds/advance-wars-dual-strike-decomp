#include "types.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u32 gLinkPlay;

void func_020ddb38(void);
void Link_SendWeather(int arg);

/* AI state 6: runs func_020ddb38 and goes back to AI_STATE_START. The name
 * is a guess from that; Link_SendWeather runs only when gLinkPlay is set. */
void AiTurn_Restart(void) {
  func_020ddb38();
  gAiTurnState->state = AI_STATE_START;
  if (gLinkPlay != 0) {
    Link_SendWeather(
        data_027c027c->unk_48[data_027c0284]->unk_1c4->nextWeather);
  }
}
