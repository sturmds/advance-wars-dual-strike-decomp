#ifndef GAME_UNK_02169B20_H
#define GAME_UNK_02169B20_H

#include "types.h"
#include "game/task.h"

/* One entry of data_02169b20 per data_021694c0->mode. func_020f379c
 * starts script and func_020f37d0 unk_04 (through data_02169b24, the
 * table at +4) in task list 3. */
typedef struct Unk_02169b20 {
  const TaskScriptCmd* script;
  const TaskScriptCmd* unk_04;
  const char* name; /* the mode's name; no reader found */
} Unk_02169b20;

typedef char Unk_02169b20_size_check[sizeof(Unk_02169b20) == 0xc ? 1 : -1];

extern Unk_02169b20 data_02169b20[];

#endif
