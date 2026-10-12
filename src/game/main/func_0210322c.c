#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"

extern const TaskScriptCmd data_0216718c[];

void Screen_SetCurrentBoth(void* state);
void ForceRank_LoadSets(int mode);

/* Sets gameMode 1 on both screens, loads the Force Rank sets (mode 1) and
 * starts the data_0216718c task. */
void func_0210322c(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c = data_027c027c;

  Screen_SetCurrentBoth(data_027c0288.unk_00);
  data_027c027c->unk_48[data_027c0284]->gameMode = GAME_MODE_WAR_ROOM;
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  data_027c027c->unk_48[data_027c0284]->gameMode = GAME_MODE_WAR_ROOM;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  ForceRank_LoadSets(1);
  Task_Create(data_0216718c, TASK_LIST(3));
}
