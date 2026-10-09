#include "types.h"
#include "game/map_header.h"
#include "game/menu_state.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

s32 HwDiv32(s32 numerator, s32 denominator);
s32 HwMod32(s32 numerator, s32 denominator); /* remainder, hardware divider */
int func_01ff9f7c(void);
void func_02001f50(int x, int y, int attr, int tile, int a4);
void func_02003188(const void *src, int slideOffset, int size);
void func_0200acac(void *state);
void func_02021858(int a0, int x, int y, void *a3, int attr, int tile);
void func_020218e4(int a0, int x, int y, void *a3, int attr, int tile);
int func_0202795c(int from, int to, int step, int steps);
void func_0202b89c(s16 x, int y, int a2, int a3, int a4);
void func_0205f03c(void);
void func_0205f0ec(void);
void func_02060718(int x, int y, int a2);
void func_02060bf8(int x, int y, int list);
void func_02062208(void);
void func_0206291c(void);
void func_020636d8(int x, int y, int a2);
void func_02063868(int x, int y);
void func_02063958(int x, int y, int list);
void func_02063e04(int list, int a1);
int func_020642e8(void);
int func_0207bd6c(void);
int func_0207bd80(void);
int Map_GetPlayerCount(int mapId);

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;
extern MenuState gMenuState;
extern MapListTables data_0229c768;
extern u8 data_027c0000;
extern u8 data_027c0028;
extern u8 data_027c0040;
extern u8 data_027c00b8;
extern u8 data_027c0108;

#define STATE (data_027c027c->unk_48[data_027c0284])
/* Needed to match: through the cast the compiler keeps field offsets out of
 * the literal pool, as the original does. */
#define MENU ((MenuState *)&gMenuState)
#define TABLES data_0229c768
/* Current tab of the menu, 0-3; each tab has its own map list. */
#define TAB() func_020642e8()
#define LIST_OF_TAB(tab) (TABLES.listOfTab[tab])
#define LIST(tab) (MENU->setup.lists[LIST_OF_TAB(tab)])
/* Writes value to the X scroll of the BG used by layer bg, then copies
 * it to the BG of layer copyBg. */
#define SET_SCROLL(bg, copyBg, value)                                   \
    {                                                                   \
        u8 from = data_027c0280->bg;                                    \
        u8 to = data_027c0280->copyBg;                                  \
        data_027c0280->bgScrollX[from] = (value);                       \
        data_027c0280->bgScrollX[to] = data_027c0280->bgScrollX[from]; \
    }
/* Six values in a 3 by 2 grid: an icon (grey when the value is 0), then
 * the value in up to two digits under it. */
#define DRAW_NUMBER_GRID(values, y)                                                             \
    for (i = 0; i < 6; i++) {                                                                   \
        func_020218e4(1, (HwMod32(i, 3) * 21 + 4) & 0x1ff, (HwDiv32(i, 3) * 28 + (y)) & 0xff, \
                      &data_027c0000, ((values)[i] != 0 ? 10 : 11) << 12, 124 + i * 8);         \
        value = (values)[i];                                                                    \
        if (value > 0) {                                                                        \
            int ones = HwMod32(value, 10);                                                \
            func_020218e4(0, (HwMod32(i, 3) * 21 + 12) & 0x1ff,                           \
                          (HwDiv32(i, 3) * 28 + (y) + 24) & 0xff, &data_027c0028, 0x3000,       \
                          ones * 2 + 104);                                                      \
            value = HwDiv32(value, 10);                                                         \
            if (value != 0) {                                                                   \
                func_020218e4(0, (HwMod32(i, 3) * 21 + 4) & 0x1ff,                        \
                              (HwDiv32(i, 3) * 28 + (y) + 24) & 0xff, &data_027c0028, 0x3000,   \
                              value * 2 + 104);                                                 \
            }                                                                                   \
        }                                                                                       \
    }

/* Map list layout: COLS by ROWS maps per page. */
#define COLS (STATE->gameMode == 1 ? 7 : (STATE->unk_8a == 0 ? 9 : 8))
#define ROWS (STATE->gameMode == 1 ? 1 : 3)
/* Page of the selected entry; the map's index is page * ROWS * COLS plus its
 * position on the page. */
#define PAGE() (TABLES.unk_0d0[TAB()][LIST(TAB()).unk_64])
#define SELECTED_MAP_ID() \
    (LIST(TAB()).mapIds[ \
        TABLES.unk_000[TAB()][LIST(TAB()).unk_64][PAGE()] + PAGE() * (ROWS * COLS)])

/* Draws one frame of the map select menu. data_027c0288 holds a draw state
 * per screen; func_0200acac switches between them. While slideFrames runs
 * (15 down to 1) the lists slide sideways, eased from 256 pixels to 0; the
 * second slide does the same for the strip at TABLES.unk_108. Then: the map
 * list thumbnails, a marker per player for the selected map's player count,
 * and the number grids. In gameMode 1 two palette colours cycle every 4
 * frames. */
