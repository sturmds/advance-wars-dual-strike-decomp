#include "types.h"
#include "game/player.h"

int Player_GetCoByteFallback(int player, int slot);

int func_02001730(int player, int slot) {
  return Player_GetCoByteFallback(player, slot) & PLAYER_CO_ID_MASK;
}
