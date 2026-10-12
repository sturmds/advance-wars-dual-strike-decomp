#include "types.h"
#include "game/Unk_0201d320.h"

extern Unk_0201d320 data_022977f4[];

/* Steps the script cursor to the next 16-byte record. */
BOOL func_0201b788(s16 index) {
  data_022977f4[index].unk_00++;
  return 1;
}
