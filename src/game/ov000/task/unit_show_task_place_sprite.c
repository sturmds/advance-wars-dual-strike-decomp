#include "types.h"
#include "game/task.h"
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
void UnitShowTask_OnDestroy(void);
void func_ov000_022ad7e0(void);
void UnitShowTask_Update(void);
void func_ov000_022ad7dc(void);
void UnitShowTask_StateMove(void);
void func_ov000_022ad7e4(void);
void UnitShowTask_StateScript(void);
void func_ov000_022ad820(void);

char data_ov000_022db568[] = {
#include "data/ov000/data_ov000_022db568.inc.c"
};
char data_ov000_022db564[] = {
#include "data/ov000/data_ov000_022db564.inc.c"
};
char data_ov000_022db560[] = {
#include "data/ov000/data_ov000_022db560.inc.c"
};
char data_ov000_022db694[] = {
#include "data/ov000/data_ov000_022db694.inc.c"
};
char data_ov000_022db690[] = {
#include "data/ov000/data_ov000_022db690.inc.c"
};
char data_ov000_022db68c[] = {
#include "data/ov000/data_ov000_022db68c.inc.c"
};
char data_ov000_022db688[] = {
#include "data/ov000/data_ov000_022db688.inc.c"
};
char data_ov000_022db684[] = {
#include "data/ov000/data_ov000_022db684.inc.c"
};
char data_ov000_022db680[] = {
#include "data/ov000/data_ov000_022db680.inc.c"
};
char data_ov000_022db67c[] = {
#include "data/ov000/data_ov000_022db67c.inc.c"
};
char data_ov000_022db678[] = {
#include "data/ov000/data_ov000_022db678.inc.c"
};
char data_ov000_022db674[] = {
#include "data/ov000/data_ov000_022db674.inc.c"
};
char data_ov000_022db670[] = {
#include "data/ov000/data_ov000_022db670.inc.c"
};
char data_ov000_022db66c[] = {
#include "data/ov000/data_ov000_022db66c.inc.c"
};
char data_ov000_022db668[] = {
#include "data/ov000/data_ov000_022db668.inc.c"
};
char data_ov000_022db664[] = {
#include "data/ov000/data_ov000_022db664.inc.c"
};
char data_ov000_022db660[] = {
#include "data/ov000/data_ov000_022db660.inc.c"
};
char data_ov000_022db65c[] = {
#include "data/ov000/data_ov000_022db65c.inc.c"
};
char data_ov000_022db658[] = {
#include "data/ov000/data_ov000_022db658.inc.c"
};
char data_ov000_022db654[] = {
#include "data/ov000/data_ov000_022db654.inc.c"
};
char data_ov000_022db650[] = {
#include "data/ov000/data_ov000_022db650.inc.c"
};
char data_ov000_022db64c[] = {
#include "data/ov000/data_ov000_022db64c.inc.c"
};
char data_ov000_022db648[] = {
#include "data/ov000/data_ov000_022db648.inc.c"
};
char data_ov000_022db644[] = {
#include "data/ov000/data_ov000_022db644.inc.c"
};
char data_ov000_022db640[] = {
#include "data/ov000/data_ov000_022db640.inc.c"
};
char data_ov000_022db63c[] = {
#include "data/ov000/data_ov000_022db63c.inc.c"
};
char data_ov000_022db638[] = {
#include "data/ov000/data_ov000_022db638.inc.c"
};
char data_ov000_022db634[] = {
#include "data/ov000/data_ov000_022db634.inc.c"
};
char data_ov000_022db630[] = {
#include "data/ov000/data_ov000_022db630.inc.c"
};
char data_ov000_022db62c[] = {
#include "data/ov000/data_ov000_022db62c.inc.c"
};
char data_ov000_022db628[] = {
#include "data/ov000/data_ov000_022db628.inc.c"
};
char data_ov000_022db624[] = {
#include "data/ov000/data_ov000_022db624.inc.c"
};
char data_ov000_022db620[] = {
#include "data/ov000/data_ov000_022db620.inc.c"
};
char data_ov000_022db61c[] = {
#include "data/ov000/data_ov000_022db61c.inc.c"
};
char data_ov000_022db618[] = {
#include "data/ov000/data_ov000_022db618.inc.c"
};
char data_ov000_022db614[] = {
#include "data/ov000/data_ov000_022db614.inc.c"
};
char data_ov000_022db610[] = {
#include "data/ov000/data_ov000_022db610.inc.c"
};
char data_ov000_022db60c[] = {
#include "data/ov000/data_ov000_022db60c.inc.c"
};
char data_ov000_022db608[] = {
#include "data/ov000/data_ov000_022db608.inc.c"
};
char data_ov000_022db604[] = {
#include "data/ov000/data_ov000_022db604.inc.c"
};
char data_ov000_022db600[] = {
#include "data/ov000/data_ov000_022db600.inc.c"
};
char data_ov000_022db5fc[] = {
#include "data/ov000/data_ov000_022db5fc.inc.c"
};
char data_ov000_022db5f8[] = {
#include "data/ov000/data_ov000_022db5f8.inc.c"
};
char data_ov000_022db5f4[] = {
#include "data/ov000/data_ov000_022db5f4.inc.c"
};
char data_ov000_022db5f0[] = {
#include "data/ov000/data_ov000_022db5f0.inc.c"
};
char data_ov000_022db5ec[] = {
#include "data/ov000/data_ov000_022db5ec.inc.c"
};
char data_ov000_022db5e8[] = {
#include "data/ov000/data_ov000_022db5e8.inc.c"
};
char data_ov000_022db5e4[] = {
#include "data/ov000/data_ov000_022db5e4.inc.c"
};
char data_ov000_022db5e0[] = {
#include "data/ov000/data_ov000_022db5e0.inc.c"
};
char data_ov000_022db5dc[] = {
#include "data/ov000/data_ov000_022db5dc.inc.c"
};
char data_ov000_022db5d8[] = {
#include "data/ov000/data_ov000_022db5d8.inc.c"
};
char data_ov000_022db5d4[] = {
#include "data/ov000/data_ov000_022db5d4.inc.c"
};
char data_ov000_022db5d0[] = {
#include "data/ov000/data_ov000_022db5d0.inc.c"
};
char data_ov000_022db5cc[] = {
#include "data/ov000/data_ov000_022db5cc.inc.c"
};
char data_ov000_022db5c8[] = {
#include "data/ov000/data_ov000_022db5c8.inc.c"
};
char data_ov000_022db5c4[] = {
#include "data/ov000/data_ov000_022db5c4.inc.c"
};
char data_ov000_022db5c0[] = {
#include "data/ov000/data_ov000_022db5c0.inc.c"
};
char data_ov000_022db5bc[] = {
#include "data/ov000/data_ov000_022db5bc.inc.c"
};
char data_ov000_022db5b8[] = {
#include "data/ov000/data_ov000_022db5b8.inc.c"
};
char data_ov000_022db5b4[] = {
#include "data/ov000/data_ov000_022db5b4.inc.c"
};
char data_ov000_022db5b0[] = {
#include "data/ov000/data_ov000_022db5b0.inc.c"
};
char data_ov000_022db5ac[] = {
#include "data/ov000/data_ov000_022db5ac.inc.c"
};
char data_ov000_022db5a8[] = {
#include "data/ov000/data_ov000_022db5a8.inc.c"
};
char data_ov000_022db5a4[] = {
#include "data/ov000/data_ov000_022db5a4.inc.c"
};
char data_ov000_022db5a0[] = {
#include "data/ov000/data_ov000_022db5a0.inc.c"
};
char data_ov000_022db59c[] = {
#include "data/ov000/data_ov000_022db59c.inc.c"
};
char data_ov000_022db598[] = {
#include "data/ov000/data_ov000_022db598.inc.c"
};
char data_ov000_022db594[] = {
#include "data/ov000/data_ov000_022db594.inc.c"
};
char data_ov000_022db590[] = {
#include "data/ov000/data_ov000_022db590.inc.c"
};
char data_ov000_022db58c[] = {
#include "data/ov000/data_ov000_022db58c.inc.c"
};
char data_ov000_022db588[] = {
#include "data/ov000/data_ov000_022db588.inc.c"
};
char data_ov000_022db584[] = {
#include "data/ov000/data_ov000_022db584.inc.c"
};
char data_ov000_022db580[] = {
#include "data/ov000/data_ov000_022db580.inc.c"
};
char data_ov000_022db57c[] = {
#include "data/ov000/data_ov000_022db57c.inc.c"
};
char data_ov000_022db578[] = {
#include "data/ov000/data_ov000_022db578.inc.c"
};
char data_ov000_022db574[] = {
#include "data/ov000/data_ov000_022db574.inc.c"
};
char data_ov000_022db570[] = {
#include "data/ov000/data_ov000_022db570.inc.c"
};
char data_ov000_022db56c[] = {
#include "data/ov000/data_ov000_022db56c.inc.c"
};
static Vec3 data_ov000_022db698 = {
#include "data/ov000/data_ov000_022db698.inc.c"
};
s16 data_ov000_022db6a4[] = {
#include "data/ov000/data_ov000_022db6a4.inc.c"
};
/* declared `UnitShowStateHandler data_ov000_022db6b4[]` in unit_show_task_update.c: declared in src/game/ov000/task/unit_show_task_update.c, with a type no header defines */
void* data_ov000_022db6b4[] = {&func_ov000_022ad7e0,      &func_ov000_022ad7dc,
                               &UnitShowTask_StateMove,   &func_ov000_022ad7e4,
                               &UnitShowTask_StateScript, &func_ov000_022ad820};
TaskScriptCmd data_ov000_022db6cc[] = {
#include "data/ov000/data_ov000_022db6cc.inc.c"
};

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
