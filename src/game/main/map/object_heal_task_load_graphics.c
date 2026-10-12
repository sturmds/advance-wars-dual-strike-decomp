#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_022a7c5c.h"
#include "game/map_object.h"
#include "game/object_heal_task.h"
#include "game/task.h"

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern char data_021681d8[];
extern char data_021681e0[];
extern char data_021681e4[];
extern char data_021681e8[];
extern char data_021681ec[];
extern char data_021681f0[];
extern char data_021681f4[];

void FS_ChangeDir(const char* dir);
void File_UnpackTo(void* dest, const char* fileName);
void Palette_LoadFromFile(const char* fileName, int a, int b, int c);
void Gfx_QueueUnpackedFileCopy(const char* fileName, void* dest);
void Gfx_QueueFileCopy(const char* fileName, void* dest, int size);
void Task_Destroy(void* task);
void func_020f5878(s16 x, s16 y);

/* First step of the object heal script: loads the heal animation's graphics
 * and palette for the object kind (like func_020d8fac), calls func_020f5878
 * on the object's tile and starts the walk over the tiles in reach. A task
 * for any other kind of object ends here. */
void ObjectHealTask_LoadGraphics(ObjectHealTask* task) {
  switch (task->objectKind) {
    case MAP_OBJECT_BLACK_CRYSTAL:
      if (data_027c027c == data_027c0288.unk_00) {
        FS_ChangeDir(data_021681d8);
        Gfx_QueueUnpackedFileCopy(data_021681e0, data_022a7c5c->unk_68);
        Gfx_QueueFileCopy(data_021681e4, data_022a7c5c->unk_6c, 32);
      } else {
        FS_ChangeDir(data_021681d8);
        File_UnpackTo(data_027c0280->objVram + 0x3280, data_021681e8);
        Palette_LoadFromFile(data_021681e4, 0, 608, 32);
      }
      break;
    case MAP_OBJECT_BLACK_OBELISK:
      if (data_027c027c == data_027c0288.unk_00) {
        FS_ChangeDir(data_021681d8);
        Gfx_QueueUnpackedFileCopy(data_021681ec, data_022a7c5c->unk_68);
        Gfx_QueueFileCopy(data_021681f0, data_022a7c5c->unk_6c, 32);
      } else {
        FS_ChangeDir(data_021681d8);
        File_UnpackTo(data_027c0280->objVram + 0x3280, data_021681f4);
        Palette_LoadFromFile(data_021681f0, 0, 608, 32);
      }
      break;
    default:
      Task_Destroy(task);
      return;
  }
  func_020f5878(task->tileX, task->tileY);
  task->offsetIndex = 0;
}
