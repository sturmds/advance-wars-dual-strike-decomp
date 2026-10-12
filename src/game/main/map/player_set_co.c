#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Screen_SetCurrentBoth(void* state);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;

#define GAME (data_027c027c->unk_48[data_027c0284])

#define MAIN_MAP (data_027c0288.unk_00)
#define SECOND_MAP (data_027c0288.unk_04)
#define OTHER_MAP ((&data_027c0288.unk_00)[1 - data_027c0284])

/* Sets a player's front (which 0) or back (which 1) CO. In a two-map
 * battle whose other map has unk_1c4->unk_30 clear, each map gets one CO:
 * the front CO goes to the main map's player and the back CO becomes the
 * same player's front CO on the second map. */
void Player_SetCo(int slot, int which, int coId) {
  Unk_02080440* saved027c;
  Unk_02080440* inner027c;
  Unk_027c0280* saved0280;
  u32 otherUnk30;

  /* needed to match: declaration order above sets the registers */
  saved027c = data_027c027c;
  saved0280 = data_027c0280;

  if (GAME->secondMapId != 0) {
    Screen_SetCurrentGfx(OTHER_MAP);
    inner027c = data_027c027c;
    Screen_SetCurrent(OTHER_MAP);
    otherUnk30 = GAME->unk_1c4->unk_30;
    Screen_SetCurrent(inner027c);
    Screen_SetCurrentGfx(saved0280);
    if (otherUnk30 == 0) {
      if (which == 0) {
        Screen_SetCurrentBoth(MAIN_MAP);
      } else {
        Screen_SetCurrentBoth(SECOND_MAP);
      }
      GAME->players[slot].front.id = coId;
      goto restore;
    }
  }
  Screen_SetCurrentBoth(MAIN_MAP);
  (&GAME->players[slot].front)[which].id = coId;
restore:
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
