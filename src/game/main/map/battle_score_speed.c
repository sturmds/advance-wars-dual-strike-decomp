#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_header.h"

BOOL Player1Team_HasPlayerInGame(void);
BOOL Player_IsInGame(u8 player);
int Campaign_IsHardMode(void);
s32 _s32_div_f(s32 numerator,
               s32 denominator); /* _s32_div_f, signed division */

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
/* MapHeader +0x2c and +0x2e: par days on normal and on hard mode */
extern u16 data_ov000_022dbd54[];
extern u16 data_ov000_022dbd56[];

#define STATE (data_027c027c->unk_48[data_027c0284])

/* The map header's u16 at an offset for a map id; the header fields are
 * u8 unk_28[8] for now. */
#define HEADER_U16(symbol, mapId) \
  (*(u16*)((u8*)(symbol) + (mapId) * sizeof(MapHeader)))

/* Speed score (0-100) of a player at the end of a battle: 100 up to the
 * map's par day, then down to 0 at four times par. 0 when player 1's team
 * is out or when the player is out. */
int BattleScore_Speed(int player) {
  int par;

  par = HEADER_U16(data_ov000_022dbd54, STATE->unk_1c4->mapId);
  if (Campaign_IsHardMode()) {
    par = HEADER_U16(data_ov000_022dbd56, STATE->unk_1c4->mapId);
  }
  if (!Player1Team_HasPlayerInGame()) {
    return 0;
  }
  if (!Player_IsInGame(player)) {
    return 0;
  }
  if (STATE->day <= par) {
    return 100;
  }
  if (STATE->day >= par * 4) {
    return 0;
  }
  return 100 - _s32_div_f((STATE->day - par) * 100, par * 3);
}
