#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* TRUE when the battle has no second map and unk_59 is 0 or 1. The War
 * Room results screen then lays out one column of CO rows and gives only
 * the front CO experience (CoExp_InitScreen, CoExp_TallyStep). The name is
 * a guess from those callers; why it keys on unk_59 is not known. */
int CoExp_IsSingleColumn(void) {
  if (STATE->secondMapId != 0) {
    return FALSE;
  }
  return STATE->unk_59 <= 1;
}
