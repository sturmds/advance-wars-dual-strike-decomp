#include "types.h"
#include "game/Unk_02080440.h"

void Gfx_LoadPlayerBmap0a0Palette(int player);
void Palette_LoadPlayerArmyBgRow(int player);
BOOL Map_IsCoSelectPhase(void);
void func_020e15d8(int player, BOOL coSelectPhase);

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Loads the current player's BMAP 0a0 palette and army background row, then
 * func_020e15d8 for the player. */
void CurrentPlayer_LoadPalettes(void)
{
    Gfx_LoadPlayerBmap0a0Palette(GAME->currentPlayer);
    Palette_LoadPlayerArmyBgRow(GAME->currentPlayer);
    func_020e15d8(GAME->currentPlayer, Map_IsCoSelectPhase());
}
