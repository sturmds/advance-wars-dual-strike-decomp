#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL Player_IsInGame(u8 player);
int func_020cf17c(u16 player);
int func_020ceffc(int player);
int func_020cf090(u16 player);
int func_020cf0e4(u16 player);
void Player_SetUnk33(int player);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* TRUE when the players still standing are all on one team. A player
 * stands while in the game and passing the four checks that
 * func_020cef2c turns into defeats. With bit 4 of unk_1c4->unk_37 set, a
 * player whose teammate is in the game but no longer stands falls too
 * (Player_SetUnk33 marks it). */
BOOL Players_IsOneTeamLeft(void) {
  u8 standing[5];
  int i;
  int j;
  int opponents;

  for (i = 1; i < 5; i++) {
    standing[i] = FALSE;
    if (Player_IsInGame(i) && func_020cf17c(i) && func_020ceffc(i) &&
        func_020cf090(i) && func_020cf0e4(i)) {
      standing[i] = TRUE;
    }
  }
  if (STATE->unk_1c4->unk_37 & 4) {
    for (i = 1; i < 5; i++) {
      if (standing[i]) {
        for (j = 1; j < 5; j++) {
          if (i != j && Player_IsInGame(j) &&
              STATE->players[i].team == STATE->players[j].team &&
              !standing[j]) {
            standing[i] = FALSE;
            Player_SetUnk33(i);
          }
        }
      }
    }
  }
  for (i = 1; i < 5; i++) {
    if (standing[i]) {
      opponents = 0;
      for (j = 1; j < 5; j++) {
        if (i != j && standing[j] &&
            STATE->players[i].team != STATE->players[j].team) {
          opponents++;
        }
      }
      if (opponents == 0) {
        return TRUE;
      }
    }
  }
  return FALSE;
}
