#include "types.h"
#include "game/Unk_02080440.h"
#include "game/player.h"

void Palette_Load(const void* src, u32 offset, u32 size);
void Gfx_LoadPlayerWindowFrame(int player);
BOOL Map_IsCoSelectPhase(void);
void Palette_LoadPlayerArmyBgRow(int player);
void PowerPanel_LoadGfx(int player, BOOL coSelectPhase);
void ObjGfx_LoadLanguageTiles(void);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u16 gArmyPalettes[][16];
extern u16 data_021396f0[16];

#define GAME (data_027c027c->unk_48[data_027c0284])
#define STATE_50 (data_027c027c->unk_50[data_027c0284])

/* Reloads the map screen palettes: the army colours of players 1-4 into BG
 * palette rows 12-15, data_021396f0 into OBJ row 2, then the state's
 * weather and tileset hook and the current player's window frame and army
 * rows. The Design Room (overlay 6) calls it. */
void Map_ReloadPalettes(void) {
  Palette_Load(gArmyPalettes[GAME->players[1].armyColour - 1], 0x180, 32);
  Palette_Load(gArmyPalettes[GAME->players[2].armyColour - 1], 0x1a0, 32);
  Palette_Load(gArmyPalettes[GAME->players[3].armyColour - 1], 0x1c0, 32);
  Palette_Load(gArmyPalettes[GAME->players[4].armyColour - 1], 0x1e0, 32);
  Palette_Load(data_021396f0, 0x240, 32);
  if (STATE_50->unk_58 != NULL) {
    STATE_50->unk_58(GAME->unk_1c4->weather, GAME->unk_1c4->tileset);
  }
  ObjGfx_LoadLanguageTiles();
  Gfx_LoadPlayerWindowFrame(GAME->currentPlayer);
  PowerPanel_LoadGfx(GAME->currentPlayer, Map_IsCoSelectPhase());
  Palette_LoadPlayerArmyBgRow(GAME->currentPlayer);
}
