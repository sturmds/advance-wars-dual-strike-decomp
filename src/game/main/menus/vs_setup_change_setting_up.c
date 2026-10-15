#include "types.h"
#include "game/menu_button.h"
#include "game/menu_state.h"
#include "game/event_queue.h"

int Link_IsActive(void);
void VsSetup_SetCo(u8 a, u8 b);
void VsSetup_SetController(u8 a, u8 b);
int VsSetup_PrevCo(int index);

#define MENU ((MenuState*)&gMenuState)

/* needed to match: the u8 casts on EventQueue_Push arguments */
#define TOGGLE_CONTROLLER(id)                                        \
  value = MENU->setup.controllers[(id) >> 1][(id) & 1] == 1 ? 2 : 1; \
  if (Link_IsActive() == 1) {                                        \
    EventQueue_Push((u8)((id) + 1), (u8)value);                      \
  } else {                                                           \
    VsSetup_SetController((id) + 1, value);                          \
  }
#define STEP_CO(id)                                          \
  if (Link_IsActive() == 1) {                                \
    EventQueue_Push((u8)((id) + 9), (u8)VsSetup_PrevCo(id)); \
  } else {                                                   \
    VsSetup_SetCo((id) + 9, VsSetup_PrevCo(id));             \
  }

/* Up on the Vs setup screen, or a touch on an up arrow; like the down
 * handler, but a CO entry steps the other way with VsSetup_PrevCo. */
void VsSetup_ChangeSettingUp(MenuButton* button) {
  int id = button->id;
  int value;
  int slot;

  switch (id) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
      TOGGLE_CONTROLLER(id)
      break;
    case 16:
    case 18:
    case 20:
    case 22:
    case 24:
    case 26:
    case 28:
    case 30:
      id = (id - 16) >> 1;
      MenuButton_Select(id);
      TOGGLE_CONTROLLER(id)
      break;
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
      slot = id - 8;
      STEP_CO(slot)
      break;
    case 32:
    case 34:
    case 36:
    case 38:
    case 40:
    case 42:
    case 44:
    case 46:
      id = (id - 32) >> 1;
      MenuButton_Select(id + 8);
      STEP_CO(id)
      break;
  }
}
