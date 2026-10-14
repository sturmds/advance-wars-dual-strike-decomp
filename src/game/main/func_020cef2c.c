#include "types.h"
#include "game/player.h"

BOOL Player_IsInGame(u8 player);
int func_020cf17c(u16 player);
int func_020ceffc(int player);
int func_020cf090(u16 player);
int func_020cf0e4(u16 player);
void LinkPlayer_Defeat(int player, int code);

void func_020cef2c(void) {
  s16 player;

  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    if (!Player_IsInGame(player)) {
      continue;
    }
    if (!func_020cf17c(player)) {
      LinkPlayer_Defeat(player, 8);
    } else if (!func_020ceffc(player)) {
      LinkPlayer_Defeat(player, 16);
    } else if (!func_020cf090(player)) {
      LinkPlayer_Defeat(player, 1);
    } else if (!func_020cf0e4(player)) {
      LinkPlayer_Defeat(player, 2);
    }
  }
}
