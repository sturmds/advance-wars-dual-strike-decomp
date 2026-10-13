#include "types.h"
#include "game/ov003_unit.h"

void Ov003Row_DrawCharge(int row);
int MainLoop_GetFrameCount(void);
void Palette_Load(const void* src, int offset, u32 size);

/* size: up to the next symbol; also declared in src/game/ov003/ov003_load_sub_screen_graphics.c */
u16 data_ov003_02369be8[0x10];

/* With unk_1df_6 set, runs Ov003Row_DrawCharge on each row in activeRows. Then loads one of 16 colours, a new one every 4 frames, into palette entry 0x10f. */
void Ov003_UpdateRowsAndPalette(void) {
  int i;

  if (data_ov003_02364928->unk_1df_6) {
    for (i = 0; i < data_ov003_02364928->rowCount; i++) {
      if (data_ov003_02364928->activeRows & (1 << i)) {
        Ov003Row_DrawCharge(i + 1);
      }
    }
  }
  Palette_Load(&data_ov003_02369be8[(MainLoop_GetFrameCount() >> 2) & 15],
               0x21e, 2);
}
