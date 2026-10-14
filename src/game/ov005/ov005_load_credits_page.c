#include "types.h"
#include "game/Unk_ov005_0236bb90.h"

u32 strlen(const char* str);

extern Ov005Menu* data_ov005_0236a418[];

Ov005Menu* Ov005_LoadCreditsPage(Unk_ov005_0236bb90* work, int menuIndex) {
  Ov005Menu* menu = data_ov005_0236a418[menuIndex];
  int y;
  int i;
  int slot;
  const char* text;
  u16 length;
  Ov005MenuItem* item;
  int height;

  if (menu == NULL) {
    return menu;
  }

  for (i = 0; i < OV005_SLIDE_ENTRIES; i++) {
    work->entriesA[i].id = 0;
    work->entriesA[i].textLength = 0;
    work->entriesA[i].text = NULL;
    work->entriesA[i].unk_08 = 0;
    work->entriesA[i].kind = 0;
    work->entriesA[i].angle = 0;
    work->entriesA[i].offsetX = 0;
    work->entriesA[i].pos = 0;
    work->entriesA[i].unk_12 = 0;
    work->entriesA[i].slideTask = NULL;
  }

  work->unk_8014 = menu->unk_30;

  item = menu->items;
  i = 0;
  slot = 0;
  while (1) {
    if (i >= OV005_MENU_ITEMS) {
      break;
    }
    if (item->kind != 0) {
      text = item->text;
      length = strlen(text);

      work->entriesA[slot].kind = item->kind;
      work->entriesA[slot].id = 1;
      work->entriesA[slot].textLength = length;
      work->entriesA[slot].text = text;
      work->entriesA[slot].angle = 0xb000;
      work->entriesA[slot].unk_08 = slot * 64;
      work->entriesA[slot].offsetX = -((length << 3) >> 1);
      slot++;
    }
    item++;
    i++;
  }

  /* Centre the lines on the 192-pixel screen. A line of kind 2 has
     * 8 pixels of space above it. (&entriesA[i])[1] instead of
     * entriesA[i + 1] makes mwcc walk one pointer, as the original does. */
  height = 0;
  i = 0;
  while (1) {
    height += 16;
    if ((&work->entriesA[i])[1].id == 0) {
      break;
    }
    if ((&work->entriesA[i])[1].kind == 2) {
      height += 8;
    }
    i++;
  }
  y = (192 - height) >> 1;
  i = 0;
  while (1) {
    if (work->entriesA[i].id == 0) {
      break;
    }
    work->entriesA[i].pos = y;
    y += 16;
    if ((&work->entriesA[i])[1].kind == 2) {
      y += 8;
    }
    i++;
  }
  return menu;
}
