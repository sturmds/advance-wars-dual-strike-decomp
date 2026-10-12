#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

s32 HwDiv32(s32 numerator, s32 denominator);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Power meter charge in one star for the CO in a player's slot (0 front,
 * 1 back): 50, plus 10 for each power used (140 after 9 uses). At 10 uses
 * the percentage is a flat 200, so a star drops back to 100. */
int PowerStarCharge(int player, int slot) {
  /* front and back are adjacent, so slot indexes from front */
  u32 uses =
      (&data_027c027c->unk_48[data_027c0284]->players[player].front)[slot]
          .powerUses;
  u16 percent = 100;

  if (uses >= 10) {
    percent = 200;
  } else {
    percent += uses * 20;
  }

  return HwDiv32(percent * 50, 100);
}
