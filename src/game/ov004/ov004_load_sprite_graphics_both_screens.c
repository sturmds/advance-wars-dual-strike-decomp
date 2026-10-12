#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov004_022bf158.h"

void FS_ChangeDir(const char* dir);
void File_UnpackTo(void* dest, const char* fileName);
void File_ReadTo(void* dest, const char* fileName);
void Palette_LoadFromFile(const char* fileName, int fileOffset,
                          int paletteOffset, int size);

extern char data_ov004_022bf0dc[]; /* directory */
extern char
    data_ov004_022bf0f8[]; /* sprite data read by Ov004Task_InitSprite */
extern char data_ov004_022bf0fc[]; /* sprite tiles */
extern char data_ov004_022bf100[]; /* sprite palette */

/* Loads the sprite data into the state's buffer, then the same sprite tiles
 * and palette into OBJ VRAM of both screens. */
void Ov004_LoadSpriteGraphicsBothScreens(Unk_ov004_022bf158* state) {
  u8* buffer;
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;

  FS_ChangeDir(data_ov004_022bf0dc);
  buffer = (u8*)state->buffer + 0x8000;
  File_UnpackTo(buffer, data_ov004_022bf0f8);
  File_ReadTo(buffer, data_ov004_022bf0f8);

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  File_UnpackTo(data_027c0280->objVram, data_ov004_022bf0fc);
  Palette_LoadFromFile(data_ov004_022bf100, 0, 0x200, 32);
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  File_UnpackTo(data_027c0280->objVram, data_ov004_022bf0fc);
  Palette_LoadFromFile(data_ov004_022bf100, 0, 0x200, 32);
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
}
