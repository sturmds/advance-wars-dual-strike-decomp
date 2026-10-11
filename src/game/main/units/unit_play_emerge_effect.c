#include "types.h"

void Unit_PlayDiveEffect(int unitType, BOOL emerge);

/* Sub emerging (or Stealth reappearing) at the acting unit's destination. */
void Unit_PlayEmergeEffect(int unitType)
{
    Unit_PlayDiveEffect(unitType, TRUE);
}
