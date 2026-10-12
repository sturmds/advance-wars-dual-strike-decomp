#include "types.h"
#include "game/Unk_02080440.h"

BOOL GameFlag_IsSet(int id);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u32 gLinkPlay;

/* TRUE when skills do not apply: game flag 0x27 not set yet, or game mode 1
 * with unk_59 of 0 or 2. Always FALSE while gLinkPlay is nonzero. */
BOOL Skills_AreDisabled(void) {
  if (gLinkPlay != 0) {
    return FALSE;
  }
  if (!GameFlag_IsSet(0x27)) {
    return TRUE;
  }
  if (data_027c027c->unk_48[data_027c0284]->gameMode == GAME_MODE_WAR_ROOM) {
    if (data_027c027c->unk_48[data_027c0284]->unk_59 == 0) {
      return TRUE;
    }
    if (data_027c027c->unk_48[data_027c0284]->unk_59 == 2) {
      return TRUE;
    }
  }
  return FALSE;
}
