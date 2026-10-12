#include "types.h"
#include "game/Unk_ov005_0236bb90.h"

void Mem_Copy32(const void* src, void* dst, u32 size);

/* Copies the 8 chunks of palette set `set` into column `column` of each chunk
 * row. Unrolled in the original. */
void Ov005Work_CopyPaletteSet(Unk_ov005_0236bb90* work, int set, int column) {
  Mem_Copy32(work->palettes[set][0], work->palettesOut[0][column], 64);
  Mem_Copy32(work->palettes[set][1], work->palettesOut[1][column], 64);
  Mem_Copy32(work->palettes[set][2], work->palettesOut[2][column], 64);
  Mem_Copy32(work->palettes[set][3], work->palettesOut[3][column], 64);
  Mem_Copy32(work->palettes[set][4], work->palettesOut[4][column], 64);
  Mem_Copy32(work->palettes[set][5], work->palettesOut[5][column], 64);
  Mem_Copy32(work->palettes[set][6], work->palettesOut[6][column], 64);
  Mem_Copy32(work->palettes[set][7], work->palettesOut[7][column], 64);
}
