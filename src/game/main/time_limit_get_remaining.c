#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_022a7b80.h"

u32 Survival_GetTimeLeft(void);
u32 TimeLimit_GetFramesLeft(void);

extern Unk_022a7b80 data_022a7b80;

/* Looks at the first screen's game for a time limit: the Survival variants 0
 * and 3 (budget in Survival_GetTimeLeft) or a map limit (timeLimitFrames). Stores the
 * frames left in framesLeft (a map limit overrides the Survival one) and returns TRUE when there
 * is a limit, otherwise stores 0 and returns FALSE. The nested tests and the
 * repeated state reads are needed to match. */
BOOL TimeLimit_GetRemaining(u32* framesLeft) {
  BOOL found;
  BOOL survivalTimed;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  u32 mapLimit;
  BOOL timedVariant;

  found = FALSE;
  *framesLeft = 0;
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  survivalTimed = FALSE;
  if (data_027c027c->unk_48[data_027c0284]->gameMode == 3) {
    timedVariant = TRUE;
    if (data_022a7b80.mode != 0) {
      if (data_022a7b80.mode != 3) {
        timedVariant = FALSE;
      }
    }
    if (timedVariant) {
      survivalTimed = TRUE;
    }
  }
  mapLimit = data_027c027c->unk_48[data_027c0284]->timeLimitFrames;
  if (survivalTimed) {
    *framesLeft = Survival_GetTimeLeft();
    found = TRUE;
  }
  if (mapLimit != 0) {
    *framesLeft = TimeLimit_GetFramesLeft();
    found = TRUE;
  }
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  return found;
}
