#include "types.h"
#include "game/screen_other_map.h"

void FS_ChangeDir(const char* dir);
void File_UnpackTo(void* dest, const char* fileName);
void Palette_LoadFromFile(const char* fileName, int fileOffset,
                          int paletteOffset, int size);

extern const char data_ov006_0236b6ac[];
extern const char data_ov006_0236b6b8[];
extern const char data_ov006_0236b6bc[];

/* On screen 0, loads sprite tiles (at byte 0x4bc0 of sprite VRAM) and a 64-byte
 * palette (at 0x200) from the overlay file directory. The slot buttons use them. */
void Ov006_LoadSlotButtonGraphics(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  FS_ChangeDir(data_ov006_0236b6ac);
  File_UnpackTo(data_027c0280->objVram + 0x4bc0, data_ov006_0236b6b8);
  Palette_LoadFromFile(data_ov006_0236b6bc, 0, 0x200, 0x40);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
