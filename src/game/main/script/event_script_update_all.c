#include "types.h"
#include "game/event_script.h"
#include "game/keypad.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/dma_task.h"
#include "game/transfer_queue.h"

extern Unk_02080440* data_027c027c;
extern Unk_027c0288 data_027c0288;
extern u16 gEventScriptKeys;
extern s16 gEventScriptOwnsInput;
static u16 data_0216e048;
static u16 data_0216e044;
volatile int data_0216e04c; /* entries queued */
volatile int data_0216e050; /* bytes queued */
DmaTaskState gDmaTaskState;
TransferEntry data_0216e05c[TRANSFER_QUEUE_SIZE];

s16 func_0201d798(void);
BOOL func_020f5848(void);
BOOL func_020eedf0(void);
void EventScript_StepSlot(s16 index);

typedef void (*EventHoldFunc)(EventScriptSlot* slot);

/* Runs the event scripts for one frame, on the main map state only. Does
 * nothing while func_0201d798 is nonzero; otherwise clears gEventScriptKeys,
 * then stops if script gMapScrollScript runs (func_020f5848) or task
 * data_021690f8 exists (func_020eedf0). Each used slot runs its hold
 * callback, then EventScript_StepSlot steps it. While gEventScriptOwnsInput is set
 * (EventScript_Start sets it), the key state is overwritten with
 * gEventScriptKeys, which scripts may have set this frame, and
 * data_0216e044 and data_0216e048 are cleared. */
void EventScript_UpdateAll(void) {
  s16 i;
  EventScriptSlot* slot;

  if (data_027c027c != data_027c0288.unk_00) {
    return;
  }
  if (func_0201d798()) {
    return;
  }
  gEventScriptKeys = 0;
  if (func_020f5848()) {
    return;
  }
  if (func_020eedf0()) {
    return;
  }
  slot = data_022977f0;
  for (i = 0; i < EVENT_SCRIPT_SLOTS; i++, slot++) {
    if (slot->start != NULL) {
      if (slot->hold != 0) {
        ((EventHoldFunc)slot->hold)(slot);
      }
      EventScript_StepSlot(i);
    }
  }
  if (gEventScriptOwnsInput == 0) {
    return;
  }
  gKeypad->repeat = gEventScriptKeys;
  gKeypad->pressed = gEventScriptKeys;
  gKeypad->held = gEventScriptKeys;
  gKeypad->prevHeld = gEventScriptKeys;
  data_0216e048 = 0;
  data_0216e044 = 0;
}
