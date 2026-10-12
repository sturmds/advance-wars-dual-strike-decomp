#include "types.h"
#include "game/player_info_task.h"
#include "game/task.h"

extern TaskScriptCmd data_0215d430[];

/* Starts the player info screen task under parent for the given player. */
void PlayerInfo_StartTaskFor(Task* parent, int player, int unk_66) {
  PlayerInfoTask* task =
      (PlayerInfoTask*)Task_CreateChild(data_0215d430, parent);

  task->player = player;
  task->unk_66 = unk_66;
  task->unk_68 = 1;
}
