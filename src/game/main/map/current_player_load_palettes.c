#include "types.h"
#include "game/Unk_02080440.h"

void Gfx_LoadPlayerBmap0a0Palette(int player);
void Palette_LoadPlayerArmyBgRow(int player);
BOOL Map_IsCoSelectPhase(void);
void PowerPanel_LoadGfx(int player, BOOL coSelectPhase);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Loads the current player's BMAP 0a0 palette and army background row, then
 * PowerPanel_LoadGfx for the player. */
void CurrentPlayer_LoadPalettes(void) {
  Gfx_LoadPlayerBmap0a0Palette(GAME->currentPlayer);
  Palette_LoadPlayerArmyBgRow(GAME->currentPlayer);
  PowerPanel_LoadGfx(GAME->currentPlayer, Map_IsCoSelectPhase());
}
