#include "types.h"
#include "game/Unk_02290118.h"

/* Number of stored campaign map records plus 1: the 1-based slot the next
 * record would take (29 when the list is full). */
int CampaignRecords_GetNextNumber(void) {
  u8 i;

  for (i = 0; i < 28 && gCampaignMapRecords[i].days != 0; i++) {
  }
  return i + 1;
}
