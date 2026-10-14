#include "types.h"
#include "game/ov003_unit.h"

s32 HwDiv32(s32 numerator, s32 denominator);
void Ov003_DrawFrameAt(int x, int y, int frame);
extern Ov003Unit data_ov003_02369538[];
extern Ov003TileInfo data_ov003_02364d88[];

/* For each live unit whose unk_04 is over 30, picks an animation frame (0 to 19)
 * from how far unk_04 is past 30 compared to its tile's unk_00, and draws it at
 * the unit's tile through Ov003_DrawFrameAt. */
void Ov003_DrawUnitAnims(void) {
  Ov003Unit* unit = data_ov003_02369538;
  int i;
  int frame;

  for (i = 0; i < OV003_UNIT_COUNT; i++, unit++) {
    if (unit->hp != 0 && !unit->unk_17_6 && unit->unk_04 > 30) {
      frame = 20 - HwDiv32((unit->unk_04 - 30) * 20,
                           data_ov003_02364d88[unit->kind].unk_00);
      if (frame <= 19) {
        Ov003_DrawFrameAt((unit->x & ~0x7f) >> 3, (unit->y & ~0x7f) >> 3,
                          frame);
      }
    }
  }
}
