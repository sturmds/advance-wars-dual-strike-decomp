#include "types.h"
#include "game/play_records.h"

/* By player 1-4 (index 0 unused): 0xFC, 0xF3, 0xCF, 0x3F. Clears that player's two bits. */
extern u8 gForceRankSetKeepMask[];

/* Chooses which of the CO's four saved Force Rank sets (0-3) player (1-4) uses. */
void Co_SetForceRankSetIndex(int co, int player, int set) {
  CoSaveRecord* record = &gPlayRecords.coRecords[co];
  u8 sets = gForceRankSetKeepMask[player] & record->forceRankSetByPlayer;

  sets |= set << ((player - 1) * 2);
  record->forceRankSetByPlayer = sets;
}
