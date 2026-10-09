#include "types.h"

void func_02001670(int slot);   /* unloads the overlay in this slot of the overlay table, if loaded */
void func_020016bc(int slot);   /* loads the overlay in this slot, if not loaded */

void Overlay_SwapSlot1ForSlot3(void)
{
    func_02001670(1);
    func_020016bc(3);
}
