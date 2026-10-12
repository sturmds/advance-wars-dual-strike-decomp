#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_tileset_files.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u8 data_0214d19c
    []; /* terrain palettes, 32 bytes each; army palettes start at 6 */
extern char data_021682f8[];

void FS_ChangeDir(const char* dir);
void Palette_Load(const void* src, u16 offset, u32 size);
void Palette_LoadFromFile(const char* fileName, int fileOffset,
                          int paletteOffset, int size);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Loads OBJ palette row `row`. In the Design Room it loads the army terrain
 * palettes instead: each player slot's army colour into rows row to row + 4
 * and army 6 into row + 5. Elsewhere it loads the tileset's OBJ palette
 * file. */
void MapGfx_LoadTilesetObjPalette(int weather, int tileset, int row) {
  const char* file;
  int i;
  u8* armyPalettes;

  /* needed to match: the cast keeps the add at run time */
  armyPalettes = (u8*)data_0214d19c + (u32)(6 * 32);
  file = data_02167e30[tileset].objPaletteFile;

  if (STATE->gameMode == GAME_MODE_DESIGN_ROOM) {
    Palette_Load(armyPalettes + STATE->players[0].armyColour * 32,
                 (row + 16) * 32, 32);
    Palette_Load(armyPalettes + 6 * 32, (row + 21) * 32, 32);
    for (i = 1; i < 5; i++) {
      Palette_Load(armyPalettes + STATE->players[i].armyColour * 32,
                   (row + 16 + i) * 32, 32);
    }
  } else {
    FS_ChangeDir(data_021682f8);
    Palette_LoadFromFile(file, 0, (u16)((row + 16) * 32), 32);
  }
}
