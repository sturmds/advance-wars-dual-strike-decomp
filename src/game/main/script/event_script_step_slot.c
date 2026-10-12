#include "types.h"
#include "game/Unk_0201d320.h"
#include "game/event_script.h"

extern Unk_0201d320 data_022977f4[];
extern EventSlotHold data_022977f8[];
extern EventSlotWait data_022977fc[];

/* Runs one command for slot index; FALSE: stop for this frame. */
typedef BOOL (*EventCmdFunc)(s16 index);

extern EventCmdFunc data_021585c0[];

/* Steps event script slot index: counts down its wait, and when the wait is
 * over and no hold callback is set, runs commands from data_021585c0 by
 * opcode until one returns FALSE. An opcode without a handler is skipped. */
void EventScript_StepSlot(s16 index)
{
    EventScriptSlot *slot = &data_022977f0[index];
    EventCmdFunc handler;

    if (slot->start == NULL) {
        return;
    }
    if (data_022977fc[index].wait != 0) {
        data_022977fc[index].wait--;
        if (data_022977fc[index].wait != 0) {
            return;
        }
    }
    if (data_022977f8[index].hold != 0) {
        return;
    }
    if (slot->start == NULL) {
        return;
    }
    if (data_022977f4[index].unk_00 == NULL) {
        return;
    }
    while (TRUE) {
        handler = data_021585c0[data_022977f0[index].pc->opcode];
        if (handler != NULL) {
            if (!handler(index)) {
                return;
            }
            /* Needed to match: an empty two-term condition here changes how
             * mwcc lays out the loop (any && or || does; found by
             * tools/permuter.py). Perhaps a check left empty in the original. */
            if (slot != NULL && slot->start != NULL) {
            }
        } else {
            data_022977f0[index].pc++;
        }
    }
}
