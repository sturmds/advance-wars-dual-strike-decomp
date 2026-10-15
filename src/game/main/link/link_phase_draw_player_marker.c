#include "types.h"
#include "game/menu_state.h"
#include "game/oam_cells.h"
#include "game/oam_buffer.h"
#include "game/math.h"

int MainLoop_GetFrameCount(void);
void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);
u16 Wireless_GetChildAids(void);
int Link_GetOwnPlayer(void);
int LinkScreen_FindPanelOfPlayer(u8 player);
int LinkScreen_GetPlayerRow(int panel);

#define DIVISOR(scale) ((scale) ? (scale) : 2)

#define SIN_0 gSineTable[0]
#define COS_0 gSineTable[64]

/* Draws a player's marker beside its panel on the link phase screen. A
 * player whose bit is clear in Wireless_GetChildAids() (bit 0 always counts) gets
 * tile 32 instead of its own. Ready players use palette 2. The player of
 * this console (Link_GetOwnPlayer), while not ready, gets a marker that pulses
 * in size through affine matrix 1. */
void LinkPhase_DrawPlayerMarker(int player) {
  int attr;
  int scale;

  if ((Wireless_GetChildAids() | 1) & (1 << player)) {
    attr = player * 4;
  } else {
    attr = 32;
  }
  if (LINK_READY_FLAG(gMenuState, player) != 0) {
    Sprite_Queue(
        0, 120,
        (LinkScreen_GetPlayerRow(LinkScreen_FindPanelOfPlayer(player)) * 8 +
         9) &
            0xff,
        &data_027c0040, attr | 0x2000, 0);
    return;
  }
  if (player != Link_GetOwnPlayer()) {
    Sprite_Queue(
        0, 120,
        (LinkScreen_GetPlayerRow(LinkScreen_FindPanelOfPlayer(player)) * 8 +
         9) &
            0xff,
        &data_027c0040, attr, 0);
    return;
  }
  scale = (Math_SinDegrees(HwMod32(MainLoop_GetFrameCount() * 16, 360)) >> 9) +
          0x100;
  Oam_SetAffineParams(1, HwDiv32(COS_0 * 16, DIVISOR(scale)),
                      HwDiv32(-SIN_0 * 16, DIVISOR(scale)),
                      HwDiv32(SIN_0 * 16, DIVISOR(scale)),
                      HwDiv32(COS_0 * 16, DIVISOR(scale)));
  Sprite_Queue(
      0, 0x270,
      ((LinkScreen_GetPlayerRow(LinkScreen_FindPanelOfPlayer(player)) * 8 + 1) &
       0xff) |
          0x300,
      &data_027c0040, attr | 0x1000, 0);
}
