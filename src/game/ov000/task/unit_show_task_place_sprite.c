#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_show_task.h"

typedef struct Vec3 {
  int x;
  int y;
  int z;
} Vec3;

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern void* data_027c0288;
extern Vec3 data_ov000_022db698;

void SpriteAnim_Draw(void* sprite, int x, int y);
void func_020224d4(void);
void SpriteAnim_Draw3d(void* sprite, Vec3* pos, Vec3* rot);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Puts the task's sprite on its tile. The first screen draws it in 3D (the
 * x and y use different scales); the other screen draws
 * a 2D sprite relative to the map scroll. */
void UnitShowTask_PlaceSprite(UnitShowTask* task) {
  Vec3 pos;
  Vec3 rot;

  if ((void*)data_027c027c == data_027c0288) {
    rot = data_ov000_022db698;
    *(volatile int*)0x04000444 = 0;
    pos.x = ((task->x + 8) << 13) / 256;
    pos.y = (task->y + 16) * 0x6000 / 0x300;
    pos.z = 0;
    func_020224d4();
    SpriteAnim_Draw3d(task->sprite, &pos, &rot);
    *(volatile int*)0x04000448 = 1;
  } else {
    SpriteAnim_Draw(task->sprite, task->x - GAME->unk_1c4->scrollX + 8,
                    task->y - GAME->unk_1c4->scrollY + 8);
  }
}
