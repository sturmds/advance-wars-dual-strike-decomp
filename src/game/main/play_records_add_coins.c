#include "types.h"
#include "game/play_records.h"

/* Adds amount to three saved counters: unk_384 (cap 999999) and unk_00 and
 * unk_04 (cap 99999). Coins are a guess: unk_00 is the balance func_02017bf8
 * spends from, and the 99999 cap suggests a currency. */
void PlayRecords_AddCoins(u32 amount)
{
    if (gPlayRecords.unk_384 + amount < 999999) {
        gPlayRecords.unk_384 += amount;
    } else {
        gPlayRecords.unk_384 = 999999;
    }
    if (99999 - gPlayRecords.unk_00 < amount) {
        gPlayRecords.unk_00 = 99999;
    } else {
        gPlayRecords.unk_00 += amount;
    }
    if (99999 - gPlayRecords.unk_04 < amount) {
        gPlayRecords.unk_04 = 99999;
    } else {
        gPlayRecords.unk_04 += amount;
    }
}
