#include "types.h"
#include "game/co_data.h"
#include "game/player.h"
#include "game/task.h"
#include "game/Unk_02080440.h"

void func_02055cac(Task* parent);

extern const TaskScriptCmd data_0215c0f0[];
extern const TaskScriptCmd data_0215c198[];

/* Needed to match: the tag power test is turned into a BOOL twice, once by
 * the flag and once by the ternary in TagPowerOn. */
static inline BOOL IsTagPowerOn(void) {
  BOOL tagPowerOn = FALSE;

  if (MAP_STATE->players[MAP_STATE->currentPlayer].back.id != 0) {
    BOOL isTagHalf =
        MAP_STATE->players[MAP_STATE->currentPlayer].tagPowerHalf ==
            TAG_POWER_FRONT ||
        MAP_STATE->players[MAP_STATE->currentPlayer].tagPowerHalf ==
            TAG_POWER_BACK;

    if (isTagHalf) {
      tagPowerOn = TRUE;
    }
  }
  return tagPowerOn;
}

static inline BOOL TagPowerOn(void) { return IsTagPowerOn() ? TRUE : FALSE; }

/* Starts the screen shown when the current player's power goes off, as a
 * child of parent: the CO Power screen for POWER_CO; for POWER_SUPER the
 * tag power screen while a tag power is on (func_02055cac), else the Super CO
 * Power break screen. */
void PowerScreen_Start(Task* parent, int powerState) {
  switch (powerState) {
    case POWER_CO:
      Task_CreateChild(data_0215c0f0, parent);
      break;
    case POWER_SUPER:
      if (TagPowerOn() == TRUE) {
        func_02055cac(parent);
      } else {
        Task_CreateChild(data_0215c198, parent);
      }
      break;
  }
}
