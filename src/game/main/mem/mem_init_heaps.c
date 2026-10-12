#include "types.h"
#include "game/heap.h"

void* OS_AllocFromArenaLo(
    int arena, u32 size,
    int align);               /* SDK: takes memory from the start of an arena */
u32 OS_GetArenaLo(int arena); /* SDK: arena low address */
u32 OS_GetArenaHi(int arena); /* SDK: arena high address */
int func_02112c8c(
    void* table,
    int size); /* SDK: loads the ROM file table; (0, 0) returns its size */

ExpHeap* Heap_Create(void* start, u32 size, u16 options);

extern ExpHeap* data_0218ff54;

/* Splits the main arena into a 64 KiB heap and a game heap (gGameHeap) with
 * the rest, then loads the ROM file table into the small heap. */
void Mem_InitHeaps(void) {
  void* smallBlock;
  void* bigBlock;
  u32 arenaLo;
  u32 arenaHi;
  u32 bigSize;
  int tableSize;

  smallBlock = OS_AllocFromArenaLo(0, 0x10000, 16);
  arenaLo = OS_GetArenaLo(0);
  arenaHi = OS_GetArenaHi(0);
  bigSize = (arenaHi & ~15) - ((arenaLo + 15) & ~15);
  bigBlock = OS_AllocFromArenaLo(0, bigSize, 16);
  data_0218ff54 = Heap_Create(smallBlock, 0x10000, 0);
  gGameHeap = Heap_Create(bigBlock, bigSize, 0);
  tableSize = func_02112c8c(0, 0);
  func_02112c8c(Heap_Alloc(data_0218ff54, tableSize, 16), tableSize);
}
