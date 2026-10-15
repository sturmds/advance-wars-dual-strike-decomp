#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/mode_font_task.h"
#include "sdk/msl.h"
#include "game/screen_other_map.h"

extern const TaskScriptCmd data_02159870[];

/* Like ModeFont_ShowTextOnScreen with ModeFont_StartTextTask inlined, but
 * the task uses its own script (data_02159870) and draws at y = 48, below
 * the first line. func_0202a7c0 destroys both tasks. */
void ModeFont_ShowSecondLine(const char* text, int tile, int paletteSlot,
                             int unk_56, u8 unk_5c, int unk_58) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;
  ModeFontTask* task;
  int i;
  u16 width;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  task = (ModeFontTask*)Task_Create(data_02159870, TASK_LIST(3));
  task->length = strlen(text);
  width = 0;
  for (i = 0; i < task->length; i++) {
    s8 c = text[i];

    switch (c) {
      case '*':
        task->glyphs[i] = 26;
        break;
      case '-':
        task->glyphs[i] = 27;
        break;
      default:
        if (c >= 'a' && c <= 'z') {
          task->glyphs[i] = c - 'a';
        } else if (c >= 'A' && c <= 'Z') {
          task->glyphs[i] = c - 'A';
        } else {
          task->glyphs[i] = MODE_FONT_SPACE;
        }
        break;
    }
    if (task->glyphs[i] == MODE_FONT_SPACE) {
      width += 16;
    } else {
      width += data_02159814[task->glyphs[i]];
    }
  }
  width = width - (task->length - 1) * 2;
  task->x = 128 - (width >> 1);
  task->y = 48;
  task->tile = tile;
  task->paletteSlot = paletteSlot;
  task->unk_5c = unk_5c;
  task->unk_56 = unk_56;
  task->unk_58 = unk_58;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
