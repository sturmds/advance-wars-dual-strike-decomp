#include "types.h"
#include "game/task.h"
#include "game/sprite_task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern const TaskScriptCmd gSpriteTaskScript[];
extern const char data_0216074c[];

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void* File_Load(const char* path);
void Mem_Free(void* file);
void Mem_Copy32(const void* src, void* dst, u32 size);
void Palette_Load(const void* src, u16 offset, u32 size);

/* Starts the gSpriteTaskScript task for the first game state in data_027c0288, then loads its 0x200-byte
 * picture and 16-colour palette into the second state's object VRAM and palette slot paletteSlot + 16. */
void SpriteTask_Create(int x, int y, int tile, int paletteSlot) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;
  Unk_027c0280* savedGfx2;
  Unk_02080440* savedState2;
  SpriteTask* task;
  u8* file;

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  task = (SpriteTask*)Task_Create(gSpriteTaskScript, TASK_LIST(3));
  task->x = x;
  task->y = y;
  task->tile = tile;
  task->paletteSlot = paletteSlot;

  file = File_Load(data_0216074c);
  savedGfx2 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedState2 = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  Mem_Copy32(file, data_027c0280->objVram + tile * 32, 0x200);
  Palette_Load(file + 0x200, (paletteSlot + 16) * 32, 32);
  Screen_SetCurrent(savedState2);
  Screen_SetCurrentGfx(savedGfx2);
  Mem_Free(file);
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
}
