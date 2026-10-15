#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov005_0236bbac.h"
#include "sdk/fs.h"

void* File_Load(const char* path);
void File_UnpackTo(void* dest, const char* fileName);
void Palette_LoadFromFile(const char* fileName, int fileOffset,
                          int paletteOffset, int size);
void* SpriteAnim_Create(void* file, int a, int b);
void SpriteAnim_SetAnimation(void* sprite, int frame);

extern const char data_ov005_0236b324[]; /* "/rikiishi" */
extern const char data_ov005_0236b33c[]; /* "046" */
extern const char data_ov005_0236b340[]; /* "04c" */
extern const char data_ov005_0236b344[]; /* "045": OBJ tiles */
extern const char data_ov005_0236b348[]; /* "047": OBJ palette */
extern const char data_ov005_0236b34c[]; /* "04b": OBJ tiles */
extern const char data_ov005_0236b350[]; /* "04d": OBJ palette */
extern const char data_ov005_0236b354[]; /* "048": OBJ tiles */

/* Loads two sprite files, then on each screen the OBJ tiles and palettes
 * from /rikiishi, and makes the screen's sprites with their first frames. */
void Ov005SoundRoom_LoadSprites(Unk_ov005_0236bbac* work) {
  work->spriteFileA = File_Load(data_ov005_0236b33c);
  work->spriteFileB = File_Load(data_ov005_0236b340);
  {
    Unk_027c0280* saved0280 = data_027c0280;
    Unk_02080440* saved027c;

    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    FS_ChangeDir(data_ov005_0236b324);
    File_UnpackTo(data_027c0280->objVram + 0x7000, data_ov005_0236b344);
    Palette_LoadFromFile(data_ov005_0236b348, 0, 0x360, 32);
    File_UnpackTo(data_027c0280->objVram + 0x7400, data_ov005_0236b34c);
    Palette_LoadFromFile(data_ov005_0236b350, 0, 0x380, 32);
    work->subObjects[0] = SpriteAnim_Create(work->spriteFileB, 0, 2);
    work->subObjects[1] = SpriteAnim_Create(work->spriteFileA, 1, 2);
    work->subObjects[2] = SpriteAnim_Create(work->spriteFileA, 1, 2);
    work->subObjects[3] = SpriteAnim_Create(work->spriteFileA, 0, 2);
    work->subObjects[4] = SpriteAnim_Create(work->spriteFileA, 0, 2);
    SpriteAnim_SetAnimation(work->subObjects[0], 0);
    SpriteAnim_SetAnimation(work->subObjects[1], 0);
    SpriteAnim_SetAnimation(work->subObjects[2], 0);
    SpriteAnim_SetAnimation(work->subObjects[3], 2);
    SpriteAnim_SetAnimation(work->subObjects[4], 2);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  }
  {
    Unk_027c0280* saved0280 = data_027c0280;
    Unk_02080440* saved027c;

    Screen_SetCurrentGfx(data_027c0288.unk_00);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    FS_ChangeDir(data_ov005_0236b324);
    File_UnpackTo(data_027c0280->objVram + 0x7000, data_ov005_0236b344);
    Palette_LoadFromFile(data_ov005_0236b348, 0, 0x360, 32);
    File_UnpackTo(data_027c0280->objVram + 0x4000, data_ov005_0236b354);
    Palette_LoadFromFile(data_ov005_0236b348, 0, 0x2c0, 32);
    work->mainObjects[0] = SpriteAnim_Create(work->spriteFileA, 1, 2);
    work->mainObjects[1] = SpriteAnim_Create(work->spriteFileA, 1, 2);
    work->mainObjects[2] = SpriteAnim_Create(work->spriteFileA, 0, 2);
    work->mainObjects[3] = SpriteAnim_Create(work->spriteFileA, 0, 2);
    SpriteAnim_SetAnimation(work->mainObjects[0], 1);
    SpriteAnim_SetAnimation(work->mainObjects[1], 1);
    SpriteAnim_SetAnimation(work->mainObjects[2], 2);
    SpriteAnim_SetAnimation(work->mainObjects[3], 2);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  }
}
