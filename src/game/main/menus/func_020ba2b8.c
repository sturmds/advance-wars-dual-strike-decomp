#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;
extern Unk_02080440_48 data_02183b08;
extern const TaskScriptCmd data_02166888[];

#define STATE (data_027c027c->unk_48[data_027c0284])

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
  if (STATE == &data_02183b08) {
    Task_Create(data_02166888, TASK_LIST(3));
    return TRUE;
  }
  if (STATE->players[STATE->currentPlayer].controller !=
      PLAYER_CONTROLLER_HUMAN) {
    return FALSE;
  }
  Task_Create(data_02166888, TASK_LIST(3));
  return TRUE;
}
