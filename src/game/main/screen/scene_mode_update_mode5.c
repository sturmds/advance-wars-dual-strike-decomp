#include "types.h"
#include "game/task.h"
#include "game/player.h"
#include "game/screen_other_map.h"
#include "game/Unk_021694c0.h"
#include "game/screen.h"

extern Unk_021694c0* data_021694c0;
extern Unk_02080440_48 data_02189cd8;

BOOL Map_IsThisMapsTurn(void);
void Task_ResumeScript(Task* task);

#define SWITCH_TO(state)       \
  saved0280 = data_027c0280;   \
  Screen_SetCurrentGfx(state); \
  saved027c = data_027c027c;   \
  Screen_SetCurrent(state)

#define SWITCH_BACK()           \
  Screen_SetCurrent(saved027c); \
  Screen_SetCurrentGfx(saved0280)

/* Task update of the scene mode script. In mode 5, requests mode 4 when the
 * first screen's map does not play now (Map_IsThisMapsTurn) or when the
 * second map (data_02189cd8) is on a non-human player's turn, but not when
 * the other map's unk_30 is set. Resumes the script once a mode change is
 * pending. */
void SceneMode_UpdateMode5(Task* task) {
  BOOL requestMode4 = FALSE;
  Unk_027c0280* outer0280;
  Unk_02080440* outer027c;
  u32 otherUnk30;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  if (data_021694c0->mode == 5) {
    SWITCH_TO(data_027c0288.unk_00);
    if (!Map_IsThisMapsTurn()) {
      requestMode4 = TRUE;
    }
    SWITCH_BACK();

    SWITCH_TO(data_027c0288.unk_00);
    if (MAP_STATE == &data_02189cd8 &&
        MAP_STATE->players[MAP_STATE->currentPlayer].controller !=
            PLAYER_CONTROLLER_HUMAN) {
      requestMode4 = TRUE;
    }
    SWITCH_BACK();

    outer0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    outer027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    if (MAP_STATE == &data_02189cd8) {
      SWITCH_TO((&data_027c0288.unk_00)[1 - data_027c0284]);
      otherUnk30 = MAP_STATE->unk_1c4->unk_30;
      SWITCH_BACK();
      if (otherUnk30 != 0) {
        requestMode4 = FALSE;
      }
    }
    Screen_SetCurrent(outer027c);
    Screen_SetCurrentGfx(outer0280);

    if (requestMode4 == TRUE && !Screens_IsTransitionRunning()) {
      SceneMode_Request(4);
    }
  }
  if (data_021694c0->mode != data_021694c0->requestedMode) {
    Task_ResumeScript(task);
  }
}
