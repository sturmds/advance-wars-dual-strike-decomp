#include "types.h"
#include "game/menu_state.h"
#include "game/screen_other_map.h"

void* File_Load(const char* path);
void Data_Unpack(const void* src, void* dest);
void Palette_Load(const void* src, u32 offset, u32 size);
void Mem_Copy32(const void* src, void* dst, u32 size);
void Mem_Free(void* block);

extern char data_0215c004[];
extern MenuState gMenuState;

#define ARMY_BLACK_HOLE 5

/* Offsets into the day banner file. */
#define FILE_BG_TILES 0x0000
#define FILE_BG_MAP 0x0f30
#define FILE_BLACK_HOLE_TILES 0x133c
#define FILE_BLACK_HOLE_MAP 0x1aa8
#define FILE_PALETTES 0x1e0c /* one 16-colour palette per army colour */
#define FILE_SPRITE_TILES 0x1eac

#define PALETTE_SIZE 32
#define BG_PALETTE(n) ((n) * PALETTE_SIZE)
#define OBJ_PALETTE(n) (0x200 + (n) * PALETTE_SIZE)
#define BANNER_SPRITE_VRAM (404 * 32) /* WORD_TILE in the day banner steps */

/* Task script step that loads the day banner graphics for the current
 * player's army colour: the layer 0 background (Black Hole has its own),
 * its map moved to BG palette 1 and saved in gMenuState, the word and digit
 * sprite tiles, and the army's palette as BG palette 1 and OBJ palette 3. */
void DayBanner_LoadGraphics(void) {
  u8 army = data_027c027c->unk_48[data_027c0284]
                ->players[data_027c027c->unk_48[data_027c0284]->currentPlayer]
                .armyColour;
  u8* file = File_Load(data_0215c004);
  const u8* palette;
  int i;

  if (army == ARMY_BLACK_HOLE) {
    Data_Unpack(
        file + FILE_BLACK_HOLE_TILES,
        data_027c0280->bgVram +
            (data_027c0280->bgCnt[data_027c0280->layer0Bg].charBase << 14));
    Data_Unpack(file + FILE_BLACK_HOLE_MAP, data_027c0280->layer0Map);
  } else {
    Data_Unpack(
        file + FILE_BG_TILES,
        data_027c0280->bgVram +
            (data_027c0280->bgCnt[data_027c0280->layer0Bg].charBase << 14));
    Data_Unpack(file + FILE_BG_MAP, data_027c0280->layer0Map);
  }

  for (i = 0; i < 0x400; i++) {
    data_027c0280->layer0Map[i] += 1 << 12;
  }

  palette = file + FILE_PALETTES + (army - 1) * PALETTE_SIZE;
  Palette_Load(palette, BG_PALETTE(1), PALETTE_SIZE);
  Mem_Copy32(data_027c0280->layer0Map, &gMenuState, 0x800);
  Data_Unpack(file + FILE_SPRITE_TILES,
              data_027c0280->objVram + BANNER_SPRITE_VRAM);
  Palette_Load(palette, OBJ_PALETTE(3), PALETTE_SIZE);
  Mem_Free(file);
}
