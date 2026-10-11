#include "types.h"
#include "game/event_script.h"

int func_020d82c8(void);
void func_02103ed8(int campaignSlot, int day);
int func_02019330(int mode, int campaignSlot);
void func_020d8028(int arg);
void func_02018f48(void);
void func_02019484(u8 recordId);

/* Event script handler. The record id in slot->unk_10 (set by
 * EventSequence_StartScript from func_02019330) picks the work: 0 runs
 * func_02018f48; 2 to 4 (campaign records) run func_02103ed8 with day 0 for
 * the campaign slot, then func_02019484 on that slot's record id; any other
 * id goes straight to func_02019484. What func_02019484 does with the record
 * is not clear yet (it may load rather than save). */
void EventSequence_RunRecordStep(EventScriptSlot *slot)
{
    int campaignSlot;

    func_020d8028(slot->unk_10);
    switch (slot->unk_10) {
    case 0:
        func_02018f48();
        break;
    case 2:
    case 3:
    case 4:
        campaignSlot = func_020d82c8();
        func_02103ed8(campaignSlot, 0);
        func_02019484(func_02019330(0, campaignSlot));
        break;
    default:
        func_02019484(slot->unk_10);
        break;
    }
}
