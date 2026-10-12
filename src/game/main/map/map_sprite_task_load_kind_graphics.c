#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_022a7c5c.h"
#include "game/map_sprite_task.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern char data_02169180[]; /* "/bmap" */

/* Files in /bmap: 3D graphics, 2D graphics and palette per effect. */
extern char data_021691c8[]; /* "04d" */
extern char data_021691cc[]; /* "04e" */
extern char data_021691d0[]; /* "04f" */
extern char data_021691d4[]; /* "050" */
extern char data_021691d8[]; /* "051" */
extern char data_021691dc[]; /* "052" */
extern char data_021691e0[]; /* "053" */
extern char data_021691e4[]; /* "054" */
extern char data_021691e8[]; /* "055" */
extern char data_021691ec[]; /* "056" */
extern char data_021691f0[]; /* "057" */
extern char data_021691f4[]; /* "061" */
extern char data_021691f8[]; /* "062" */
extern char data_021691fc[]; /* "063" */
extern char data_02169200[]; /* "058" */
extern char data_02169204[]; /* "059" */
extern char data_02169208[]; /* "05a" */
extern char data_0216920c[]; /* "05b" */
extern char data_02169210[]; /* "05c" */
extern char data_02169214[]; /* "05d" */
extern char data_02169218[]; /* "044" */
extern char data_0216921c[]; /* "045" */
extern char data_02169220[]; /* "046" */
extern char data_02169224[]; /* "047" */
extern char data_02169228[]; /* "048" */
extern char data_0216922c[]; /* "049" */
extern char data_02169230[]; /* "04a" */
extern char data_02169234[]; /* "04b" */
extern char data_02169238[]; /* "04c" */

/* Sprite animation data per effect. */
extern u8 data_0213c220[];
extern u8 data_0213c42c[];
extern u8 data_0213c734[];
extern u8 data_0213d54c[];
extern u8 data_0213d6ac[];
extern u8 data_0213d81c[];
extern u8 data_0213d8b8[];
extern u8 data_0213d948[];
extern u8 data_0213d9c0[];

void Sound_PlaySe(u32 soundId);
void* SpriteAnim_Find(const void* file);
SpriteAnim* SpriteAnim_Create(void* file, int layer, int flags);
void FS_ChangeDir(const char* dir);
void File_UnpackTo(void* dest, const char* fileName);
void Palette_LoadFromFile(const char* fileName, int fileOffset,
                          int paletteOffset, int size);
void Gfx_QueueUnpackedFileCopy(const char* fileName, void* dest);
void Gfx_QueueFileCopy(const char* fileName, void* dest, int size);
s32 HwDiv32(s32 numer, s32 denom);

/* First step of a map sprite task: picks the effect graphics for the task's
 * kind (kinds 29 and 30 also play a sound; kinds 31-35 play sound 0x84, or
 * use the 04b/04c set in the Design Room), then makes the sprite. On the
 * screen that shows the 3D engine the graphics and palette go to the
 * buffers of data_022a7c5c; on the other, to sprite tile 404 and palette 5.
 * Leaves task->sprite NULL when no sprite slot is free. */
void MapSpriteTask_LoadKindGraphics(MapSpriteTask* task) {
  /* declaration order needed to match */
  char* file2d;
  char* palette;
  u8* anim;
  char* file3d;

  switch (task->kind) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 9:
    case 10:
    case 27:
    default:
      file3d = data_021691c8;
      file2d = data_021691cc;
      palette = data_021691d0;
      anim = data_0213d81c;
      break;
    case 8:
    case 11:
    case 28:
      file3d = data_021691d4;
      file2d = data_021691d8;
      palette = data_021691d0;
      anim = data_0213c220;
      break;
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
      file3d = data_021691dc;
      file2d = data_021691e0;
      palette = data_021691e4;
      anim = data_0213d8b8;
      break;
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
      file3d = data_021691e8;
      file2d = data_021691ec;
      palette = data_021691f0;
      anim = data_0213d948;
      break;
    case 25:
    case 26:
      file3d = data_021691f4;
      file2d = data_021691f8;
      palette = data_021691fc;
      anim = data_0213d9c0;
      break;
    case 30:
      Sound_PlaySe(0xb1);
      file3d = data_02169200;
      file2d = data_02169204;
      palette = data_02169208;
      anim = data_0213c734;
      break;
    case 29:
      Sound_PlaySe(0xb3);
      file3d = data_0216920c;
      file2d = data_02169210;
      palette = data_02169214;
      anim = data_0213c42c;
      break;
    case 24:
    case 31:
      file3d = data_02169218;
      file2d = data_0216921c;
      palette = data_02169220;
      anim = data_0213d6ac;
      break;
    case 32:
      file3d = data_02169218;
      file2d = data_0216921c;
      palette = data_02169224;
      anim = data_0213d6ac;
      break;
    case 33:
      file3d = data_02169218;
      file2d = data_0216921c;
      palette = data_02169228;
      anim = data_0213d6ac;
      break;
    case 34:
      file3d = data_02169218;
      file2d = data_0216921c;
      palette = data_0216922c;
      anim = data_0213d6ac;
      break;
    case 35:
      file3d = data_02169218;
      file2d = data_0216921c;
      palette = data_02169230;
      anim = data_0213d6ac;
      break;
  }

  switch (task->kind) {
    case 31:
    case 32:
    case 33:
    case 34:
    case 35:
      if (data_027c027c->unk_48[data_027c0284]->gameMode ==
          GAME_MODE_DESIGN_ROOM) {
        file3d = data_02169234;
        file2d = data_02169238;
        anim = data_0213d54c;
      } else {
        Sound_PlaySe(0x84);
      }
      break;
  }

  if (SpriteAnim_Find(NULL) == NULL) {
    task->sprite = NULL;
    return;
  }

  if (data_027c027c == data_027c0288.unk_00) {
    if (task->kind == 29) {
      task->sprite = SpriteAnim_Create(anim, 1, 30);
    } else {
      task->sprite = SpriteAnim_Create(anim, 1, 22);
    }
    FS_ChangeDir(data_02169180);
    task->sprite->tileBase = (u32)data_022a7c5c->unk_68;
    task->sprite->unk_28 = (u32)data_022a7c5c->unk_6c;
    Gfx_QueueUnpackedFileCopy(file3d, data_022a7c5c->unk_68);
    Gfx_QueueFileCopy(palette, data_022a7c5c->unk_6c, 32);
  } else {
    task->sprite = SpriteAnim_Create(anim, 1, 2);
    task->sprite->tileBase =
        (5 << 12) | 404; /* sprite attribute 2: palette 5, tile 404 */
    FS_ChangeDir(data_02169180);
    File_UnpackTo(data_027c0280->objVram + 404 * 32, file2d);
    Palette_LoadFromFile(palette, 0, (5 + 16) << 5, 32);
  }

  switch (task->kind) {
    case 25:
    case 26:
      task->sprite->framesLeft = 0;
      task->sprite->speed = HwDiv32(0x7800, 100);
      break;
  }
}
