#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

extern Unk_02080440_48 data_02183b08;
extern const TaskScriptCmd data_02166888[];

BOOL func_020fa6f8(void);

/* Starts the task at data_02166888 and returns TRUE, unless this is the
 * first screen or func_020fa6f8 answers yes. On the first map it always
 * starts; otherwise only for a human-controlled player. What the task does
 * is unknown. */
BOOL func_020ba2b8(void) {
  if (data_027c027c == data_027c0288.unk_00) {
    return FALSE;
  }
  if (func_020fa6f8()) {
    return FALSE;
  }
  if (MAP_STATE == &data_02183b08) {
    Task_Create(data_02166888, TASK_LIST(3));
    return TRUE;
  }
  if (MAP_STATE->players[MAP_STATE->currentPlayer].controller !=
      PLAYER_CONTROLLER_HUMAN) {
    return FALSE;
  }
  Task_Create(data_02166888, TASK_LIST(3));
  return TRUE;
}
