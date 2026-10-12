#include "types.h"
#include "game/play_records.h"

/* Adds amount to three saved counters: unk_384 (cap 999999), points and
 * unk_04 (cap 99999). points is the balance Hachi's shop spends from
 * (PlayRecords_SpendPoints). */
void PlayRecords_AddCoins(u32 amount) {
  if (gPlayRecords.unk_384 + amount < 999999) {
    gPlayRecords.unk_384 += amount;
  } else {
    gPlayRecords.unk_384 = 999999;
  }
  if (99999 - gPlayRecords.points < amount) {
    gPlayRecords.points = 99999;
  } else {
    gPlayRecords.points += amount;
  }
  if (99999 - gPlayRecords.unk_04 < amount) {
    gPlayRecords.unk_04 = 99999;
  } else {
    gPlayRecords.unk_04 += amount;
  }
}
