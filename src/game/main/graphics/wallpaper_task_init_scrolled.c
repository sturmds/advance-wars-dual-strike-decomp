#include "types.h"
#include "game/wallpaper_task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

extern Unk_02080440* data_027c027c;
extern Unk_027c0288 data_027c0288;

/* Resets the frame counter. scroll2 starts at 192 on the screen whose state
 * is data_027c0288.unk_00, otherwise at 0. Nothing calls this yet; guess:
 * a task script step. */
void WallpaperTask_InitScrolled(WallpaperTask* task) {
  task->frame = 0;
  if (data_027c027c == data_027c0288.unk_00) {
    task->scroll2 = 192;
  } else {
    task->scroll2 = 0;
  }
}
