#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL func_020f788c(void);
void Bg_SetScroll(int layer, int scrollX, int scrollY);

#define VIEW (data_027c027c->unk_48[data_027c0284]->unk_1c4)

/* Sets the scroll of background layers 0 to 3 to the map view scroll,
 * measured from the map tile the background buffer starts at (unk_26,
 * unk_28, 16 per tile). Layer 0 only while func_020f788c is true. */
void MapView_ApplyBgScroll(void) {
  if (func_020f788c()) {
    Bg_SetScroll(0, VIEW->scrollX - VIEW->unk_26 * 16,
                 VIEW->scrollY - VIEW->unk_28 * 16);
  }
  Bg_SetScroll(1, VIEW->scrollX - VIEW->unk_26 * 16,
               VIEW->scrollY - VIEW->unk_28 * 16);
  Bg_SetScroll(2, VIEW->scrollX - VIEW->unk_26 * 16,
               VIEW->scrollY - VIEW->unk_28 * 16);
  Bg_SetScroll(3, VIEW->scrollX - VIEW->unk_26 * 16,
               VIEW->scrollY - VIEW->unk_28 * 16);
}
