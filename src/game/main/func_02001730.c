#include "types.h"
#include "game/player.h"

int func_0200174c(int player, int slot);

int func_02001730(int player, int slot)
{
    return func_0200174c(player, slot) & PLAYER_CO_ID_MASK;
}
