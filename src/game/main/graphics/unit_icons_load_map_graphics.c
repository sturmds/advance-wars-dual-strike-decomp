#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/player.h"

void Mem_CopyAuto(const void *src, void *dst, u32 size);
void Palette_Load(const void *src, u32 offset, u32 size);
void Palette_LoadPlayerArmyBgRow(int player);
u8 *func_020c67bc(void);
int func_020c67a8(void);
u8 *func_020c67b0(void);

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern int data_027c0284;
extern u16 gArmyPalettes[][16];

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Copies the unit icon tiles (func_020c67a8 tiles from func_020c67bc) to
 * tile 0x1e1 of the tile data at unk_f8, where gUnitIconTileBase points, and
 * 30 tiles from func_020c67b0 to tile 0x26a. Then loads the army colours of
 * players 1-4 into BG palette rows 12-15 and the current player's army row. */
void UnitIcons_LoadMapGraphics(void)
{
    u8 *tiles = data_027c0280->unk_f8;

    Mem_CopyAuto(func_020c67bc(), tiles + 0x3c20, (u16)(func_020c67a8() * 32));
    Mem_CopyAuto(func_020c67b0(), (u8 *)data_027c0280->unk_f8 + 0x4d40, 0x3c0);
    Palette_Load(gArmyPalettes[GAME->players[1].armyColour - 1], 0x180, 32);
    Palette_Load(gArmyPalettes[GAME->players[2].armyColour - 1], 0x1a0, 32);
    Palette_Load(gArmyPalettes[GAME->players[3].armyColour - 1], 0x1c0, 32);
    Palette_Load(gArmyPalettes[GAME->players[4].armyColour - 1], 0x1e0, 32);
    Palette_LoadPlayerArmyBgRow(GAME->currentPlayer);
}
