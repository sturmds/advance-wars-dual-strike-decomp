#include "types.h"
#include "game/menu_state.h"
#include "game/screen_other_map.h"
#include "game/Unk_02160ac0.h"
#include "game/co_graphics.h"
#include "game/ov003_unit.h"

/* gMenuState as the CO pictures Ov003_LoadScreenGraphics puts there. */

void Mem_Copy32(const void* src, void* dst, u32 size);
int CombatCo_IdFromNumber(int value);
void Co_LoadPalette(int coId, int paletteSlot);
void Co_LoadArmyLargePicture(int coMood, void* dest, int paletteSlot, int army);

/* Puts the CO pictures of the rows into object VRAM of the second screen and
 * loads their palettes (slots 18 and up). Without data_02160ac0->unk_7c, the
 * pictures of rows 0 and 1 come from gMenuState, where
 * Ov003_LoadScreenGraphics put them. With it, a picture is loaded from its file
 * for each active row, 36 tiles apart. */
void Ov003_UploadCoPictures(void) {
  int i;
  Unk_027c0280* savedGfx;
  Unk_02080440* savedScreen;

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  if (data_02160ac0->unk_7c == 0) {
    Mem_Copy32(gMenuState.coPictures[0], data_027c0280->objVram + 0x1a00,
               CO_LARGE_PICTURE_SIZE);
    Co_LoadPalette(CombatCo_IdFromNumber(data_ov003_02364928->rows[0].co), 18);
    Mem_Copy32(gMenuState.coPictures[1], data_027c0280->objVram + 0x5000,
               CO_LARGE_PICTURE_SIZE);
    Co_LoadPalette(CombatCo_IdFromNumber(data_ov003_02364928->rows[1].co), 19);
  } else {
    for (i = 0; i < data_ov003_02364928->rowCount; i++) {
      if (data_ov003_02364928->rows[i].active) {
        Co_LoadArmyLargePicture(
            CombatCo_IdFromNumber(data_ov003_02364928->rows[i].co),
            data_027c0280->objVram + (208 + i * 36) * 32, i + 18, i + 1);
      }
    }
  }
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);
}
