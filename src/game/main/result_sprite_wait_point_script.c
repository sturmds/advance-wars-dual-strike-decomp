#include "types.h"
#include "game/Unk_02080440.h"
#include "game/menu_button.h"
#include "game/result_sprite_task.h"

int func_02038164(void);
void Task_ResumeScript(ResultSpriteTask* task);
int Task_JumpToLabel(ResultSpriteTask* task, int label);
MenuButton* func_02032a9c(void);

/* Script step: waits until func_02038164 (the point task's unk_a4) reads 0,
 * then resumes the script. In Survival it adds the button of func_02032a9c
 * and jumps to label 255 instead. */
void ResultSprite_WaitPointScript(ResultSpriteTask* task) {
  if (func_02038164() != 0) {
    return;
  }
  if (MAP_STATE->gameMode == GAME_MODE_SURVIVAL) {
    func_02032a9c();
    Task_JumpToLabel(task, 255);
    return;
  }
  Task_ResumeScript(task);
}
