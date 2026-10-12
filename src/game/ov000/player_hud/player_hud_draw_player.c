#include "types.h"
#include "game/screen_other_map.h"

#define STATE (data_027c027c->unk_48[data_027c0284])

BOOL Player_IsInGame(u8 player);
void PlayerHud_DrawSlot(int x, int y, int slot, int frontCo, int backCo,
                        int frontState, int backState, int frontFromOtherMap,
                        int backFromOtherMap, int backFlag);

/* Draws a player's HUD slot at (x, y) with the player's COs. A player with no
 * back CO, in a two-map battle whose other map has unk_30 clear, shows the
 * front CO of the same slot on the other map in the back position. Both COs
 * show the front CO's power state. */
void PlayerHud_DrawPlayer(int x, int y, int slot) {
  int backCo = 0;
  int frontCo;
  int frontState;
  int backState = 0;
  int fromOtherMap = 0;
  int backFlag = 0;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  if (!Player_IsInGame(slot)) {
    return;
  }
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx((&data_027c0288.unk_00)[data_027c0284]);
  saved027c = data_027c027c;
  Screen_SetCurrent((&data_027c0288.unk_00)[data_027c0284]);
  frontCo = STATE->players[slot].front.id;
  frontState = STATE->players[slot].front.powerState;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  if (STATE->players[slot].back.id != 0) {
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx((&data_027c0288.unk_00)[data_027c0284]);
    saved027c = data_027c027c;
    Screen_SetCurrent((&data_027c0288.unk_00)[data_027c0284]);
    fromOtherMap = 0;
    backCo = STATE->players[slot].back.id;
    backState = STATE->players[slot].front.powerState;
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  } else if (STATE->secondMapId != 0) {
    if (OtherMap_GetUnk30() == 0) {
      saved0280 = data_027c0280;
      Screen_SetCurrentGfx((&data_027c0288.unk_00)[1 - data_027c0284]);
      saved027c = data_027c027c;
      Screen_SetCurrent((&data_027c0288.unk_00)[1 - data_027c0284]);
      fromOtherMap = 1;
      backCo = STATE->players[slot].front.id;
      backState = STATE->players[slot].front.powerState;
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
      backFlag = fromOtherMap;
    }
  }
  PlayerHud_DrawSlot(x, y, slot, frontCo, backCo, frontState, backState, 0,
                     fromOtherMap, backFlag);
}
