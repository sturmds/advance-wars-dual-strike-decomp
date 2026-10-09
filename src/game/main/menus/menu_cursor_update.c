#include "types.h"
#include "game/menu_button.h"

int func_01ff9f7c(void);
void func_01ff8850(int x, int y, void *shape, int tileAndPalette);
int MenuButton_TestFlags(MenuButton *entry, int mask);

extern u8 data_0216e4d0;
extern u8 data_02158f3c[];
extern u8 data_02158f44[16];

#define ABS(x) ((x) < 0 ? -(x) : (x))
#define EASE(cur, target) (ABS((target) - (cur)) <= 1 ? (target) : (cur) + (((target) - (cur)) >> 1))

/* Runs the menu cursor each frame: each corner moves half the way to the
 * matching corner of the selected button (or jumps there while
 * data_0216e4d0 is set), then the four corner sprites are drawn if the
 * button has flag 1. On frames where func_01ff9f7c() & 0x30 is 0 the
 * corners shake by offsets from data_02158f44. */
void MenuCursor_Update(void)
{
    MenuButton *button;
    int i;
    int mode;
    int left;
    int top;
    int right;
    int bottom;

    mode = 0;
    MenuButton_Select(data_02158f38->selectedId);
    if (!MenuCursor_HasSelection()) {
        return;
    }
    button = data_02158f38->selected;
    left = button->x;
    top = button->y;
    right = left + button->width;
    bottom = top + button->height;
    if (data_0216e4d0 == 0) {
        data_02158f38->corners[0][0] = EASE(data_02158f38->corners[0][0], left);
        data_02158f38->corners[0][1] = EASE(data_02158f38->corners[0][1], top);
        data_02158f38->corners[1][0] = EASE(data_02158f38->corners[1][0], right);
        data_02158f38->corners[1][1] = EASE(data_02158f38->corners[1][1], top);
        data_02158f38->corners[2][0] = EASE(data_02158f38->corners[2][0], left);
        data_02158f38->corners[2][1] = EASE(data_02158f38->corners[2][1], bottom);
        data_02158f38->corners[3][0] = EASE(data_02158f38->corners[3][0], right);
        data_02158f38->corners[3][1] = EASE(data_02158f38->corners[3][1], bottom);
    } else {
        /* needed to match: the original sign-extends each value once */
        s16 l = left;
        s16 t = top;
        s16 r = right;
        s16 b = bottom;
        data_02158f38->corners[0][0] = l;
        data_02158f38->corners[0][1] = t;
        data_02158f38->corners[1][0] = r;
        data_02158f38->corners[1][1] = t;
        data_02158f38->corners[2][0] = l;
        data_02158f38->corners[2][1] = b;
        data_02158f38->corners[3][0] = r;
        data_02158f38->corners[3][1] = b;
    }

    button = MenuButton_Find(data_02158f38->selectedId);
    if (MenuButton_TestFlags(button, 1) == 0) {
        return;
    }
    if (MenuButton_TestFlags(button, 0x800) != 0) {
        mode = 0x400;
    }
    for (i = 0; i < 4; i++) {
        if (func_01ff9f7c() & 0x30) {
            func_01ff8850(data_02158f38->corners[i][0] & 0x1ff,
                          mode | (data_02158f38->corners[i][1] & 0xff), data_02158f3c,
                          (data_02158f38->tile + i * 2) | (data_02158f38->palette << 12));
        } else {
            /* left and top now hold the shake offsets; separate locals
             * get worse register allocation */
            left = data_02158f44[func_01ff9f7c() & 15];
            top = data_02158f44[func_01ff9f7c() & 15];
            if (i == 1 || i == 3) {
                left = -left;
            }
            if (i == 2 || i == 3) {
                top = -top;
            }
            func_01ff8850((left + data_02158f38->corners[i][0]) & 0x1ff,
                          mode | ((top + data_02158f38->corners[i][1]) & 0xff), data_02158f3c,
                          (data_02158f38->tile + i * 2) | (data_02158f38->palette << 12));
        }
    }
}
