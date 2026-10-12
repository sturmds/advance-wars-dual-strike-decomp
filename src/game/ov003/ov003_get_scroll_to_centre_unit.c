#include "types.h"
#include "game/ov003_unit.h"

/* Scroll position that centres the unit in the 256 x 192 view, kept inside the map. */
void Ov003_GetScrollToCentreUnit(const Ov003Unit* unit, s16* scrollX,
                                 s16* scrollY) {
  *scrollX = (unit->x >> 3) - 128;
  *scrollY = (unit->y >> 3) - 96;
  if (*scrollX < 0) {
    *scrollX = 0;
  }
  if (*scrollY < 0) {
    *scrollY = 0;
  }
  if (*scrollX > data_ov003_02364a88->width * 16 - 257) {
    *scrollX = data_ov003_02364a88->width * 16 - 256;
  }
  if (*scrollY > data_ov003_02364a88->height * 16 - 193) {
    *scrollY = data_ov003_02364a88->height * 16 - 192;
  }
}
