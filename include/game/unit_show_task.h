#ifndef GAME_UNIT_SHOW_TASK_H
#define GAME_UNIT_SHOW_TASK_H

#include "types.h"
#include "game/map_unit.h"

#define UNIT_SHOW_SLOT_COUNT \
  2 /* show tasks that can run at once (Unk_02080440.unk_58) */

#define UNIT_SHOW_PATH_LENGTH 16

/* Sprite handle of a UnitShowTask (SpriteAnim_Create makes it from a loaded file). */
typedef struct ShowSprite {
  u8 pad_00[0x20];
  u8 flags; /* 0x20 set for the first screen's 3D sprite */
  u8 pad_21[0x24 - 0x21];
  u32 unk_24; /* palette or layer word the task sets */
  u32 unk_28;
  void* unk_2c; /* the task's second loaded file */
} ShowSprite;

/* Task made by UnitShowTask_Create for a unit the AI is showing. The first
 * 0x2c bytes belong to the task system (Task_Create). */
typedef struct UnitShowTask {
  const void* taskScript; /* Task.script; 0 once the task has been freed */
  u8 pad_04[0x2c - 0x04];
  ShowSprite* sprite; /* 2D or 3D sprite handle */
  MapUnit* unit; /* flags have SELECTED and LOCKED set while the task lives */
  u8 scriptPos;  /* next byte of script */
  u8 state; /* UNIT_SHOW_STATE_*, picks the handler in data_ov000_022db6b4 */
  u8 entry; /* unit type, index into gUnitShowEntries */
  u8 owner; /* player slot minus 1 of the army whose colour the sprite wears */
  s8 frame; /* sprite frame last set with SpriteAnim_SetAnimation */
  u8 unk_39;
  s8 slot; /* show slot (flag in Unk_02080440.unk_58) the task holds */
  u8 pad_3b;
  u16 waitFrames; /* UnitShowTask_RunScript runs only while this is 0 */
  s16 stepSize;   /* pixels per move step, from UnitShowEntry_GetStepSize */
  s16 x;          /* pixels: tile * 16 */
  s16 y;
  void* unk_44;    /* second loaded file */
  void* unk_48;    /* first loaded file */
  s8 script[0x20]; /* commands of UnitShowTask_RunScript */
} UnitShowTask;

/* path: UNIT_SHOW_PATH_LENGTH signed steps, read as bytes */
void UnitShowTask_SetPath(UnitShowTask* task, const u8* path);

/* UnitShowTask.state values */
#define UNIT_SHOW_STATE_NONE 0 /* no handler: the task runs its script first */
#define UNIT_SHOW_STATE_IDLE 1 /* set by UnitShowTask_Spawn */
#define UNIT_SHOW_STATE_MOVE 2
#define UNIT_SHOW_STATE_DELAY 3
#define UNIT_SHOW_STATE_SCRIPT 4
#define UNIT_SHOW_STATE_DONE 5 /* script command 10 */

void ShowEntry_PlaySound(UnitShowTask* task);
void ShowEntry_Finish(UnitShowTask* task);
void UnitShowTask_PlayStepSound(UnitShowTask* task);
void UnitShowTask_PlayEntrySound(UnitShowTask* task);

#endif
