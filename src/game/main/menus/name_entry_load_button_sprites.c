#include "game/screen_other_map.h"
#include "sdk/fs.h"
/* Built without optimization, like the rest of the name entry screen. */
#pragma optimization_level 0

#include "types.h"
#include "game/name_entry_work.h"
#include "game/sprite_anim.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void* File_Load(const char* path);
void File_UnpackTo(void* dest, const char* fileName);
void Palette_LoadFromFile(const char* fileName, int fileOffset,
                          int paletteOffset, int size);
SpriteAnim* SpriteAnim_Create(void* file, int a, int b);
void SpriteAnim_SetAnimation(SpriteAnim* sprite, int frame);

extern char data_0216b338[];
extern char data_0216b348[];
extern char data_0216b34c[];
extern char data_0216b350[];

/* Loads the sprites of the three buttons under the keyboard, each showing
 * its own animation. */
void NameEntry_LoadButtonSprites(NameEntry* entry) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  FS_ChangeDir(data_0216b338);
  entry->buttonSpriteFile = File_Load(data_0216b348);
  File_UnpackTo(data_027c0280->objVram + 0x4000, data_0216b34c);
  Palette_LoadFromFile(data_0216b350, 0, 896, 32);
  entry->buttonSprites[0] = SpriteAnim_Create(entry->buttonSpriteFile, 0, 2);
  entry->buttonSprites[1] = SpriteAnim_Create(entry->buttonSpriteFile, 0, 2);
  entry->buttonSprites[2] = SpriteAnim_Create(entry->buttonSpriteFile, 0, 2);
  SpriteAnim_SetAnimation(entry->buttonSprites[0], 0);
  SpriteAnim_SetAnimation(entry->buttonSprites[1], 1);
  SpriteAnim_SetAnimation(entry->buttonSprites[2], 2);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
