#include "types.h"
#include "game/event_picture.h"
#include "game/name_entry.h"
#include "game/screen_hblank.h"
#include "game/screen_other_map.h"

#pragma explicit_zero_data on
/* DISPCNT BG screen offset */
volatile u8 data_027c0238 = {
#include "data/dtcm/data_027c0238.inc.c"
};
/* BG2PB */
u16 data_027c0260 = {
#include "data/dtcm/data_027c0260.inc.c"
};
/* BG1CNT */
u16 data_027c0254 = {
#include "data/dtcm/data_027c0254.inc.c"
};
/* BG2X */
u16 data_027c0250 = {
#include "data/dtcm/data_027c0250.inc.c"
};
/* BG2CNT */
u16 data_027c0258 = {
#include "data/dtcm/data_027c0258.inc.c"
};
/* BG2PA */
u16 data_027c025c = {
#include "data/dtcm/data_027c025c.inc.c"
};
/* BG2Y */
u16 data_027c0244 = {
#include "data/dtcm/data_027c0244.inc.c"
};
s16 data_027c0240 = {
#include "data/dtcm/data_027c0240.inc.c"
};
/* BG2PD */
u16 data_027c0248 = {
#include "data/dtcm/data_027c0248.inc.c"
};
s16 data_027c024c = {
#include "data/dtcm/data_027c024c.inc.c"
};
/* BG2PC */
u16 data_027c023c = {
#include "data/dtcm/data_027c023c.inc.c"
};
/* BLDCNT and BLDALPHA */
u32 data_027c0268 = {
#include "data/dtcm/data_027c0268.inc.c"
};
/* DISPCNT BG/OBJ layer bits */
volatile u32 data_027c0270 = {
#include "data/dtcm/data_027c0270.inc.c"
};
/* BG2OFS */
u32 data_027c0274 = {
#include "data/dtcm/data_027c0274.inc.c"
};
Unk_02080440* data_027c027c = 0;
Unk_027c0280* data_027c0280 = 0;
/* BG1OFS */
u32 data_027c0264 = {
#include "data/dtcm/data_027c0264.inc.c"
};
/* DISPCNT window bits */
volatile u32 data_027c026c = {
#include "data/dtcm/data_027c026c.inc.c"
};
int data_027c0278 = {
#include "data/dtcm/data_027c0278.inc.c"
};
int data_027c0284 = {
#include "data/dtcm/data_027c0284.inc.c"
};
Unk_027c0288 data_027c0288 = {
#include "data/dtcm/data_027c0288.inc.c"
};
u8 gMainScreenContext[324] = {
#include "data/dtcm/gMainScreenContext.inc.c"
};
u8 gSubScreenContext[324] = {
#include "data/dtcm/gSubScreenContext.inc.c"
};
#pragma explicit_zero_data reset
