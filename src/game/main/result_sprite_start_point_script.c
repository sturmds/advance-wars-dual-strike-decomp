#include "types.h"
#include "game/Unk_02080440.h"
#include "game/result_sprite_task.h"

int Task_JumpToLabel(ResultSpriteTask* task, int label);
void func_02038264(void);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Script command: in Versus jumps to label 0; otherwise creates the result
 * point task (func_02038264). */
void ResultSprite_StartPointScript(ResultSpriteTask* task) {
  if (STATE->gameMode == GAME_MODE_VERSUS) {
    Task_JumpToLabel(task, 0);
  } else {
    func_02038264();
  }
}
