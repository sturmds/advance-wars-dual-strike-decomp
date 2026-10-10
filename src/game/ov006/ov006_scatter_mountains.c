#include "types.h"

s32 Rng_Range(s32 min, s32 max);
int Ov006_FindRandomCell(int *x, int *y, int cell);
void Ov006_SetMapCell(int x, int y, int cell);
void func_ov006_0235a7c0(int a, int b);
void func_ov006_0235c3a0(void);
void Ov006_MarkMapBorder(void);

extern u8 data_ov006_0236d744[][30];

/* Turns 8 to 12 random plain tiles (cell 1) into mountain (cell 32) and marks each
 * with 2 in data_ov006_0236d744, a grid of 30-byte rows. */
void Ov006_ScatterMountains(void)
{
    int x;
    int y;
    int i;
    int count;

    Ov006_MarkMapBorder();
    count = Rng_Range(8, 12);
    for (i = 0; i < count; i++) {
        if (Ov006_FindRandomCell(&x, &y, 1) == 0) {
            break;
        }
        Ov006_SetMapCell(x, y, 32);
        data_ov006_0236d744[y][x] = 2;
    }
    func_ov006_0235c3a0();
    func_ov006_0235a7c0(32, 1);
}
