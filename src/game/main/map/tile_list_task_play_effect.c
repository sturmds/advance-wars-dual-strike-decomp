#include "types.h"
#include "game/menu_state.h"
#include "sdk/fx.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_02168140.h"
#include "game/Unk_022a7c5c.h"
#include "game/sprite_anim.h"
#include "sdk/fs.h"

extern char data_021681d8[];
extern char data_021681fc[];
extern char data_02168204[];
extern char data_02168208[];
extern u8 data_021480e0[];
extern u8 data_021488c4[];

void File_UnpackTo(void* dest, const char* fileName);
void Palette_LoadFromFile(const char* fileName, int a, int b, int c);
void Gfx_QueueUnpackedFileCopy(const char* fileName, void* dest);
void Gfx_QueueFileCopy(const char* fileName, void* dest, int size);
SpriteAnim3dTask* SpriteAnim3dTask_StartAlt(const void* file,
                                            const VecFx32* pos, u32 tileBase,
                                            u32 unk28, int animation,
                                            int layer);
/* Defined returning void, but leaves the new task in r0, and this caller uses it. */
void Sound_PlaySe(u32 soundId);

/* Step of the data_02168140 script: loads an animation's graphics and plays
 * it 2 tiles right of the task's tile, with sound 160. On the screen that
 * shows the 3D engine it draws in 3D; on the other the graphics go to a
 * buffer in gMenuState and the sprite takes its tiles from there. What the
 * animation shows is not known; name is a guess. */
void TileListTask_PlayEffect(Unk_02168140Task* task) {
  VecFx32 pos;
  SpriteAnimTask* spriteTask;

  if (data_027c027c == data_027c0288.unk_00) {
    FS_ChangeDir(data_021681d8);
    Gfx_QueueUnpackedFileCopy(data_02168204, data_022a7c5c->unk_68);
    Gfx_QueueFileCopy(data_021681fc, data_022a7c5c->unk_6c, 32);
    pos.x = (task->unk_2c * 16 + 32) * 0x2000 / 256;
    pos.y = (task->unk_30 * 16 + 8) * 0x6000 / 768;
    pos.z = 8;
    SpriteAnim3dTask_StartAlt(data_021488c4, &pos, (u32)data_022a7c5c->unk_68,
                              (u32)data_022a7c5c->unk_6c, 0, 1);
  } else {
    FS_ChangeDir(data_021681d8);
    File_UnpackTo(gMenuState.bytes, data_02168208);
    Palette_LoadFromFile(data_021681fc, 0, 672, 32);
    spriteTask = SpriteAnimTask_Start(
        data_021480e0,
        (task->unk_2c * 16 - MAP_STATE->unk_1c4->scrollX + 32) & 0x1ff,
        (task->unk_30 * 16 - MAP_STATE->unk_1c4->scrollY + 8) & 0xff, 0x5194, 0,
        1);
    spriteTask->sprite->tileSource = gMenuState.bytes;
  }
  Sound_PlaySe(160);
}
