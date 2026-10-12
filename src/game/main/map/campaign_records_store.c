#include "types.h"
#include "game/Unk_02290118.h"

/* Needed to match: the original reads the id through data_02290118 and adds
 * the offset of the records (unk_8c) at run time, but writes through
 * gCampaignMapRecords. */
#define SAVED_RECORD(i) \
  (*(MapRecord*)((u8*)&data_02290118 + (i) * sizeof(MapRecord) + 0x8c))

/* Stores days and score for a map: overwrites the record of the same map
 * index or takes the first free slot (nothing when all 28 are used). A new
 * entry is followed by a cleared one that ends the list. */
void CampaignRecords_Store(int mapIndex, int days, int score) {
  u8 i;
  BOOL found = FALSE;

  for (i = 0; i < 28 && gCampaignMapRecords[i].days != 0; i++) {
    if (mapIndex == SAVED_RECORD(i).mapIndex) {
      found = TRUE;
      break;
    }
  }
  if (i >= 28) {
    return;
  }
  gCampaignMapRecords[i].mapIndex = mapIndex;
  gCampaignMapRecords[i].days = days;
  gCampaignMapRecords[i].score = score;
  if (i >= 27) {
    return;
  }
  if (!found) {
    gCampaignMapRecords[i + 1].days = 0;
  }
}
