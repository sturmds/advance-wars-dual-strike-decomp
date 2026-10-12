#include "types.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_0236bba8.h"
#include "sdk/os.h"

/* Needed to match: the original keeps the unused `loaded` count. */
#pragma opt_dead_assignments off

void* Mem_Alloc(u32 size);
void Mem_Free(void* block);
void Mem_Fill32(u32 value, void* dst, u32 size);
void func_02028808(void);
void Co_LoadTexture(int coId, int tileOffset, int paletteOffset);
void Gfx_LoadTexPalette(void* palette, int start, int offset);

/* Loads the picture texture of every CO in the grid, one 0x180 tile block and one 32-byte
 * palette slot per CO. A CO that is not available gets an all-zero palette. */
void Ov005_LoadCoGridTextures(Ov005Task* task) {
  Unk_ov005_0236bba8* work = task->work;
  void* blank;
  int i;
  int loaded;
  Ov005CoGridEntry* entry;
  int paletteOffset;
  int tileOffset;

  blank = Mem_Alloc(32);
  Mem_Fill32(0, blank, 32);
  DC_FlushRange(blank, 32);
  func_02028808();
  loaded = 0;
  i = 0;
  paletteOffset = 0;
  tileOffset = 0;
  entry = work->grid;
  for (; i < OV005_CO_GRID_ENTRIES; i++, entry++) {
    if (entry->coId == -1) {
      continue;
    }
    Co_LoadTexture(entry->coId, tileOffset, paletteOffset);
    if (entry->available == 0) {
      Gfx_LoadTexPalette(blank, paletteOffset, paletteOffset);
    }
    paletteOffset += 32;
    tileOffset += 0x180;
    loaded++;
  }
  Mem_Free(blank);
}
