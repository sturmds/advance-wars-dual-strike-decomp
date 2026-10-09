#include "types.h"
#include "game/menu_button.h"

int func_02026404(MenuButton *entry, int mask);
const void *func_01ff8000(int width, int height);
void func_01ff8748(int attr1, int attr0, const void *cells, int attr2, int tile);

/* Puts the button's sprite into OAM. Bit 10 of OAM attribute 0 selects
 * semi-transparent mode. */
void MenuButton_Draw(MenuButton *button)
{
    u16 tile;
    u16 palette;
    s16 width;
    s16 height;
    int mode;

    tile = button->tile;
    palette = button->palette;
    width = button->width;
    height = button->height;
    mode = 0;
    if (func_02026404(button, MENU_BUTTON_VISIBLE) == 0) {
        return;
    }
    if (func_02026404(button, MENU_BUTTON_SEMI_TRANSPARENT) != 0) {
        mode = 0x400;
    }
    func_01ff8748(button->x & 0x1ff, (button->y & 0xff) | mode, func_01ff8000(width, height),
                  palette << 12, tile);
}
