#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_ov006_0235f690.h"

void func_020edb84(MapUnit* unit);
void func_ov006_0235f690(void);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern Unk_ov006_0235f690* data_ov006_0236d640;

/* data_027c027c->unk_48[data_027c0284], written so that the compiler loads
 * the index first, as the original does. */
#define STATE                                                             \
  (*(Unk_02080440_48**)((u8*)(data_027c0284 * sizeof(Unk_02080440_48*)) + \
                            (u32)data_027c027c + (u32) &                  \
                        ((Unk_02080440*)0)->unk_48))

/* Takes the unit on tile x, y off the map. With withEffect it starts the
 * unit task (func_020edb84); otherwise it clears the unit's type. Then
 * counts the players' units again. Returns TRUE if a unit was there. */
int Ov006_RemoveUnitAt(int withEffect, int x, int y) {
  int removed = FALSE;
  int slot = STATE->mapTiles->unitAt[x + STATE->mapTiles->rowStart[y]];

  if (slot != 0) {
    MapUnit* unit = &STATE->units[slot];

    if (unit != NULL && withEffect) {
      func_020edb84(unit);
    } else {
      unit->type = 0;
    }
    func_ov006_0235f690();
    data_ov006_0236d640->recountDelay = 5;
    removed = TRUE;
  }
  return removed;
}