void MapSelect_Draw(void)
{
    int slideOffset;
    int subSlideOffset;
    int thumbOffset;
    Unk_027c0280 *savedState;
    int count;
    Unk_027c0280 *savedState2;
    int i;
    int j;
    int value;

    if (MENU->setup.slideFrames != 0) {
        if (MENU->setup.slideFrames == 15) {
            func_02063e04(LIST_OF_TAB(TAB()), PAGE());
            func_02062208();
            func_0206291c();
        }
        slideOffset = func_0202795c(256, 0, 17 - MENU->setup.slideFrames, 16);
        if (MENU->setup.slideBack != 0) {
            SET_SCROLL(layer1Bg, layer0Bg, -slideOffset);
            savedState = data_027c0280;
            func_0200acac(data_027c0288.unk_04);
            SET_SCROLL(layer2Bg, layer1Bg, -slideOffset);
            func_0200acac(savedState);
        } else {
            SET_SCROLL(layer1Bg, layer0Bg, slideOffset);
            savedState = data_027c0280;
            func_0200acac(data_027c0288.unk_04);
            SET_SCROLL(layer2Bg, layer1Bg, slideOffset);
            func_0200acac(savedState);
        }
        MENU->setup.slideFrames--;
        if (MENU->setup.slideFrames == 0) {
            func_0205f0ec();
        }
    }
    if (MENU->setup.subSlideFrames != 0) {
        subSlideOffset = func_0202795c(256, 0, 17 - MENU->setup.subSlideFrames, 16);
        MENU->setup.subSlideFrames--;
        if (MENU->setup.subSlideFrames == 0) {
            func_0205f03c();
        }
    }

    savedState = data_027c0280;
    func_0200acac(data_027c0288.unk_00);
    if (MENU->setup.slideFrames != 0) {
        if (MENU->setup.slideBack != 0) {
            thumbOffset = slideOffset >> 1;
            func_02063958(thumbOffset - 128, 8, LIST_OF_TAB(TAB()) - 2);
            func_02063958(thumbOffset, 8, LIST_OF_TAB(TAB()) - 1);
            func_02063958(thumbOffset + 128, 8, LIST_OF_TAB(TAB()));
            func_02063958(thumbOffset + 256, 8, LIST_OF_TAB(TAB()) + 1);
            if (STATE->gameMode == 1) {
                func_02060718(slideOffset, 72, 0);
                func_02060718(slideOffset - 256, 72, 1);
                func_02060bf8(slideOffset + 200, 64, LIST_OF_TAB(TAB()));
                func_02060bf8(slideOffset - 56, 64, MENU->setup.prevList);
            } else if (STATE->unk_8a == 0) {
                func_02060bf8(slideOffset + 192, 32, LIST_OF_TAB(TAB()));
                func_02060bf8(slideOffset - 64, 32, MENU->setup.prevList);
            } else {
                func_02060bf8(slideOffset + 192, 48, LIST_OF_TAB(TAB()));
                func_02060bf8(slideOffset - 64, 48, MENU->setup.prevList);
            }
        } else {
            thumbOffset = slideOffset >> 1;
            func_02063958(-thumbOffset, 8, LIST_OF_TAB(TAB()) - 1);
            func_02063958(128 - thumbOffset, 8, LIST_OF_TAB(TAB()));
            func_02063958(256 - thumbOffset, 8, LIST_OF_TAB(TAB()) + 1);
            func_02063958(384 - thumbOffset, 8, LIST_OF_TAB(TAB()) + 2);
            if (STATE->gameMode == 1) {
                func_02060718(-slideOffset, 72, 0);
                func_02060718(256 - slideOffset, 72, 1);
                func_02060bf8(200 - slideOffset, 64, LIST_OF_TAB(TAB()));
                func_02060bf8(456 - slideOffset, 64, MENU->setup.prevList);
            } else if (STATE->unk_8a == 0) {
                func_02060bf8(192 - slideOffset, 32, LIST_OF_TAB(TAB()));
                func_02060bf8(448 - slideOffset, 32, MENU->setup.prevList);
            } else {
                func_02060bf8(192 - slideOffset, 48, LIST_OF_TAB(TAB()));
                func_02060bf8(448 - slideOffset, 48, MENU->setup.prevList);
            }
        }
    } else {
        func_02063958(0, 8, LIST_OF_TAB(TAB()) - 1);
        func_02063958(128, 8, LIST_OF_TAB(TAB()));
        func_02063958(256, 8, LIST_OF_TAB(TAB()) + 1);
        if (STATE->gameMode == 1) {
            func_02060bf8(200, 64, LIST_OF_TAB(TAB()));
            func_02060718(0, 72, 0);
        } else if (STATE->unk_8a == 0) {
            func_02060bf8(192, 32, LIST_OF_TAB(TAB()));
        } else {
            func_02060bf8(192, 48, LIST_OF_TAB(TAB()));
        }
    }

    if (STATE->gameMode == 1) {
        if (MENU->setup.subSlideFrames != 0) {
            if (MENU->setup.subSlideBack != 0) {
                subSlideOffset >>= 1;
                func_020636d8(subSlideOffset - 128, 40, TABLES.unk_108 - 2);
                func_020636d8(subSlideOffset, 40, TABLES.unk_108 - 1);
                func_020636d8(subSlideOffset + 128, 40, TABLES.unk_108);
                func_020636d8(subSlideOffset + 256, 40, TABLES.unk_108 + 1);
            } else {
                subSlideOffset >>= 1;
                func_020636d8(-subSlideOffset, 40, TABLES.unk_108 - 1);
                func_020636d8(128 - subSlideOffset, 40, TABLES.unk_108);
                func_020636d8(256 - subSlideOffset, 40, TABLES.unk_108 + 1);
                func_020636d8(384 - subSlideOffset, 40, TABLES.unk_108 + 2);
            }
        } else {
            func_020636d8(0, 40, TABLES.unk_108 - 1);
            func_020636d8(128, 40, TABLES.unk_108);
            func_020636d8(256, 40, TABLES.unk_108 + 1);
        }
    } else if (STATE->unk_8a == 1) {
        func_02063868(128, 8);
    }

    if (STATE->gameMode == 1) {
        if (MENU->setup.slideFrames != 0) {
            if (MENU->setup.slideBack != 0) {
                if (LIST(TAB()).unk_64 == 12) {
                    func_0202b89c(slideOffset + 128, 120, 0x2800, 0x600, 3);
                } else if (MENU->setup.lists[MENU->setup.prevList].unk_64 == 12) {
                    func_0202b89c(slideOffset - 128, 120, 0x2800, 0x600, 3);
                }
            } else {
                if (LIST(TAB()).unk_64 == 12) {
                    func_0202b89c(128 - slideOffset, 120, 0x2800, 0x600, 3);
                } else if (MENU->setup.lists[MENU->setup.prevList].unk_64 == 12) {
                    func_0202b89c(384 - slideOffset, 120, 0x2800, 0x600, 3);
                }
            }
        } else if (LIST(TAB()).unk_64 == 12) {
            func_0202b89c(128, 120, 0x2800, 0x600, 3);
        }
    } else if (MENU->setup.unk_4fc != 0) {
        func_0202b89c(128, 112, 0x2800, 0x600, 3);
    }
    func_0200acac(savedState);

    savedState2 = data_027c0280;
    func_0200acac(data_027c0288.unk_04);
    if (MENU->setup.slideFrames != 0) {
        if (MENU->setup.slideBack != 0) {
            func_0202b89c(slideOffset + 128, 96, 0x2c00, 0x400, 3);
            func_0202b89c(slideOffset - 128, 96, 0x2c00, 0x600, 3);
        } else {
            func_0202b89c(128 - slideOffset, 96, 0x2c00, 0x400, 3);
            func_0202b89c(384 - slideOffset, 96, 0x2c00, 0x600, 3);
        }
    } else {
        func_0202b89c(128, 96, 0x2c00, 0x400, 3);
    }

    if (STATE->gameMode == 1) {
        count = Map_GetPlayerCount(SELECTED_MAP_ID());
        for (i = 0; i < count - 1; i++) {
            for (j = 0; j <= 1; j++) {
                int k = j + i * 2;
                func_02001f50(((232 - j * 32) & 0x1ff) | 0x1000, (192 - i * 16) & 0xff,
                              (k + 4) << 12, k * 12 + 16, (k & 1) + 1);
            }
        }
        func_020218e4(0, 208, (192 - count * 16) & 0xff, &data_027c00b8, 0x3000, 0);
    }

    DRAW_NUMBER_GRID(MENU->setup.unk_4ce, 128);

    /* The second grid shows on tab 2 and for a map with a second map. */
    if (TAB() == 2 || (STATE->gameMode == 1 && Ov000_gMapHeaders[SELECTED_MAP_ID()].variantMapIds[0] != 0)) {
        DRAW_NUMBER_GRID(MENU->setup.unk_4d4, 56);
    }
    func_0200acac(savedState2);

    if (STATE->gameMode == 1) {
        func_02003188(&MENU->colours[(func_01ff9f7c() >> 2) & 15], 0x238, 2);
    }
    if (func_0207bd80() == 1 && func_0207bd6c() != 0) {
        func_02021858(0, 172, 172, &data_027c0040, 0x3000, 0x5c4);
        func_02021858(0, 188, 172, &data_027c0108, 0x3000, 0x5e8);
        func_02003188(&MENU->setup.colours2[(func_01ff9f7c() >> 2) & 15], 0x274, 2);
    }
}
