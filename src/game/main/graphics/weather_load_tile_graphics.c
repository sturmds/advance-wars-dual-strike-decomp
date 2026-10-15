#include "types.h"
#include "game/Unk_02088d04.h"
#include "sdk/fs.h"

void File_UnpackTo(void* dest, const char* fileName);
void Mem_Copy32(const void* src, void* dst, u32 size);
BOOL func_020fa6f8(void);
u8* Palette_GetArmyBlock(int player);
void Map_LoadExtraObjTiles(int a, int b);

extern char data_021682f8[];
extern char data_0216830c[];
extern char data_02168310[];

/* Copies the four players' army blocks (Palette_GetArmyBlock) into the layer 3 tile data, then unpacks the weather tile
 * file to object VRAM at firstTile (32 bytes per tile) and lets Map_LoadExtraObjTiles load the rest. The tileset argument is
 * not used here. */
void Weather_LoadTileGraphics(int weather, int tileset, int firstTile) {
  const char* file;

  if (func_020fa6f8() == 1) {
    file = data_0216830c;
  } else {
    file = data_02168310;
  }
  Mem_Copy32(Palette_GetArmyBlock(1), data_027c0280->unk_120 + 0x4c00,
             PALETTE_BLOCK_SIZE);
  Mem_Copy32(Palette_GetArmyBlock(2), data_027c0280->unk_120 + 0x4d00,
             PALETTE_BLOCK_SIZE);
  Mem_Copy32(Palette_GetArmyBlock(3), data_027c0280->unk_120 + 0x4e00,
             PALETTE_BLOCK_SIZE);
  Mem_Copy32(Palette_GetArmyBlock(4), data_027c0280->unk_120 + 0x4f00,
             PALETTE_BLOCK_SIZE);
  FS_ChangeDir(data_021682f8);
  File_UnpackTo(data_027c0280->objVram + firstTile * 32, file);
  Map_LoadExtraObjTiles(weather, firstTile);
}
