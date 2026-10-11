#include "types.h"
#include "game/map_select.h"
#include "game/menu_button.h"

int func_0207bd6c(void);
int func_0206b608(int player);
u32 func_02073874(void);
BOOL Skills_AreDisabled(void);

extern MenuButtonTemplate data_0215cb20;
extern MenuButtonTemplate data_0215cb48;
extern MenuButtonTemplate data_0215cb70;
extern MenuButtonTemplate data_0215cb98;
extern MenuButtonTemplate data_0215cd50;
extern MenuButtonTemplate data_0215cda0;
extern MenuButtonTemplate data_0215cdf0;
extern MenuButtonTemplate data_0215ce40;

static inline BOOL IsFirstLinkPlayer(void)
{
    return func_0207bd6c() == 0;
}

/* Adds the Vs setup buttons of a link game for this machine's player slot:
 * second map (id player * 2 + 1, with twoMaps), the two CO buttons (8 +
 * player * 2 and + 1), buttons 250-253 (250-252 only when skills are on)
 * and the four buttons above and below the COs (32 + player * 4 to + 3),
 * plus 18 + player * 4 and + 1 with twoMaps. The first link player also gets the CO buttons of each
 * slot not set in the func_02073874 mask. Other players close button 254. */
void VsSetup_AddLinkButtons(int player)
{
    int other;

    if (STATE->twoMaps == 1) {
        MenuButton_Add(func_0206b608(player) * 8 + 28, 24, &data_0215cb20, player * 2 + 1);
    }
    MenuButton_Add(func_0206b608(player) * 8, 80, &data_0215cb98, player * 2 + 8);
    MenuButton_Add(func_0206b608(player) * 8 + 24, 80, &data_0215cb98, player * 2 + 9);
    if (IsFirstLinkPlayer() == 1) {
        for (other = 0; other < MENU->setup.unk_2e3; other++) {
            if ((func_02073874() & (1 << other)) == 0) {
                MenuButton_Add(func_0206b608(other) * 8, 80, &data_0215cb98, other * 2 + 8);
                MenuButton_Add(func_0206b608(other) * 8 + 24, 80, &data_0215cb98, other * 2 + 9);
            }
        }
    }
    MenuButton_Add(0, 0, &data_0215ce40, 253);
    if (!Skills_AreDisabled()) {
        MenuButton_Add(0, 0, &data_0215cd50, 252);
        MenuButton_Add(56, 152, &data_0215cda0, 251);
        MenuButton_Add(152, 152, &data_0215cb48, 250);
    }
    if (STATE->twoMaps == 1) {
        MenuButton_Add(func_0206b608(player) * 8 + 28, 8, &data_0215cdf0, player * 4 + 18);
        MenuButton_Add(func_0206b608(player) * 8 + 28, 40, &data_0215cb70, player * 4 + 19);
    }
    MenuButton_Add(func_0206b608(player) * 8 + 4, 64, &data_0215cdf0, player * 4 + 32);
    MenuButton_Add(func_0206b608(player) * 8 + 4, 128, &data_0215cb70, player * 4 + 33);
    MenuButton_Add(func_0206b608(player) * 8 + 28, 64, &data_0215cdf0, player * 4 + 34);
    MenuButton_Add(func_0206b608(player) * 8 + 28, 128, &data_0215cb70, player * 4 + 35);
    if (IsFirstLinkPlayer() == 1) {
        for (other = 0; other < MENU->setup.unk_2e3; other++) {
            if ((func_02073874() & (1 << other)) == 0) {
                MenuButton_Add(func_0206b608(other) * 8 + 4, 64, &data_0215cdf0, other * 4 + 32);
                MenuButton_Add(func_0206b608(other) * 8 + 4, 128, &data_0215cb70, other * 4 + 33);
                MenuButton_Add(func_0206b608(other) * 8 + 28, 64, &data_0215cdf0, other * 4 + 34);
                MenuButton_Add(func_0206b608(other) * 8 + 28, 128, &data_0215cb70, other * 4 + 35);
            }
        }
    }
    if (IsFirstLinkPlayer() == 0) {
        MenuButton_Close(254);
    }
}
