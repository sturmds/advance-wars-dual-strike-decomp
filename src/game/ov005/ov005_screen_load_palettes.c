#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov005_0236bb90.h"

void FS_ChangeDir(const char* dir);
void File_UnpackTo(void* dest, const char* fileName);
void File_ReadTo(void* dest, const char* fileName);
void Mem_Zero32(u32 value, void* dst, u32 size);
void Palette_Flush(void);

extern const TaskScriptCmd data_ov005_0236960c[];
extern char data_ov005_0236988c[];
extern char data_ov005_023698c0[];
extern char data_ov005_023698c4[];
extern char data_ov005_023698c8[];

/* Loads the BG2 tiles of the main screen plus two palette files into the large work
 * block, copies palette sets into the output columns (set 0 twice) and starts the
 * task data_ov005_0236960c. */
void Ov005Screen_LoadPalettes(Unk_ov005_0236bb90* work) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  work->unk_834c = 64;
  work->unk_8350 = 0;
  work->unk_8354 = 0;
  work->unk_8358 = 0;
  work->unk_835c = 0;

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  FS_ChangeDir(data_ov005_0236988c);
  File_UnpackTo(
      data_027c0280->bgVram +
          (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14),
      data_ov005_023698c0);
  File_UnpackTo(work->palettes, data_ov005_023698c4);
  File_ReadTo(work->unk_8360, data_ov005_023698c8);
  Mem_Zero32(0, data_027c0280->paletteBuffer, 0x80);
  Palette_Flush();
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  Ov005Work_CopyPaletteSet(work, 0, 1);
  Ov005Work_CopyPaletteSet(work, 1, 2);
  Ov005Work_CopyPaletteSet(work, 2, 3);
  Ov005Work_CopyPaletteSet(work, 3, 4);
  Ov005Work_CopyPaletteSet(work, 0, 5);
  work->unk_8348 = Task_Create(data_ov005_0236960c, TASK_LIST(3));
}
