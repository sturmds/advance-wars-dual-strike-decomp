#include "types.h"
#include "game/screen_other_map.h"

void Screen_SetCurrentBoth(void* state);
void ForceRank_LoadSets(int mode);
void func_020d6a1c(void);

/* Puts both screens in game mode 2, loads the Force Rank sets of mode 2 and
 * starts the Versus task through func_020d6a1c. */
void Versus_Start(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c = data_027c027c;

  Screen_SetCurrentBoth(data_027c0288.unk_00);
  data_027c027c->unk_48[data_027c0284]->gameMode = GAME_MODE_VERSUS;
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  data_027c027c->unk_48[data_027c0284]->gameMode = GAME_MODE_VERSUS;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  ForceRank_LoadSets(2);
  func_020d6a1c();
}
