#include "types.h"
#include "game/menu_button.h"

void func_020b9890(void);

/* Link client side of the Combat setup screen when every row is decided:
 * closes the Ready and Unready buttons (ids 254 and 252) and sets the
 * flag that func_020b9890 sets for the screen's caller. */
void CombatSetup_ClientFinish(void)
{
    MenuButton_Close(254);
    MenuButton_Close(252);
    func_020b9890();
}
