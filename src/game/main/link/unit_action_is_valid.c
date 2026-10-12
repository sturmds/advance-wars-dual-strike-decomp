#include "types.h"
#include "game/unit_action.h"

/* Actions are numbered 1 to 27; 0 marks an empty entry. */
int UnitAction_IsValid(UnitAction* action) {
  if (action->action == 0 || action->action >= 28) {
    return 0;
  }
  return 1;
}
