#include "types.h"
#include "game/Unk_02080440.h"
#include "game/menu_item.h"
#include "game/event_script.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u32 gLinkPlay;
extern EventScriptCommand gSendNoSlotScript[];
extern EventScriptCommand gSendNoTileScript[];

void func_020b9d48(void);
void func_0201e954(void);
void ActingUnit_FinishAction(void);
void UnitAction_MoveToOtherMap(void);
BOOL Unit_LacksOtherMapSlots(MapUnit* unit);
int MapScript_RunList10(int id, void* unit);
void UnitAction_Send(int action, int unitIndex, u8 target, u8 arg);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Handler of the unit menu's Send entries (send the acting unit to the other
 * map). On the greyed-out entry it starts a script that tells why: no free
 * unit slot there, or no tile for the unit. Otherwise it sends the unit, sends
 * unit action 26 when gLinkPlay is set, and runs map script list 10, id 46. */
void UnitMenu_Send(s8 index, s8 selected, s8 flags) {
  if (flags & MENU_ITEM_GREYED) {
    if (Unit_LacksOtherMapSlots(STATE->actingUnit) == 1) {
      EventScript_Start(gSendNoSlotScript);
    } else {
      EventScript_Start(gSendNoTileScript);
    }
    return;
  }
  func_020b9d48();
  func_0201e954();
  ActingUnit_FinishAction();
  UnitAction_MoveToOtherMap();
  if (gLinkPlay != 0) {
    UnitAction_Send(26, STATE->actingUnitIndex, 0, 0);
  }
  MapScript_RunList10(46, NULL);
}
