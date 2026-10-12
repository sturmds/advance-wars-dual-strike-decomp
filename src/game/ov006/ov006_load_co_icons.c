#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;

extern s8 data_ov006_0236a7b4[];
extern s8 data_ov006_0236a7d0[];
extern u8 gArmyPalettes[][32];

int GameFlag_IsSet(int flag);
void Palette_Load(const void* src, u32 offset, u32 size);
void TransferQueue_Add(const void* src, void* dst, u32 size);
u8* UnitIcons_GetTiles(void);
int Player_GetUnitIconTile(int player, int unitType);

/* Loads the CO icons of the list (data_ov006_0236a7b4, or data_ov006_0236a7d0
 * with one more entry when game flag 0x2b is set) as 4-tile sprites at
 * 0x4bc0 in OBJ VRAM of the first screen. The tile index of each CO comes from
 * Player_GetUnitIconTile, which picks its row by the player's army style (the sprite
 * set). Palette slot 6 of the sprite palettes gets the player's army colour
 * (the palette). player 0 means player 1. Names are guesses: the entries run
 * over 1 to 25 (26), as CO ids do. */
void Ov006_LoadCoIcons(int player) {
  u8* dst;
  s8* table;
  int i;
  int count;

  if (player == 0) {
    player = 1;
  }
  dst = (u8*)((Unk_027c0280*)data_027c0288.unk_00)->objVram + 0x4bc0;
  if (GameFlag_IsSet(0x2b) == 1) {
    table = data_ov006_0236a7d0;
    count = 26;
  } else {
    table = data_ov006_0236a7b4;
    count = 25;
  }
  for (i = 0; i < count; i++) {
    TransferQueue_Add(UnitIcons_GetTiles() +
                          Player_GetUnitIconTile(player, *table & 0x3f) * 32,
                      dst, 128);
    dst += 128;
    table++;
  }
  Palette_Load(
      gArmyPalettes
          [data_027c027c->unk_48[data_027c0284]->players[player].armyColour -
           1],
      0x2c0, 32);
}
