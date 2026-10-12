#ifndef GAME_UNIT_ACTION_H
#define GAME_UNIT_ACTION_H

#include "types.h"

/* A unit action about to be shown, in gUnitAction. Filled by
 * AiUnit_PerformAction; func_0208fda8 copies it to data_0229d8c8 when
 * gLinkPlay is set. */
typedef struct UnitAction {
  u8 action;
  u8 unitIndex; /* index into Unk_02080440_48.units */
  u8 x;         /* destination */
  u8 y;
  u8 fromX; /* from Unk_02080440_48.actingUnitStartX */
  u8 fromY; /* from Unk_02080440_48.actingUnitStartY */
  u8 target;
  u8 unk_07;
  u32 rngState; /* gRngState when the action was made */
  u8 packed
      [6]; /* Unk_02080440_48.pathSteps after PackNibblePairs; dsd also names this address data_0229e1a4 */
  u8 fuelLeft;
  u8 pad_13;
  u32 pending; /* 1: set by AiUnit_PerformAction; AiTurn_RunQueue then moves to AI_STATE_SHOW_ACTION */
} UnitAction;

extern UnitAction gUnitAction;

#endif
