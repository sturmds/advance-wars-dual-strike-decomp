#include "types.h"
#include "game/task.h"
#include "game/menu_state.h"
#include "game/BattleSpriteTask.h"
#include "game/keypad.h"
#include "game/touch.h"
#include "game/Unk_0229fb30.h"

extern MenuState gMenuState;

BOOL Link_IsActive(void);
void Battle_EnsureTask(void);
void Battle_ReleaseSideTasks(void);
void BattleCellSprites_Move(void);
void Battle_UpdateFx(void);
void BattleCells_RunImpactList(Task* task);
void BattleCells_RunStepList(Task* task);
void BattleCells_RunSequence(Task* task);
void BattleCells_PushStep2(void);
void BattleCells_PushStep(void);
void Battle_UpdateHud(void);

/* Needed to match: the counter is read through data_0229fb30 and written through gMenuState. */
#define MENU ((MenuState*)&gMenuState)

/* Per-frame task of the battle screen: runs the two events of the attacker's HP drain
 * profile when the task's frame (waitFrames) reaches them, steps the screen parts, and
 * calls Battle_EnsureTask on A or a touch (what that does is not known yet). */
void Battle_TaskUpdate(Task* task) {
  const u16* profile;

  profile = MENU->battle.drainProfile[0];
  if (task->waitFrames == profile[0]) {
    BattleCells_PushStep();
  }
  if (task->waitFrames == ((const u16*)MENU->battle.drainProfile[0])[1]) {
    BattleCells_PushStep2();
  }
  BattleCells_RunSequence(task);
  BattleCells_RunStepList(task);
  BattleCells_RunImpactList(task);
  if (data_0229fb30.unk_10 != 0xffff) {
    MENU->battle.frames++;
  }
  task->waitFrames = data_0229fb30.unk_10;
  Battle_UpdateFx();
  BattleCellSprites_Move();
  Battle_ReleaseSideTasks();
  Battle_UpdateHud();
  if ((gKeypad->pressed & PAD_BUTTON_A) ||
      (gTouch.phase == TOUCH_PRESSED && gTouch.active == 1)) {
    if (Link_IsActive() == 0) {
      Battle_EnsureTask();
    }
  }
}
