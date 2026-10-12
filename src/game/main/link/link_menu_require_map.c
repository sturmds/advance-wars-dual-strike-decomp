#include "types.h"
#include "game/task.h"

int LinkMenu_PlayerHasMap(int player);
void LinkBanner_CreateWithText(Task* task, u32 textId);
int Task_JumpToLabel(Task* task, int label);

/* Script step. When neither of the two link players has a map, shows text 0x48000003 through
 * LinkBanner_CreateWithText and jumps to label 255. */
void LinkMenu_RequireMap(Task* task) {
  int anyMap = 0;
  int player;

  for (player = 0; player < 2; player++) {
    if (LinkMenu_PlayerHasMap(player) == 1) {
      anyMap = 1;
    }
  }
  if (anyMap != 0) {
    return;
  }
  LinkBanner_CreateWithText(task, 0x48000003);
  Task_JumpToLabel(task, 255);
}
