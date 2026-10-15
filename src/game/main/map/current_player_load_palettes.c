#include "types.h"
#include "game/Unk_02080440.h"

void Gfx_LoadPlayerBmap0a0Palette(int player);
void Palette_LoadPlayerArmyBgRow(int player);
BOOL Map_IsCoSelectPhase(void);
void PowerPanel_LoadGfx(int player, BOOL coSelectPhase);

/* Loads the current player's BMAP 0a0 palette and army background row, then
 * PowerPanel_LoadGfx for the player. */
void CurrentPlayer_LoadPalettes(void) {
  Gfx_LoadPlayerBmap0a0Palette(MAP_STATE->currentPlayer);
  Palette_LoadPlayerArmyBgRow(MAP_STATE->currentPlayer);
  PowerPanel_LoadGfx(MAP_STATE->currentPlayer, Map_IsCoSelectPhase());
}
