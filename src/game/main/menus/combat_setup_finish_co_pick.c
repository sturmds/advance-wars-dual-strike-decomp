#include "types.h"
#include "game/menu_button.h"

void func_020b9890(void);

/* Ends the CO pick phase of the Combat setup screen (link play) once every row is locked:
 * closes the Ready and Unready buttons (ids 254 and 252) and sets the
 * flag that func_020b9890 sets for the screen's caller. */
void CombatSetup_FinishCoPick(void) {
  MenuButton_Close(254);
  MenuButton_Close(252);
  func_020b9890();
}
