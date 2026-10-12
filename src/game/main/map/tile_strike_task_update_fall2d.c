#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/tile_strike_task.h"
#include "game/sprite_anim.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u8 data_02148eec[];

void Task_ResumeScript(TileStrikeTask* task);
void Sound_PlaySe(int songId);
void ScreenShake_Start(int tableIndex, int steps, void* parent);
void SpriteAnim_SetAnimation(SpriteAnim* sprite, int animation);
void SpriteAnim_Free(SpriteAnim* sprite);
SpriteAnim* SpriteAnim_Create(void* file, int a, int b);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* The sprite falls 10 pixels a frame. Once it is below the tile it is
 * replaced by the impact animation, with a sound and a screen shake. */
void TileStrikeTask_UpdateFall2d(TileStrikeTask* task) {
  SpriteAnim_Draw(task->sprite, (task->x - GAME->unk_1c4->scrollX) & 0x1ff,
                  (task->y - GAME->unk_1c4->scrollY) & 0xff);
  task->y += 10;
  if (task->tileY * 16 + 16 >= task->y) {
    return;
  }
  SpriteAnim_Free(task->sprite);
  task->y = task->tileY * 16 + 8;
  task->sprite = SpriteAnim_Create(data_02148eec, 1, 2);
  task->sprite->tileBase = 0x5194;
  SpriteAnim_SetAnimation(task->sprite, 1);
  Sound_PlaySe(159);
  ScreenShake_Start(0, 8, NULL);
  Task_ResumeScript(task);
}
