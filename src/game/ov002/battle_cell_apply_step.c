#include "types.h"
#include "game/menu_state.h"

void func_ov002_0235d514(int side);

/* Needed to match: a side's HP cell entry (battle.unk_310[side][cell]) as seen
 * from the start of the menu state. The original keeps the offset 0x310 in the
 * load, not in the address. */
typedef struct CellEntryAt {
  u8 pad_000[offsetof(MenuState, battle.unk_310[0][0].fillAfter)];
  u8 fillAfter;
  u8 fillShown;
} CellEntryAt;

/* Needed to match: the first address multiplies side as int, the store as
 * unsigned (sizeof), so mwcc shares only cell * 24 between the two. */
#define CELL_ENTRY(side, cell)                                           \
  ((CellEntryAt*)((u8*)&gMenuState +                                     \
                  (side) * (int)sizeof(BattleEntry[BATTLE_CELL_COUNT]) + \
                  (cell) * (int)sizeof(BattleEntry)))
#define CELL_ENTRY_U(side, cell)                                    \
  ((CellEntryAt*)((u8*)&gMenuState +                                \
                  (side) * sizeof(BattleEntry[BATTLE_CELL_COUNT]) + \
                  (cell) * (int)sizeof(BattleEntry)))

/* Applies entry `step` of the side's step list to its HP cell: the cell shows its
 * final fill if no later entry names the same cell, and otherwise at least 1
 * when the final fill is 0. */
void BattleCell_ApplyStep(int side, int step) {
  BattleHitList* list;
  s8 cell;
  u8 isLast;
  CellEntryAt* entry;
  u8 fillAfter;

  /* Needed to match: battle.stepLists[side], with its offset added at run time. */
  list = (BattleHitList*)((u8*)&gMenuState +
                          offsetof(MenuState, battle.stepLists)) +
         side;
  cell = list->flags[step].cell;

  func_ov002_0235d514(side);
  entry = CELL_ENTRY(side, cell);
  fillAfter = entry->fillAfter;
  if (entry->fillShown == fillAfter) {
    return;
  }
  isLast = TRUE;
  for (step++; step < list->count; step++) {
    if (cell == list->flags[step].cell) {
      isLast = FALSE;
      break;
    }
  }
  if (isLast == TRUE) {
    CELL_ENTRY_U(side, cell)->fillShown = fillAfter;
  } else if (fillAfter == 0) {
    entry->fillShown = 1;
  }
}
