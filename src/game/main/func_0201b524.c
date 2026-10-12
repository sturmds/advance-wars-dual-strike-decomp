#include "types.h"
#include "game/Unk_0201d320.h"
#include "game/screen_other_map.h"

extern Unk_0201d320 data_022977f4[];
void LinkPlayers_DefeatOtherTeams(int player, int arg);

BOOL func_0201b524(s16 index) {
  int player;
  Unk_0201d320* script = &data_022977f4[index];
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  player = script->unk_00->unk_08;

  if (player == 5) {
    player = data_027c027c->unk_48[data_027c0284]->currentPlayer;
  }
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  LinkPlayers_DefeatOtherTeams(player, 128);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  script->unk_00++;
  return 1;
}
