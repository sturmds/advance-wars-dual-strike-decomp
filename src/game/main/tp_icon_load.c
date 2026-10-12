#include "types.h"
#include "game/screen_other_map.h"

const char* File_LocalizePath(const char* path);
void File_UnpackTo(void* dest, const char* fileName);

extern char data_02159910[];

/* Unpacks the localized /ohashi/res_tpicon_lang (data_02159910) into object
 * VRAM at offset 0x5c00, with data_027c0288.unk_00 as the current screen. */
void TpIcon_Load(void) {
  Unk_027c0280* savedGfx = data_027c0280;
  Unk_02080440* savedState;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  File_UnpackTo(data_027c0280->objVram + 0x5c00,
                File_LocalizePath(data_02159910));
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
}
