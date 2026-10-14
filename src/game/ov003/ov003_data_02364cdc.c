#include "types.h"
#include "game/ov003_unit.h"

extern u8 data_ov003_02364f18[];
extern u8 data_ov003_02364f50[];
extern u8 data_ov003_02365004[];
extern u8 data_ov003_023650a4[];
extern u8 data_ov003_0236510c[];
extern u8 data_ov003_023651f0[];
extern u8 data_ov003_02364f88[];
extern u8 data_ov003_02364fc0[];
extern u8 data_ov003_02365054[];
extern u8 data_ov003_02365178[];

u16 data_ov003_02364cdc[][2] = {
#include "data/ov003/data_ov003_02364cdc.inc.c"
};
u8 data_ov003_02364cfc[] = {
#include "data/ov003/data_ov003_02364cfc.inc.c"
};
Ov003TileInfo data_ov003_02364d88[] = {
#include "data/ov003/data_ov003_02364d88.inc.c"
};
/* declared `Ov003Preset data_ov003_02364e50[]` in ov003_init_rows.c: Ov003Preset.rowCount is a bit field */
u32 data_ov003_02364e50[] = {
#include "data/ov003/data_ov003_02364e50.inc.c"
};
