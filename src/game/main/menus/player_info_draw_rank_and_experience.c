#include "types.h"
#include "game/menu_state.h"

extern MenuState gMenuState;
extern u8 data_027c0028;
extern u8 data_027c0038;
extern u8 data_027c0040;
extern u8 data_027c0060;

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
int Co_GetRankFiltered(int coId, int army);
int Co_GetExperienceFiltered(int coId, int army);
s32 HwDiv32(s32 numerator, s32 denominator);
s32 HwMod32(s32 numerator, s32 denominator);

#define MENU (gMenuState.unk_0206d67c)

/* MENU.coShown[player - 1][slot]. Needed to match: the original adds the
 * offset of coShown to gMenuState's address at run time. */
#define SHOWN_CO_OFFSET ((u32) & ((MenuState*)0)->unk_0206d67c.coShown)
#define SHOWN_CO \
  (((u8*)&gMenuState + (MENU.player - 1) * 2 + MENU.slot)[SHOWN_CO_OFFSET])

/* Rank and experience sprites of the shown CO on the player info screen:
 * the rank (Co_GetRankFiltered, ExperienceToRank of the experience) as up to two
 * digits, or one sprite at rank 100, then the experience modulo 1000 as up
 * to three digits. Both lookups pass the player as the army argument. */
void PlayerInfo_DrawRankAndExperience(void) {
  int rank;
  int exp;
  int tens;
  BOOL shown;

  rank = Co_GetRankFiltered(SHOWN_CO, MENU.player);
  exp = Co_GetExperienceFiltered(SHOWN_CO, MENU.player);
  Sprite_QueueAt(0, 56, 16, &data_027c0060, 0x2000, 48);
  if (rank == 100) {
    Sprite_QueueAt(0, 88, 16, &data_027c0040, 0x2000, 76);
  } else {
    if (rank >= 10) {
      tens = HwDiv32(rank, 10);
      rank = HwMod32(rank, 10);
      Sprite_QueueAt(0, 88, 16, &data_027c0038, 0x2000, tens * 2 + 56);
    }
    Sprite_QueueAt(0, 96, 16, &data_027c0038, 0x2000, rank * 2 + 56);
  }
  Sprite_QueueAt(0, 64, 32, &data_027c0028, 0x2000, 100);
  Sprite_QueueAt(0, 96, 32, &data_027c0028, 0x2000, 102);
  shown = FALSE;
  exp = HwMod32(exp, 1000);
  if (exp >= 100) {
    tens = HwDiv32(exp, 100);
    exp = HwMod32(exp, 100);
    Sprite_QueueAt(0, 72, 32, &data_027c0028, 0x2000, tens * 2 + 80);
    shown = TRUE;
  }
  if (shown || exp >= 10) {
    tens = HwDiv32(exp, 10);
    exp = HwMod32(exp, 10);
    Sprite_QueueAt(0, 80, 32, &data_027c0028, 0x2000, tens * 2 + 80);
  }
  Sprite_QueueAt(0, 88, 32, &data_027c0028, 0x2000, exp * 2 + 80);
}
