#ifndef GAME_BATTLESPRITETASK_H
#define GAME_BATTLESPRITETASK_H

#include "types.h"

/* Task that draws one object of the battle animation in ov002. The first
 * 0x2c bytes belong to the task system (Task_Create). The creators take the
 * value of each field as an argument, named after the field (unk_5a picks the
 * task list: 5 when nonzero, else 4). */
typedef struct BattleSpriteTask {
  u8 pad_00[0x2c];
  u32 unk_2c; /* first word of the table the task is made from */
  u32 unk_30; /* word at index + 1 of the same table */
  u32 frame;  /* frame being drawn, from the unk_2c table; 0: none */
  union {
    const u16* script; /* script position */
    struct {
      s16 unk_38; /* fighter sprites: position read by the battle screen */
      s16 unk_3a;
    };
  };
  s16 unk_3c;
  s16 unk_3e;
  s16 unk_40;
  s16 unk_42;
  union {
    s16 unk_44;
    u16 scale; /* 8.8 fixed point */
  };
  s16 unk_46;
  u16 unk_48;
  u16 unk_4a;
  u16 unk_4c;
  u16 unk_4e;      /* 0x100 when made by BattleSpriteTask_CreateDefault */
  u16 unk_50;      /* same */
  u16 entryFrames; /* frames the current script entry has been shown */
  u16 unk_54;
  u8 unk_56;
  u8 unk_57;
  union {
    u8 unk_58;
    struct {
      u8 unk_58_flag : 1;
      u8 kind : 7;
    };
  };
  union {
    u8 unk_59;
    struct {
      u8 unk_59_lo : 4;
      u8 side : 1; /* battle side the sprite belongs to */
      u8 unk_59_hi : 3;
    };
  };
  u8 unk_5a;
  u8 unk_5b; /* starts at 0xff */
  u8 unk_5c; /* starts at 0 */
  u8 unk_5d; /* starts at 1 */
} BattleSpriteTask;

void BattleSpriteTask_Init(BattleSpriteTask* task, const u32* table, int index,
                           int unk_3c, int unk_3e, int unk_40, int unk_48,
                           int unk_4a, int unk_4c, int unk_4e, int unk_50,
                           int unk_54, int unk_56, int unk_59, int unk_5a,
                           int unk_57, int unk_42, int unk_44, int unk_46,
                           int unk_58);

BattleSpriteTask* BattleSpriteTask_Create(const u32* table, int index,
                                          int unk_3c, int unk_3e, int unk_40,
                                          int unk_48, int unk_4a, int unk_4c,
                                          int unk_4e, int unk_50, int unk_54,
                                          int unk_56, int unk_59, int unk_5a,
                                          int unk_57, int unk_42, int unk_44,
                                          int unk_46, int unk_58);

/* Like Create with unk_4e and unk_50 set to 0x100, unk_42 to unk_46 to 0 and
 * unk_58 to unk_56. The last five arguments are not used; the wrapper below
 * passes them. */
BattleSpriteTask* BattleSpriteTask_CreateDefault(
    const u32* table, int index, int unk_3c, int unk_3e, int unk_40, int unk_48,
    int unk_4a, int unk_4c, int unk_54, int unk_56, int unk_59, int unk_5a,
    int unused_0, int unused_1, int unused_2, int unused_3, int unused_4);

/* Same as CreateDefault. The extra arguments after unk_5a are ignored. */
BattleSpriteTask* BattleSpriteTask_CreateDefaultWrapper(
    const u32* table, int index, int unk_3c, int unk_3e, int unk_40, int unk_48,
    int unk_4a, int unk_4c, int unk_54, int unk_56, int unk_59, int unk_5a);

/* Create, with no task when table is NULL; uses another script. */
void BattleSpriteTask_CreateIfSet(const u32* table, int index, int unk_3c,
                                  int unk_3e, int unk_40, int unk_48,
                                  int unk_4a, int unk_4c, int unk_4e,
                                  int unk_50, int unk_54, int unk_56,
                                  int unk_59, int unk_5a, int unk_57,
                                  int unk_42, int unk_44, int unk_46,
                                  int unk_58);

/* Per-frame callback: draws the task's frame and steps its script. */
void BattleSpriteTask_Update(BattleSpriteTask* task);

#endif
