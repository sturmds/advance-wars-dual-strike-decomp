#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_show_task.h"
#include "game/player.h"
#include "game/play_records.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern int gLinkPlay;

void UnitShowTask_Destroy(UnitShowTask* task);
void UnitAction_Send(int action, int unitIndex, u8 target, u8 arg);
void SpriteAnim_SetAnimation(void* sprite, int frame);
void ZoomSpriteTask_StartHeld(int x, int y);
int func_020800a4(void);
void func_ov000_022ad610(UnitShowTask* task);
void UnitShowTask_PlaySpecialSound(UnitShowTask* task);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Runs the task's script, one signed byte per command, until a command
 * ends the step: -1 plays the sound and destroys the task; 0-3 set that
 * sprite frame (state 2); 4 plays the sound and changes state; 5-8 set frame
 * 0-3 and go on; 9 waits the next byte in frames (state 3); 10 plays the
 * sound, calls ZoomSpriteTask_StartHeld on the tile and, in link play, sends unit action
 * 2 for the unit. The case order and STATE_IDX_FIRST are needed to match. */
void UnitShowTask_RunScript(UnitShowTask* task) {
  s16 cmd;

  while (TRUE) {
    cmd = task->script[task->scriptPos++];
    switch (cmd) {
      case 9:
        task->waitFrames = task->script[task->scriptPos++];
        task->state = UNIT_SHOW_STATE_DELAY;
        return;
      case 10:
        ShowEntry_PlaySound(task);
        task->state = UNIT_SHOW_STATE_DONE;
        ZoomSpriteTask_StartHeld(task->x >> 4, task->y >> 4);
        if (STATE_IDX_FIRST->players[STATE_IDX_FIRST->currentPlayer]
                .controller == PLAYER_CONTROLLER_HUMAN) {
          if (gPlayRecords.unk_3cc < 999999) {
            gPlayRecords.unk_3cc++;
          }
        }
        if (gLinkPlay != 0 ? GAME->unk_18 != 21 : 0) {
          if (func_020800a4() == 0) {
            UnitAction_Send(2, (u8)(task->unit - GAME->units), 0, 0);
          }
        }
        return;
      case 4:
        func_ov000_022ad610(task);
        return;
      case -1:
        ShowEntry_PlaySound(task);
        UnitShowTask_Destroy(task);
        return;
      case 0:
      case 1:
      case 2:
      case 3:
        UnitShowTask_PlaySpecialSound(task);
        if (cmd == task->frame) {
          return;
        }
        SpriteAnim_SetAnimation(task->sprite, cmd);
        task->frame = cmd;
        task->state = UNIT_SHOW_STATE_MOVE;
        return;
      case 5:
      case 6:
      case 7:
      case 8:
        cmd = cmd - 5;
        if (cmd == task->frame) {
          break;
        }
        SpriteAnim_SetAnimation(task->sprite, cmd);
        task->frame = cmd;
        break;
    }
  }
}
