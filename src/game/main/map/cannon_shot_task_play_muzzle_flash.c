#include "types.h"
#include "sdk/fx.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_022a7c5c.h"
#include "game/cannon_shot_task.h"
#include "game/map_object.h"
#include "game/sprite_anim.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern char data_021681d8[];
extern char data_0216820c[];

void FS_ChangeDir(const char* dir);
void File_UnpackTo(void* dest, const char* fileName);
void Palette_LoadFromFile(const char* fileName, int a, int b, int c);
void Gfx_QueueUnpackedFileCopy(const char* fileName, void* dest);
void Gfx_QueueFileCopy(const char* fileName, void* dest, int size);
SpriteAnim3dTask* SpriteAnim3dTask_StartAlt(const void* file,
                                            const VecFx32* pos, u32 tileBase,
                                            u32 unk28, int animation,
                                            int layer);
SpriteAnim3dTask* SpriteAnim3dTask_StartAltFlagged(const void* file,
                                                   const VecFx32* pos,
                                                   u32 tileBase, u32 unk28,
                                                   int animation, int layer);
void Sound_PlaySe(u32 soundId);
void CannonShot_GetMuzzleOffset(int* x, int* y, int kind);
char* CannonShot_Get3dGraphicsFile(int kind);
char* CannonShot_Get2dGraphicsFile(int kind);
u8* CannonShot_GetAnimFile(int kind);
int CannonShot_GetFacingAnim(u32 facing);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Step of the cannon shot script: loads the shot graphics for the cannon
 * kind and plays the muzzle flash at the cannon (3D or sprite, as the
 * screen needs), with sound 170. */
void CannonShotTask_PlayMuzzleFlash(CannonShotTask* task) {
  int x;
  int y;
  VecFx32 pos;

  x = 0;
  y = 0;
  CannonShot_GetMuzzleOffset(&x, &y, task->cannonKind);
  if (data_027c027c == data_027c0288.unk_00) {
    FS_ChangeDir(data_021681d8);
    Gfx_QueueUnpackedFileCopy(CannonShot_Get3dGraphicsFile(task->cannonKind),
                              data_022a7c5c->unk_68);
    Gfx_QueueFileCopy(data_0216820c, data_022a7c5c->unk_6c, 64);
    pos.x = (x + task->cannonX * 16) * 0x2000 / 256;
    pos.y = (y + task->cannonY * 16) * 0x6000 / 768;
    pos.z = 8;
    if (task->cannonKind == MAP_OBJECT_MINICANNON) {
      SpriteAnim3dTask_StartAltFlagged(
          CannonShot_GetAnimFile(task->cannonKind), &pos,
          (u32)data_022a7c5c->unk_68, (u32)data_022a7c5c->unk_6c,
          CannonShot_GetFacingAnim(task->facing), 1);
    } else {
      SpriteAnim3dTask_StartAlt(CannonShot_GetAnimFile(task->cannonKind), &pos,
                                (u32)data_022a7c5c->unk_68,
                                (u32)data_022a7c5c->unk_6c,
                                CannonShot_GetFacingAnim(task->facing), 1);
    }
  } else {
    FS_ChangeDir(data_021681d8);
    File_UnpackTo(data_027c0280->objVram + 0x3280,
                  CannonShot_Get2dGraphicsFile(task->cannonKind));
    Palette_LoadFromFile(data_0216820c, 0, 608, 64);
    x = x + task->cannonX * 16 - STATE->unk_1c4->scrollX;
    y = y + task->cannonY * 16 - STATE->unk_1c4->scrollY;
    SpriteAnimTask_Start(CannonShot_GetAnimFile(task->cannonKind), x, y, 0x3194,
                         CannonShot_GetFacingAnim(task->facing), 1);
  }
  Sound_PlaySe(170);
}
