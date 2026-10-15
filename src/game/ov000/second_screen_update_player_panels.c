#include "types.h"
#include "game/map_header.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/screen_other_map.h"

extern int data_ov000_0234eb84[][5];

u8 Map_GetLoadedPlayerCount(void);
int MainLoop_GetFrameCount(void);
void PlayerHud_DrawPlayer(int a0, int a1, int player);
void Player_AnimatePaletteColour(int player, int frame);

/* On the second screen: for each player slot 1-4 that has a controller, calls
 * PlayerHud_DrawPlayer and Player_AnimatePaletteColour (the latter checks CanUseCoPower)
 * with a column from data_ov000_0234eb84 (rows indexed by the loaded player count). */
void SecondScreen_UpdatePlayerPanels(void) {
  Unk_027c0280* savedGfx = data_027c0280;
  int i;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  for (i = 1; i < ARMY_SLOT_COUNT; i++) {
    if (MAP_STATE->players[i].controller != PLAYER_CONTROLLER_NONE) {
      int column = data_ov000_0234eb84[Map_GetLoadedPlayerCount()][i];
      PlayerHud_DrawPlayer(192, (column << 3) + 4, i);
      Player_AnimatePaletteColour(i, MainLoop_GetFrameCount());
    }
  }
  Screen_SetCurrentGfx(savedGfx);
}
