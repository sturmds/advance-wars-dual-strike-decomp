#ifndef GAME_BATTLESIDEDATA_H
#define GAME_BATTLESIDEDATA_H

#include "types.h"
#include "game/battle_gfx.h"
#include "game/battle_fighter.h"

/* Reached through BattleSideData.unk_18: a table of points, one per cell of the
 * side's HP bar in each of its two sets (index 0-5 and 6-). */
typedef struct BattleSideData_18 {
  u8 pad_00[0x0e];
  BattlePoint points[10]; /* length a guess */
} BattleSideData_18;

/* One entry of BattleSideData.effects: how a flying sprite of a side starts and moves.
 * Values are 8.8 fixed point; Battle_StartCellSprite and friends scale them by the
 * sprite's scale. */
typedef struct BattleEffectParams {
  char files
      [5][4]; /* Battle_LoadSideGraphics reads them as BattleGfxByStyle.files */
  const void* table;        /* and this as BattleGfxByStyle.tables[0] */
  BattlePoint offset;       /* start offset, scaled */
  u16 delay;                /* frames before the sprite moves */
  BattlePoint velocity;     /* scaled */
  BattlePoint acceleration; /* added to the velocity every frame */
  BattlePoint unk_26;    /* added to the point for the cell to get the step */
  BattlePoint direction; /* BattleCellSprite.direction when it starts */
  BattlePoint
      unk_2e; /* signs: which way the sprite is heading, per axis; 0: no check */
} BattleEffectParams;

typedef char
    BattleEffectParams_size_check[sizeof(BattleEffectParams) == 0x34 ? 1 : -1];

/* An entry of BattleSideData.effects that only names a graphics file: the
 * first 8 bytes of a BattleGfxFile, as Battle_LoadSideGraphics reads it. */
typedef struct BattleEffectFile {
  char file[4];
  const void* table;
} BattleEffectFile;

typedef char
    BattleEffectFile_size_check[sizeof(BattleEffectFile) == 8 ? 1 : -1];

/* The shorter record some sides have as BattleSideData.effects[8]: the
 * start sprite, of which Battle_MakeFighters reads offset and delay; the
 * same layout as the start of BattleEffectParams. */
typedef struct BattleStartSprite {
  char files[5][4];
  const void* table;
  BattlePoint offset;
  u16 delay;
} BattleStartSprite;

typedef char
    BattleStartSprite_size_check[sizeof(BattleStartSprite) == 0x20 ? 1 : -1];

/* Layout of BattleSideData.cellPositions that the fighter tasks read: positions they step through, from byte 14 of the data. */
typedef struct BattleAnimData {
  u8 pad_00[14];
  struct {
    s16 x;
    s16 y;
  } offsets[1]; /* one per step; length unknown */
} BattleAnimData;

/* Position of one HP cell on the battle screen. */
typedef struct BattleCellPos {
  s16 x;
  s16 y;
  s16 z;
} BattleCellPos;

/* Reached through BattleSideData.weapons, unk_08 and unk_10. mwcc packs bit fields
 * from the low end. The three matchup values are picked by Battle_GetAirGroundMatchup. */
typedef struct BattleSideWeapon {
  BattleGfxByStyle
      gfx; /* the HP cell sprites (BattleScreenWork.spriteTables) */
  u8 spriteIndex
      : 4; /* index argument of BattleSpriteTask_Create for the HP cells */
  u8 unk_28_hi : 4;
  u8 unk_29_lo : 4;
  u8 sameAltitude : 4; /* matchup 0 */
  u8 groundVsAir : 4;  /* matchup 1 */
  u8 airVsGround : 4;  /* matchup 2 */
  u8 unk_2b; /* index argument of BattleSpriteTask_Create, like spriteIndex */
} BattleSideWeapon;

typedef char
    BattleSideWeapon_size_check[sizeof(BattleSideWeapon) == 0x2c ? 1 : -1];

/* What BattleScreenWork.unk_27c points to, one per side of the battle in ov002. */
typedef struct BattleSideData {
  const BattleSideWeapon* weapons[2]; /* by BattleSide.usesSecondary */
  const BattleSideWeapon* unk_08[2];  /* by BattleSide.usesSecondary */
  const BattleSideWeapon* unk_10;
  const char* const* paletteFiles;    /* by BattleSide.army - 1 */
  const BattleCellPos* cellPositions; /* by cell */
  const BattleGfxFile* unk_1c[2];     /* by BattleSide.usesSecondary */
  BattleEffectParams* effects
      [9]; /* indexed by byte 9 of the side's 12-byte record in the scene; length a guess */
  u8 unk_48; /* 0: the cells step with the hit list (BattleCells_RunSequence); else the side runs a fighter task */
  u8 unk_49
      [3]; /* indexed by byte 9 of the side's 12-byte record in the scene, or by BattleSide.usesSecondary */
  const BattleSounds* sounds;
} BattleSideData;

typedef char BattleSideData_size_check[sizeof(BattleSideData) == 0x50 ? 1 : -1];

#endif
