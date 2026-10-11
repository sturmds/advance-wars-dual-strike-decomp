#include "types.h"

void Unit_PlayDiveEffect(int unitType, BOOL emerge);

/* Sub diving (or Stealth hiding) at the acting unit's destination. */
void Unit_PlayHideEffect(int unitType)
{
    Unit_PlayDiveEffect(unitType, FALSE);
}
