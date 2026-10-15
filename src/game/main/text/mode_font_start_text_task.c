#include "types.h"
#include "game/task.h"
#include "game/mode_font_task.h"
#include "sdk/msl.h"

extern const TaskScriptCmd data_02159830[];

/* Starts a task that draws text in the mode font. Letters of either case, '*'
 * and '-' have glyphs; any other character is a 16 pixel gap. The line is
 * centred on x = 128, each glyph overlapping the next by 2 pixels. */
void ModeFont_StartTextTask(const char* text, int tile, int paletteSlot,
                            int unk_56, u8 unk_5c, int unk_58) {
  ModeFontTask* task;
  int i;
  u16 width;

  task = (ModeFontTask*)Task_Create(data_02159830, TASK_LIST(3));
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
  task->y = 16;
  task->tile = tile;
  task->paletteSlot = paletteSlot;
  task->unk_5c = unk_5c;
  task->unk_56 = unk_56;
  task->unk_58 = unk_58;
}
