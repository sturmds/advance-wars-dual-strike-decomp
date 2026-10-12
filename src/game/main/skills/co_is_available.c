#include "types.h"
#include "game/play_records.h"

/* Bit 31 of the CO's saved record, set by Co_SetAvailable; FALSE for ids past the last CO. */
BOOL Co_IsAvailable(int co) {
  if (co >= CO_COUNT) {
    return FALSE;
  }
  return gPlayRecords.coRecords[co].available;
}
