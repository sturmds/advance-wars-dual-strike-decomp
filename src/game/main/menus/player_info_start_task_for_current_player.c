#include "types.h"
#include "game/player_info_task.h"
#include "game/screen_other_map.h"
#include "game/task.h"

extern TaskScriptCmd data_0215d430[];
extern int data_027c0278;

/* Starts the player info screen task under parent for the current player.
 * When data_027c0278 (the screen saved by func_0200a614) is not 0, the
 * player comes from the other screen's state. */
void PlayerInfo_StartTaskForCurrentPlayer(Task* parent) {
  PlayerInfoTask* task =
      (PlayerInfoTask*)Task_CreateChild(data_0215d430, parent);

  if (data_027c0278 == 0) {
    task->player = data_027c027c->unk_48[data_027c0284]->currentPlayer;
    task->unk_66 = 0;
  } else {
    Unk_027c0280* saved0280 = data_027c0280;
    Unk_02080440* saved027c;

    Screen_SetCurrentGfx((&data_027c0288.unk_00)[1 - data_027c0284]);
    saved027c = data_027c027c;
    Screen_SetCurrent((&data_027c0288.unk_00)[1 - data_027c0284]);
    task->player = data_027c027c->unk_48[data_027c0284]->currentPlayer;
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
    task->unk_66 = 1;
  }
  task->unk_68 = 0;
}
