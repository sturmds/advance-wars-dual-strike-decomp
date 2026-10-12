#include "types.h"
#include "game/task.h"
#include "game/unlocks.h"

#define FRAME(task) (*(s16*)&(task)->unk_4c)

/* Game flag 63: set once every medal is earned, so the banner shows once. */
#define GAME_FLAG_ALL_MEDALS_SHOWN 63

void Task_ResumeScript(Task* task);
int Task_JumpToLabel(Task* task, int label);
void TabList_CountMedals(u16* earned, u16* total);

/* Script task: runs the banner commands that follow once, when every medal is
 * earned and the flag is still clear. Otherwise it jumps to label 0. */
void HistoryScript_CheckAllMedals(Task* task) {
  u16 earned;
  u16 total;

  if (!GameFlag_IsSet(GAME_FLAG_ALL_MEDALS_SHOWN)) {
    TabList_CountMedals(&earned, &total);
    if (earned == total) {
      GameFlag_Set(GAME_FLAG_ALL_MEDALS_SHOWN, 1);
      FRAME(task) = 0;
      Task_ResumeScript(task);
      return;
    }
  }
  Task_JumpToLabel(task, 0);
}
