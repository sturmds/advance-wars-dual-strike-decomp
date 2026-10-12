#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern u32 data_027c0284;

int UnitMenu_EmergeHidden(void);

/* Returns 1 when the acting unit is not a Stealth, else UnitMenu_EmergeHidden(). */
int func_020beda8(void) {
  /* A one-case switch, needed to match (an if gives conditional returns). */
  switch (data_027c027c->unk_48[data_027c0284]->actingUnit->type) {
    case UNIT_STEALTH:
      return UnitMenu_EmergeHidden();
  }
  return 1;
}
