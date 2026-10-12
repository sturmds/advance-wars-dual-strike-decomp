#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov004_022bf158.h"

void FS_ChangeDir(const char* dir);
void File_UnpackTo(void* dest, const char* fileName);
void Palette_LoadFromFile(const char* fileName, int fileOffset,
                          int paletteOffset, int size);

extern char data_ov004_022bf0dc[]; /* directory */
extern char data_ov004_022bf0f0[]; /* sprite tiles */
extern char data_ov004_022bf0f4[]; /* sprite palette */

void Ov004_LoadBottomSpriteGraphics(Unk_ov004_022bf158* state) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;

  FS_ChangeDir(data_ov004_022bf0dc);
  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  File_UnpackTo(data_027c0280->objVram + 0x800, data_ov004_022bf0f0);
  Palette_LoadFromFile(data_ov004_022bf0f4, 0, 0x220, 32);
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
  state->unk_1c0c = 0;
}
