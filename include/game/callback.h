#ifndef GAME_CALLBACK_H
#define GAME_CALLBACK_H

#include "types.h"

#define CALLBACK_SLOTS 16

/* Callbacks that CallbackSlots_RunAll calls on every vblank; empty slots
 * are NULL. volatile: needed to match, CallbackSlots_RunAll reads the slot
 * again before the call. */
extern void (*volatile gCallbackSlots[CALLBACK_SLOTS])(void);
/* Callbacks that CallbackList_RunAndClear calls once, in the order
 * CallbackList_Add added them; gCallbackCount of them are set. The count
 * is volatile to match CallbackList_Add and CallbackList_RunAndClear. */
extern void (*gCallbackList[CALLBACK_SLOTS])(void);
extern volatile s16 gCallbackCount;

#endif
