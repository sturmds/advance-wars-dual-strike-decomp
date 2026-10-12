/* Built without optimization, like the rest of the name entry screen. */
#pragma optimization_level 0

#include "types.h"
#include "game/name_entry_work.h"
#include "game/name_entry.h"
#include "game/text_print.h"

void Gfx_RequestLayerCopy(int layer);

extern u8
    data_0216acc4[]; /* tile x and y of each key label, two bytes per key */
extern u8 data_0216acc5[];  /* data_0216acc4 + 1: the y bytes */
extern u8* data_0216ad5c[]; /* label text of each key */

/* Prints the label of every character key into layer 0. */
void NameEntry_DrawKeyLabels(void) {
  int left = 1;
  int top = 5;
  int i;
  u8 tileX;
  u8 tileY;
  u16 width;
  s8 pad;
  u8 label[3];

  for (i = 0; i < NAME_ENTRY_KEY_COUNT; i++) {
    tileX = data_0216acc4[i * 2];
    tileY = data_0216acc5[i * 2];
    label[0] = *data_0216ad5c[i];
    label[1] = label[2] = 0;
    width = Text_GetPrintWidth(data_0216ad5c[i]);
    /* A label narrower than 6 pixels gets character 24 + pad in front,
         * which centers it in the key. */
    if (width <= 6) {
      pad = (6 - width) >> 1;
      label[1] = label[0];
      label[0] = 24;
      label[0] += pad;
    }
    Text_Print(left + tileX, top + tileY, NameEntry_GetTextMap(), label, 0,
               0x260 + i * 2);
  }
  Gfx_RequestLayerCopy(0);
}
