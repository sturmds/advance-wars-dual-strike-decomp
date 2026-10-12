#include "types.h"
#include "game/play_records.h"

/* Writes bit 31 of the CO's saved record (CoSaveRecord.available). Ov005 shows a
 * button for a CO in its list only while the bit is set, hence the name; a guess. */
void Co_SetAvailable(int co, int value) {
  if (co >= CO_COUNT) {
    return;
  }
  gPlayRecords.coRecords[co].available = value;
}
