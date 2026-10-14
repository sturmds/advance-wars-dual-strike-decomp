#include "types.h"
#include "game/task.h"
#include "game/menu_button.h"

void PopupMenuButton_Update(void);
void func_0201ee24(void);
void PopupMenuButton_Keys(void);
void PopupMenu_Draw(void);
void PopupMenuButton_Confirm(void);
void PopupMenuButton_Cancel(void);
void PopupMenuButton_Select(void);

static u16 data_02158748[] = {
#include "data/main/data_02158748.inc.c"
};
static u16 data_0215874c[] = {
#include "data/main/data_0215874c.inc.c"
};
static u16 data_02158758[] = {
#include "data/main/data_02158758.inc.c"
};
static u16 data_02158754[] = {
#include "data/main/data_02158754.inc.c"
};
u16 gWindowFrameBottomTiles[] = {
#include "data/main/gWindowFrameBottomTiles.inc.c"
};
u8 data_0215875c[] = {
#include "data/main/data_0215875c.inc.c"
};
MenuButtonTemplate data_02158780 = {
#include "data/main/data_02158780.inc.c"
};
TaskScriptCmd data_021587a8[] = {
#include "data/main/data_021587a8.inc.c"
};

/* Writes a middle row of a window frame into a tile map row: a left edge
 * tile, width - 3 inner tiles and two right edge tiles. rowKind (0 or 1)
 * picks the tile from each of the four tables, so alternating kinds stripe
 * the frame. Tiles outside columns 0 to 31 are skipped. */
void Window_DrawFrameMiddleRow(u16* row, int x, s16 width, int rowKind,
                               int palette) {
  u16 i;
  int middle;
  u16 tileBits = palette << 12;

  if (x >= 0 && x < 32) {
    *row = tileBits | data_02158748[rowKind];
  }
  middle = width - 3;
  row++;
  x++;
  for (i = 0; i < middle; i++) {
    if (x >= 0 && x < 32) {
      *row = tileBits | data_0215874c[rowKind];
    }
    row++;
    x++;
  }
  if (x >= 0 && x < 32) {
    *row = tileBits | data_02158758[rowKind];
  }
  x++;
  if (x < 0) {
    return;
  }
  if (x < 32) {
    row[1] = tileBits | data_02158754[rowKind];
  }
}
