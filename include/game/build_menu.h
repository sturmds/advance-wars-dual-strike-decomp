#ifndef GAME_BUILD_MENU_H
#define GAME_BUILD_MENU_H

#include "types.h"

/* One row of the unit build menu: up to 14 rows, shown as one column of 7
 * or two. */
typedef struct BuildMenuEntry {
  u8 unitType; /* enum UnitType; 0 ends the list */
  u8 state;    /* BUILD_ENTRY_* */
} BuildMenuEntry;

/* BuildMenuEntry.state, set by BuildMenu_FillList from the player's funds. */
#define BUILD_ENTRY_AFFORDABLE 0
#define BUILD_ENTRY_TOO_EXPENSIVE 2

#define BUILD_MENU_ROWS 7 /* rows per column */

/* What the build menu task keeps in Task.pad_2c onwards. */
typedef struct BuildMenuVars {
  s16 unk_2c;    /* set to 0 by BuildMenu_Setup */
  s16 lastEntry; /* index of the last entry in gBuildMenuEntries */
} BuildMenuVars;

#define BUILD_MENU_VARS(task) ((BuildMenuVars*)(task)->pad_2c)

extern BuildMenuEntry gBuildMenuEntries[];
extern int gBuildMenuCount; /* entries in gBuildMenuEntries */

#endif
