#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern u32 data_027c0284;

int UnitMenu_EmergeHidden(void);

/* Returns 1 unless the acting unit is a Sub, then returns UnitMenu_EmergeHidden(). */
int func_020bee00(void) {
  /* A one-case switch, needed to match (an if gives conditional returns). */
  switch (data_027c027c->unk_48[data_027c0284]->actingUnit->type) {
    case UNIT_SUB:
      return UnitMenu_EmergeHidden();
  }
  return 1;
}
