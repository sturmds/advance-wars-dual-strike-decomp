#include "types.h"
#include "game/script_slot.h"
#include "game/play_records.h"
#include "game/song.h"
#include "game/screen.h"

void Battle_ResumeFromSave(void);
void MenuDepth_Push(void);
void func_02017d1c(void);
void MenuDepth_Pop(void);

/* By player 1-4 (index 0 unused): 0xFC, 0xF3, 0xCF, 0x3F. Clears that player's two bits. */
static u8 gForceRankSetKeepMask[] = {
#include "data/main/gForceRankSetKeepMask.inc.c"
};
ScriptCommand data_02158468[] = {
#include "data/main/data_02158468.inc.c"
};

/* Chooses which of the CO's four saved Force Rank sets (0-3) player (1-4) uses. */
void Co_SetForceRankSetIndex(int co, int player, int set) {
  CoSaveRecord* record = &gPlayRecords.coRecords[co];
  u8 sets = gForceRankSetKeepMask[player] & record->forceRankSetByPlayer;

  sets |= set << ((player - 1) * 2);
  record->forceRankSetByPlayer = sets;
}
