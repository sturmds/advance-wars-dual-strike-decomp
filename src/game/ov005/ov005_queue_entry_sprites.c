#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_ov005_0235a30c.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Ov005_QueueSpritePair(void* work, int x, int y, int tile, u16 flags);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;

/* Queues the sprites of every entry of the second table on the second screen. */
void Ov005_QueueEntrySprites(Unk_ov005_0235a30c* work) {
  int i = 0;

  for (;;) {
    int tile;
    int x;
    int y;
    Unk_027c0280* saved0280;
    Unk_02080440* saved027c;

    if (work->entriesB[i].id == 0) {
      return;
    }
    x = work->entriesB[i].x;
    y = work->entriesB[i].y;
    tile = work->entriesB[i].tile;
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    Ov005_QueueSpritePair(work, x, y, tile, 1);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
    i++;
  }
}
