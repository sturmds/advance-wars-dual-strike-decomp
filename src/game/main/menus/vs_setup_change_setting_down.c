#include "types.h"
#include "game/menu_button.h"
#include "game/menu_state.h"
#include "game/event_queue.h"

int Link_IsActive(void);
void VsSetup_SetCo(u8 a, u8 b);
void VsSetup_SetController(u8 a, u8 b);
int VsSetup_NextCo(int index);

#define MENU ((MenuState*)&gMenuState)

/* A controller entry: asks for the other of human and AI. */
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
    EventQueue_Push((u8)((id) + 9), (u8)VsSetup_NextCo(id)); \
  } else {                                                   \
    VsSetup_SetCo((id) + 9, VsSetup_NextCo(id));             \
  }

/* Down on the Vs setup screen, or a touch on the down arrow of a button.
 * Button ids 0-7 are the controller entries and 8-15 the CO entries (index =
 * slot * 2 + main or second map); the arrows have ids 16 + 2 * n and 17 + 2 * n
 * for controller entry n (up, down), and 32 + 2 * n, 33 + 2 * n for CO entry n. A controller entry
 * switches between human and AI; a CO entry steps to another CO with
 * VsSetup_NextCo. In a link game the change goes through the event queue,
 * else it is applied at once. Event ids: controllers 1-8, COs 9-16. */
void VsSetup_ChangeSettingDown(MenuButton* button) {
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
    case 17:
    case 19:
    case 21:
    case 23:
    case 25:
    case 27:
    case 29:
    case 31:
      id = (id - 17) >> 1;
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
    case 33:
    case 35:
    case 37:
    case 39:
    case 41:
    case 43:
    case 45:
    case 47:
      id = (id - 33) >> 1;
      MenuButton_Select(id + 8);
      STEP_CO(id)
      break;
  }
}
