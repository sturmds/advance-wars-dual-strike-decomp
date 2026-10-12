#include "types.h"
#include "game/player.h"
#include "game/play_records.h"
#include "game/Unk_022a7b80.h"

/* Needed to match: the original reads data_022a7a9c with its rows, so mwcc
 * folds the row offset into the relocation. Hide the header's one-entry
 * declaration and use BattleResults here. */
#define data_022a7a9c data_022a7a9c_entry
#include "game/Unk_022a7a9c.h"
#undef data_022a7a9c

extern BattleResults data_022a7a9c;
extern Unk_022a7b80 data_022a7b80;

/* At the end of a Survival run, saves the run's COs (player 1's front and
 * back CO) and its result into the record of the current mode variant when
 * the record is empty or the new value is higher. Variants 0-2 keep
 * data_022a7b80.budgetLeft; the others keep mapIndex when it is not 0. */
void Survival_SaveRecord(void) {
  int mode = data_022a7b80.mode;
  u32 value;

  switch (mode) {
    case 0:
    case 1:
    case 2:
      if (gPlayRecords.survivalRecords[mode].co != 0 &&
          gPlayRecords.survivalRecords[mode].value >=
              data_022a7b80.budgetLeft) {
        return;
      }
      gPlayRecords.survivalRecords[mode].co =
          data_022a7a9c.rows[1].unk_00 & PLAYER_CO_ID_MASK;
      gPlayRecords.survivalRecords[mode].tagCo =
          data_022a7a9c.rows[1].unk_02 & PLAYER_CO_ID_MASK;
      gPlayRecords.survivalRecords[mode].value = data_022a7b80.budgetLeft;
      return;
  }
  value = data_022a7b80.mapIndex;
  if (value == 0) {
    return;
  }
  if (gPlayRecords.survivalRecords[mode].co != 0 &&
      gPlayRecords.survivalRecords[mode].value >= value) {
    return;
  }
  gPlayRecords.survivalRecords[mode].co =
      data_022a7a9c.rows[1].unk_00 & PLAYER_CO_ID_MASK;
  gPlayRecords.survivalRecords[mode].tagCo =
      data_022a7a9c.rows[1].unk_02 & PLAYER_CO_ID_MASK;
  gPlayRecords.survivalRecords[mode].value = value;
}
