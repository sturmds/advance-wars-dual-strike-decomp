#include "types.h"
#include "game/Unk_02088d04.h"

typedef struct Unk_0215fed0 {
    u8 redraw;
} Unk_0215fed0;

typedef struct LinkPlayerData {
    u8 pad_00[0x14];
    u16 unk_14;                        /* one bit per level (func_0207a324), set when done */
} LinkPlayerData;

typedef struct LinkPlayerInfo {
    u8 pad_00[0x50];
    LinkPlayerData data;
} LinkPlayerInfo;

extern Unk_0215fed0 *data_0215fed0;
extern Unk_027c0280 *data_027c0280;
extern u8 gMenuState[];
extern char data_02167c88[];

void Mem_Copy32(const void *src, void *dst, u32 size);
void Gfx_RequestLayerCopy(int layer);
void func_01ff8c84(int layer);
void Number_Draw(s16 x, s16 y, u16 *map, int value, u16 a, u16 b);
void Text_PrintStringImmediate(s16 x, s16 y, u16 *map, const char *text, u16 a, u16 b);
int func_0207a324(void);
u8 func_0207a3a4(void);
LinkPlayerInfo *func_0207a3b8(u8 index);
void func_0207c434(void);
void func_020827b0(LinkPlayerData *data, int a, u16 b, int c);

/* Link play: redraws the player list. Each player with a record gets a name
 * and a "done/total" count of set bits in its record, one row every 5 tiles. */
void Link_DrawPlayerRows(void)
{
    LinkPlayerInfo *info;
    u16 *map;
    LinkPlayerData *data;
    int i;
    int bit;
    int count;

    if (data_0215fed0->redraw == 1) {
        Mem_Copy32(gMenuState, data_027c0280->layer2Map, 0x600);
        Gfx_RequestLayerCopy(2);
        func_0207c434();
        data_0215fed0->redraw = 0;
    }
    func_01ff8c84(0);
    for (i = 0; i < func_0207a3a4(); i++) {
        info = func_0207a3b8(i);
        if (info != 0) {
            data = &info->data;
            func_020827b0(data, 1, 1 + i * 5, 512 + i * 40);
            count = 0;
            for (bit = 0; bit < func_0207a324(); bit++) {
                if (data->unk_14 & (1 << bit)) {
                    count++;
                }
            }
            Number_Draw(28, 1 + i * 5, data_027c0280->layer0Map, count, 0, 0);
            Text_PrintStringImmediate(29, 1 + i * 5, data_027c0280->layer0Map, data_02167c88, 0, 0);
            map = data_027c0280->layer0Map;
            Number_Draw(30, 1 + i * 5, map, func_0207a324(), 0, 0);
        }
    }
    Gfx_RequestLayerCopy(0);
}
