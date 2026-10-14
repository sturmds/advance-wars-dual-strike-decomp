#include "types.h"
#include "game/menu_state.h"
#include "game/screen_other_map.h"
#include "game/oam_cells.h"

int MainLoop_GetFrameCount(void);
void Palette_Load(const void* src, int offset, int size);
void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);
int Link_IsActive(void);
int Link_GetOwnPlayer(void);
void VsSetup_DrawSettingRow(int row, int x, int y);

#define SETTING_ROWS 9

extern u8 gVsSetupSettingRowY[SETTING_ROWS];
extern u8 gVsSetupSettingRowX[SETTING_ROWS];
extern MenuState gMenuState;

/* Needed to match: the cast keeps the field offsets out of the literal pool. */
#define MENU ((MenuState*)&gMenuState)

/* Draws the nine setting rows of the Vs setup screen. In a link game, a
 * link player other than the first, while setup.unk_2eb is 0, also gets
 * two sprites at (172, 172) and (188, 172), and the palette colour at byte
 * 0x3d4 cycles through MenuState.colours every 4 frames. */
void VsSetup_DrawSettings(void) {
  int row;
  Unk_027c0280* saved = data_027c0280;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  for (row = 0; row < SETTING_ROWS; row++) {
    VsSetup_DrawSettingRow(row, gVsSetupSettingRowX[row],
                           gVsSetupSettingRowY[row]);
  }
  Screen_SetCurrentGfx(saved);
  if (Link_IsActive() != 1) {
    return;
  }
  if (Link_GetOwnPlayer() == 0) {
    return;
  }
  if (MENU->setup.unk_2eb != 0) {
    return;
  }
  Sprite_Queue(0, 172, 172, &data_027c0040, 0xe000, 0x600);
  Sprite_Queue(0, 188, 172, &data_027c0108, 0xe000, 0x624);
  Palette_Load(&MENU->colours[(MainLoop_GetFrameCount() >> 2) & 15], 0x3d4, 2);
}
