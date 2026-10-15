#include "types.h"
#include "game/Unk_022a7c5c.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_02080440.h"
#include "game/task.h"
#include "game/unit_show_task.h"
#include "sdk/fs.h"

char data_ov000_022dbd20[] = {
#include "data/ov000/data_ov000_022dbd20.inc.c"
};
static char data_ov000_022dbd1c[] = {
#include "data/ov000/data_ov000_022dbd1c.inc.c"
};
char data_ov000_022dbd24[] = {
#include "data/ov000/data_ov000_022dbd24.inc.c"
};
extern const TaskScriptCmd data_ov000_022db6cc[];
extern const u16 gArmyPalettes[][16];

int UnitShowTask_FindFreeSlot(void);
const char* UnitShowEntry_GetSpritePath(int type);
const char* UnitShowTask_GetPalettePath(UnitShowTask* task);
int UnitShowEntry_GetStepSize(int entry);
u32 UnitShowTask_GetObjBase(int slot);
void* File_Load(const char* path);
void Palette_Load(const void* src, u32 offset, u32 size);
void GfxQueue_AddKind2(const void* src, void* dst, u32 size);
ShowSprite* SpriteAnim_Create(void* file, int a, int b);
void SpriteAnim_SetAnimation(ShowSprite* sprite, int frame);
int UnitShowTask_CheckVisible(UnitShowTask* task);
void UnitShowTask_PlaceSprite(UnitShowTask* task);

/* Makes a show task for a unit of the given type at tile (x, y). It takes a
 * free show slot (NULL when both are taken), loads the unit's sprite files
 * into a new sprite and gives it the owner's army palette. owner is the
 * player slot minus 1. The first screen draws the sprite in 3D and the
 * other in 2D, so the palette goes to different places. frame starts at 1
 * for owners 0 and 2. */
UnitShowTask* UnitShowTask_Spawn(int x, int y, int owner, int type) {
  int slot;
  void* file;
  UnitShowTask* task;

  slot = UnitShowTask_FindFreeSlot();
  if (slot == -1) {
    return NULL;
  }
  data_027c027c->unk_58[slot] = 1;
  FS_ChangeDir(data_ov000_022dbd1c);
  file = File_Load(UnitShowEntry_GetSpritePath(type));
  task = (UnitShowTask*)Task_Create(data_ov000_022db6cc, TASK_LIST(5));
  if ((void*)data_027c027c == data_027c0288.unk_00) {
    task->sprite = SpriteAnim_Create(file, 2, 14);
    task->sprite->flags |= 0x20;
  } else {
    task->sprite = SpriteAnim_Create(file, 2, 2);
  }
  task->slot = slot;
  task->unit = NULL;
  task->state = UNIT_SHOW_STATE_IDLE;
  task->x = x << 4;
  task->y = y << 4;
  if (owner == 0 || owner == 2) {
    task->frame = 1;
  } else {
    task->frame = 0;
  }
  task->waitFrames = 0;
  task->unk_39 = 0;
  task->entry = type;
  task->stepSize = UnitShowEntry_GetStepSize(task->entry);
  task->owner = owner;
  task->unk_44 = File_Load(UnitShowTask_GetPalettePath(task));
  task->unk_48 = file;
  SpriteAnim_SetAnimation(task->sprite, task->frame);
  task->sprite->unk_2c = task->unk_44;
  if ((void*)data_027c027c == data_027c0288.unk_00) {
    task->sprite->unk_24 = UnitShowTask_GetObjBase(task->slot);
    task->sprite->unk_28 = data_022a7c5c->unk_64;
    GfxQueue_AddKind2(
        gArmyPalettes[MAP_STATE->players[task->owner + 1].armyColour - 1],
        (void*)data_022a7c5c->unk_64, 32); /* a texture palette offset */
  } else {
    task->sprite->unk_24 = (task->slot << 4) + 0x264 | 0x4000;
    Palette_Load(
        gArmyPalettes[MAP_STATE->players[task->owner + 1].armyColour - 1], 640,
        32);
  }
  if (UnitShowTask_CheckVisible(task)) {
    UnitShowTask_PlaceSprite(task);
  }
  return task;
}
