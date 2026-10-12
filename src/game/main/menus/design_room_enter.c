#include "types.h"
#include "game/song.h"
#include "game/screen_other_map.h"

void Screen_SetCurrentBoth(void* state);
void func_020015ac(void);
void func_ov006_02350ec0(int arg);

/* Puts both screens in game mode 4, stops the song, swaps overlay 1 for the
 * Design Room overlay and starts its task. */
void DesignRoom_Enter(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c = data_027c027c;

  Screen_SetCurrentBoth(data_027c0288.unk_00);
  data_027c027c->unk_48[data_027c0284]->gameMode = GAME_MODE_DESIGN_ROOM;
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  data_027c027c->unk_48[data_027c0284]->gameMode = GAME_MODE_DESIGN_ROOM;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  Sound_FadeOutSong();
  func_020015ac();
  func_ov006_02350ec0(0);
}
