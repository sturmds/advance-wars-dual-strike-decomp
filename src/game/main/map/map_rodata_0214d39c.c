#include "types.h"

/* palette and first tile: [0] grey (waited), then players 1-4 */
const u16 gUnitIconTileBase[6] = {
#include "data/main/gUnitIconTileBase.inc.c"
};
/* map bytes of a rotation/scaling BG by screen size */
const u32 data_0214d3a8[4] = {
#include "data/main/data_0214d3a8.inc.c"
};
/* map bytes of a text BG by screen size */
const u32 data_0214d3b8[4] = {
#include "data/main/data_0214d3b8.inc.c"
};
/* per shown HP 0-10: negative when no digit is drawn (0 and 10) */
const s16 gUnitIconHpDigit[12] = {
#include "data/main/gUnitIconHpDigit.inc.c"
};
