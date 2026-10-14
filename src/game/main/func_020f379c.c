#include "types.h"
#include "game/task.h"
#include "game/Unk_02169b20.h"
#include "game/Unk_021694c0.h"

extern Unk_021694c0* data_021694c0;

/* Starts the task script of entry unk_08 of data_02169b20 in list 3. */
void func_020f379c(void) {
  Task_Create(data_02169b20[data_021694c0->mode].script, TASK_LIST(3));
}
