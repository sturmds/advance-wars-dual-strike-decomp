#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov004_022bf158.h"

void FS_ChangeDir(const char* dir);
void File_UnpackTo(void* dest, const char* fileName);
void File_ReadTo(void* dest, const char* fileName);
void Palette_LoadFromFile(const char* fileName, int fileOffset,
                          int paletteOffset, int size);

extern char data_ov004_022bf0dc[]; /* directory */
/* layer 2 map */
char data_ov004_022bf114[] = {
#include "data/ov004/data_ov004_022bf114.inc.c"
};
/* layer 2 tiles */
char data_ov004_022bf110[] = {
#include "data/ov004/data_ov004_022bf110.inc.c"
};
/* layer 1 palette */
char data_ov004_022bf10c[] = {
#include "data/ov004/data_ov004_022bf10c.inc.c"
};
/* layer 1 map */
char data_ov004_022bf108[] = {
#include "data/ov004/data_ov004_022bf108.inc.c"
};
/* layer 1 tiles */
char data_ov004_022bf104[] = {
#include "data/ov004/data_ov004_022bf104.inc.c"
};
/* sprite palette */
char data_ov004_022bf100[] = {
#include "data/ov004/data_ov004_022bf100.inc.c"
};
/* sprite tiles */
static char data_ov004_022bf0fc[] = {
#include "data/ov004/data_ov004_022bf0fc.inc.c"
};
/* sprite data read by Ov004Task_InitSprite */
static char data_ov004_022bf0f8[] = {
#include "data/ov004/data_ov004_022bf0f8.inc.c"
};
/* layer 2 palette */
char data_ov004_022bf118[] = {
#include "data/ov004/data_ov004_022bf118.inc.c"
};

/* Loads the sprite data into the state's buffer, then the same sprite tiles
 * and palette into OBJ VRAM of both screens. */
void Ov004_LoadSpriteGraphicsBothScreens(Unk_ov004_022bf158* state) {
  u8* buffer;
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;

  FS_ChangeDir(data_ov004_022bf0dc);
  buffer = (u8*)state->buffer + OV004_SPRITE_GRAPHICS_OFFSET;
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
