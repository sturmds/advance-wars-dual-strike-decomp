#include "types.h"
#include "game/Unk_02080440.h"
#include "game/result_sprite_task.h"

int Task_JumpToLabel(ResultSpriteTask* task, int label);
void func_02038264(void);

/* Script command: in Versus jumps to label 0; otherwise creates the result
 * point task (func_02038264). */
void ResultSprite_StartPointScript(ResultSpriteTask* task) {
  if (MAP_STATE->gameMode == GAME_MODE_VERSUS) {
    Task_JumpToLabel(task, 0);
  } else {
    func_02038264();
  }
}
