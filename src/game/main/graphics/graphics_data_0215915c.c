#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/screen_contexts.h"
#include "game/task.h"
#include "gfx/wallpaper_offsets.h" /* build/gen, from the wallpaper files (tools/gfx.py) */

void WallpaperSingleTask_Update(void);
void WallpaperTask_Init(void);
void WallpaperTask_InitScrolled(void);
void WallpaperTask_Update(void);

u8 data_02159168[] = {
#include "data/main/data_02159168.inc.c"
};
u8 data_0215915c[] = {
#include "data/main/data_0215915c.inc.c"
};
char data_02159174[] = {
#include "data/main/data_02159174.inc.c"
};
char data_02159194[] = {
#include "data/main/data_02159194.inc.c"
};
char data_021591a4[] = {
#include "data/main/data_021591a4.inc.c"
};
char data_02159184[] = {
#include "data/main/data_02159184.inc.c"
};
char data_021591d4[] = {
#include "data/main/data_021591d4.inc.c"
};
char data_021591e4[] = {
#include "data/main/data_021591e4.inc.c"
};
char data_021591c4[] = {
#include "data/main/data_021591c4.inc.c"
};
char data_021591f4[] = {
#include "data/main/data_021591f4.inc.c"
};
char data_02159204[] = {
#include "data/main/data_02159204.inc.c"
};
char data_02159214[] = {
#include "data/main/data_02159214.inc.c"
};
char data_021591b4[] = {
#include "data/main/data_021591b4.inc.c"
};
char data_02159224[] = {
#include "data/main/data_02159224.inc.c"
};
char data_02159234[] = {
#include "data/main/data_02159234.inc.c"
};
char data_02159244[] = {
#include "data/main/data_02159244.inc.c"
};
char data_021592a4[] = {
#include "data/main/data_021592a4.inc.c"
};
char data_02159254[] = {
#include "data/main/data_02159254.inc.c"
};
char data_02159264[] = {
#include "data/main/data_02159264.inc.c"
};
char data_02159274[] = {
#include "data/main/data_02159274.inc.c"
};
char data_02159284[] = {
#include "data/main/data_02159284.inc.c"
};
char data_02159294[] = {
#include "data/main/data_02159294.inc.c"
};
char data_021592b4[] = {
#include "data/main/data_021592b4.inc.c"
};
char data_021592c4[] = {
#include "data/main/data_021592c4.inc.c"
};
char data_021592d4[] = {
#include "data/main/data_021592d4.inc.c"
};
char data_021592e4[] = {
#include "data/main/data_021592e4.inc.c"
};
char data_021592f4[] = {
#include "data/main/data_021592f4.inc.c"
};
char data_02159318[] = {
#include "data/main/data_02159318.inc.c"
};
char data_02159304[] = {
#include "data/main/data_02159304.inc.c"
};
char data_0215932c[] = {
#include "data/main/data_0215932c.inc.c"
};
char data_02159340[] = {
#include "data/main/data_02159340.inc.c"
};
char data_02159354[] = {
#include "data/main/data_02159354.inc.c"
};
char data_02159368[] = {
#include "data/main/data_02159368.inc.c"
};
TaskScriptCmd data_0215937c[] = {
#include "data/main/data_0215937c.inc.c"
};
TaskScriptCmd gWallpaperTaskScript[] = {
#include "data/main/gWallpaperTaskScript.inc.c"
};
/* per wallpaper: offset of its tile map in the file of data_021595cc */
u16 data_021593fc[] = {
    RES_WALL_DEFAULT_MAP,
    RES_WALL_CATHY_MAP,
    RES_WALL_RYO_MAP,
    RES_WALL_MAX_MAP,
    RES_WALL_WHIP_MAP,
    RES_WALL_DOMINO_MAP,
    RES_WALL_BILLY_MAP,
    RES_WALL_KIKUCHIYO_MAP,
    RES_WALL_ASUKA_MAP,
    RES_WALL_EAGLE_MAP,
    RES_WALL_MOP_MAP,
    RES_WALL_HAGEVOLT_MAP,
    RES_WALL_ZIPO_MAP,
    RES_WALL_CAT_MAP,
    RES_WALL_CHAKKA_MAP,
    RES_WALL_HAWKE_MAP,
    RES_WALL_HACHI_MAP,
    RES_WALL_EWAN_MAP,
    RES_WALL_HANNA_MAP,
    RES_WALL_YAMAMOTO_MAP,
    RES_WALL_JOHN_MAP,
    RES_WALL_RACHEL_MAP,
    RES_WALL_SASHA_MAP,
    RES_WALL_BITTMANN_MAP,
    RES_WALL_KOUZOU_MAP,
    RES_WALL_CANDLE_MAP,
    RES_WALL_KONG_MAP,
    RES_WALL_SNAKE_MAP,
    /* the extra wallpapers, from WALLPAPER_EXTRA_FIRST */
    RES_WALL_AGB1_MAP,
    RES_WALL_AGB2_MAP,
    RES_WALL_RANKMAX_MAP,
    0, /* the ROM's table ends in a zero, past the last wallpaper */
};
/* per wallpaper: offset of its palette in the file of data_021595cc */
u16 data_021593bc[] = {
    RES_WALL_DEFAULT_PALETTE,
    RES_WALL_CATHY_PALETTE,
    RES_WALL_RYO_PALETTE,
    RES_WALL_MAX_PALETTE,
    RES_WALL_WHIP_PALETTE,
    RES_WALL_DOMINO_PALETTE,
    RES_WALL_BILLY_PALETTE,
    RES_WALL_KIKUCHIYO_PALETTE,
    RES_WALL_ASUKA_PALETTE,
    RES_WALL_EAGLE_PALETTE,
    RES_WALL_MOP_PALETTE,
    RES_WALL_HAGEVOLT_PALETTE,
    RES_WALL_ZIPO_PALETTE,
    RES_WALL_CAT_PALETTE,
    RES_WALL_CHAKKA_PALETTE,
    RES_WALL_HAWKE_PALETTE,
    RES_WALL_HACHI_PALETTE,
    RES_WALL_EWAN_PALETTE,
    RES_WALL_HANNA_PALETTE,
    RES_WALL_YAMAMOTO_PALETTE,
    RES_WALL_JOHN_PALETTE,
    RES_WALL_RACHEL_PALETTE,
    RES_WALL_SASHA_PALETTE,
    RES_WALL_BITTMANN_PALETTE,
    RES_WALL_KOUZOU_PALETTE,
    RES_WALL_CANDLE_PALETTE,
    RES_WALL_KONG_PALETTE,
    RES_WALL_SNAKE_PALETTE,
    /* the extra wallpapers, from WALLPAPER_EXTRA_FIRST */
    RES_WALL_AGB1_PALETTE,
    RES_WALL_AGB2_PALETTE,
    RES_WALL_RANKMAX_PALETTE,
    0, /* the ROM's table ends in a zero, past the last wallpaper */
};
GfxLayer data_0215943c[] = {
#include "data/main/data_0215943c.inc.c"
};
GfxLayer data_021594dc[] = {
#include "data/main/data_021594dc.inc.c"
};
GfxLayer data_0215952c[] = {
#include "data/main/data_0215952c.inc.c"
};
GfxLayer data_0215948c[] = {
#include "data/main/data_0215948c.inc.c"
};
GfxLayer data_0215957c[] = {
#include "data/main/data_0215957c.inc.c"
};
/* "res_wall_default", "res_wall_cathy", ... */
const char* data_021595cc[] = {
    (char*)&data_02159318, (char*)&data_02159254, (char*)&data_02159174,
    (char*)&data_021591a4, (char*)&data_021591b4, (char*)&data_021592d4,
    (char*)&data_02159294, (char*)&data_02159368, (char*)&data_02159234,
    (char*)&data_02159274, (char*)&data_02159194, (char*)&data_02159354,
    (char*)&data_02159204, (char*)&data_02159184, (char*)&data_021592b4,
    (char*)&data_02159224, (char*)&data_021592a4, (char*)&data_021591e4,
    (char*)&data_02159244, (char*)&data_0215932c, (char*)&data_02159214,
    (char*)&data_021592c4, (char*)&data_02159284, (char*)&data_02159340,
    (char*)&data_021592e4, (char*)&data_021592f4, (char*)&data_021591f4,
    (char*)&data_02159264,
    /* the extra wallpapers, from WALLPAPER_EXTRA_FIRST */
    (char*)&data_021591d4, (char*)&data_021591c4, (char*)&data_02159304};
