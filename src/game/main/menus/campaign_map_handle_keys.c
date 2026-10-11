#include "types.h"
#include "game/keypad.h"

void func_0204e544(int unused);
void func_0204e57c(int unused);

/* Key handler of the mission buttons: A or Start confirms, B cancels. */
void CampaignMap_HandleKeys(void)
{
    u16 keys = gKeypad->pressed;

    if (keys & (PAD_BUTTON_A | PAD_BUTTON_START)) {
        func_0204e57c(0);
    } else if (keys & PAD_BUTTON_B) {
        func_0204e544(0);
    }
}
