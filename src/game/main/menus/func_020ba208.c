#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

extern Unk_02080440_48 data_02183b08;
extern const TaskScriptCmd data_02166828[];

/* Starts the task at data_02166828 and returns TRUE, unless this is the
 * second screen, the first map, or a human-controlled player's turn. What the
 * task does is unknown. */
BOOL func_020ba208(void) {
  if (data_027c027c == data_027c0288.unk_04) {
    return FALSE;
  }
  if (MAP_STATE == &data_02183b08) {
    return FALSE;
  }
  if (MAP_STATE->players[MAP_STATE->currentPlayer].controller ==
      PLAYER_CONTROLLER_HUMAN) {
    return FALSE;
  }
  Task_Create(data_02166828, TASK_LIST(3));
  return TRUE;
}
