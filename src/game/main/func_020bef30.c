#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int UnitMenu_DiveHidden(void);

/* Returns 1 when the acting unit is not a Stealth, else UnitMenu_DiveHidden(). */
int func_020bef30(void) {
  /* A one-case switch, needed to match (an if gives conditional returns). */
  switch (data_027c027c->unk_48[data_027c0284]->actingUnit->type) {
    case UNIT_SUB:
      return UnitMenu_DiveHidden();
  }
  return 1;
}
