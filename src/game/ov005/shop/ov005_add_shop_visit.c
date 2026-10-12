#include "types.h"
#include "game/play_records.h"

/* Counts one more visit to Hachi's shop, up to 255. */
void Ov005_AddShopVisit(void) {
  u8 visits = gPlayRecords.shopVisits;
  if (visits != 255) {
    int next = visits + 1;
    gPlayRecords.shopVisits = next;
  }
}
