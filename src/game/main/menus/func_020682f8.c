#include "types.h"
#include "game/map_select.h"
#include "game/menu_button.h"

int func_0207bd80(void);
int func_0207bd6c(void);
int Link_FindSlotOfPlayer(int player);
int func_0206b608(int player);
u32 func_02073874(void);

extern MenuButtonTemplate data_0215cb20;
extern MenuButtonTemplate data_0215cb98;

static inline BOOL IsFirstLinkPlayer(void)
{
    return func_0207bd6c() == 0;
}

/* Buttons of the Vs setup screen: per player, a map button row at y 24 (ids
 * player * 2 and + 1, two with twoMaps) and two CO buttons at y 80 (ids
 * 8 + player * 2 and + 1). Link games add the own slot, and the other
 * slots for player 0. Not matching yet: the loop counters of the first loop
 * take other registers (r7 and r9 swapped). */
void func_020682f8(void)
{
    int slot;
    int i;

    if (func_0207bd80() == 1) {
        slot = Link_FindSlotOfPlayer(func_0207bd6c()) - 1;
        if (STATE->twoMaps == 1) {
            MenuButton_Add(func_0206b608(slot) * 8 + 28, 24, &data_0215cb20, slot * 2 + 1);
        }
        MenuButton_Add(func_0206b608(slot) * 8, 80, &data_0215cb98, slot * 2 + 8);
        MenuButton_Add(func_0206b608(slot) * 8 + 24, 80, &data_0215cb98, slot * 2 + 9);
        if (IsFirstLinkPlayer() == 1) {
            for (i = 0; i < MENU->setup.unk_2e3; i++) {
                if ((func_02073874() & (1 << i)) == 0) {
                    MenuButton_Add(func_0206b608(i) * 8, 80, &data_0215cb98, i * 2 + 8);
                    MenuButton_Add(func_0206b608(i) * 8 + 24, 80, &data_0215cb98, i * 2 + 9);
                }
            }
        }
    } else {
        for (i = 0; i < MENU->setup.unk_2e3; i++) {
            if (STATE->twoMaps == 0) {
                MenuButton_Add(func_0206b608(i) * 8 + 16, 24, &data_0215cb20, i * 2);
            } else {
                MenuButton_Add(func_0206b608(i) * 8 + 4, 24, &data_0215cb20, i * 2);
                MenuButton_Add(func_0206b608(i) * 8 + 28, 24, &data_0215cb20, i * 2 + 1);
            }
            MenuButton_Add(func_0206b608(i) * 8, 80, &data_0215cb98, i * 2 + 8);
            MenuButton_Add(func_0206b608(i) * 8 + 24, 80, &data_0215cb98, i * 2 + 9);
        }
    }
}
