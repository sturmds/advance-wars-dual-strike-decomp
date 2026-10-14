#ifndef GAME_BATTLE_GFX_H
#define GAME_BATTLE_GFX_H

#include "types.h"

/* A graphics file of the ov002 battle screen, one per army style (1-5), and the sprite
 * table that goes with each. The file names are short paths stored in place; an empty
 * name means the style has none. */
typedef struct BattleGfxByStyle {
  char files[5][4];      /* by army style - 1 */
  const void* tables[5]; /* by army style - 1 */
} BattleGfxByStyle;

typedef char
    BattleGfxByStyle_size_check[sizeof(BattleGfxByStyle) == 0x28 ? 1 : -1];

/* A graphics file of the ov002 battle screen and its sprite table. */
typedef struct BattleGfxFile {
  char file[4];
  const void* table;
  u8 unk_08_0 : 4; /* Battle_GetMatchupValue for matchup 0 */
  u8 unk_08_4 : 4; /* matchup 1 */
  u8 unk_09;       /* matchup 2 */
} BattleGfxFile;

typedef char BattleGfxFile_size_check[sizeof(BattleGfxFile) == 0xc ? 1 : -1];

#endif
