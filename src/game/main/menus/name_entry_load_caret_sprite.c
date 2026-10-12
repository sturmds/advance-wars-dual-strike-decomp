/* Built without optimization, like the rest of the name entry screen. */
#pragma optimization_level 0

#include "types.h"
#include "game/name_entry_work.h"
#include "game/sprite_anim.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void FS_ChangeDir(const char* dir);
void* File_Load(const char* path);
void File_UnpackTo(void* dest, const char* fileName);
void Palette_LoadFromFile(const char* fileName, int fileOffset,
                          int paletteOffset, int size);
SpriteAnim* SpriteAnim_Create(void* file, int a, int b);
void SpriteAnim_SetAnimation(SpriteAnim* sprite, int frame);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern char data_0216b338[];
extern char data_0216b354[];
extern char data_0216b358[];
extern char data_0216b35c[];

void NameEntry_LoadCaretSprite(NameEntry* entry) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  FS_ChangeDir(data_0216b338);
  entry->spriteFile = File_Load(data_0216b354);
  File_UnpackTo(data_027c0280->objVram + 0x7000, data_0216b358);
  Palette_LoadFromFile(data_0216b35c, 0, 864, 32);
  entry->caret = SpriteAnim_Create(entry->spriteFile, 0, 2);
  SpriteAnim_SetAnimation(entry->caret, 0);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
