#include "types.h"
#include "game/Unk_02080440.h"

void MapView_DrawBottomRow(u16 bufX, u16 bufY, s16 mapX, s16 mapY);
void MapView_DrawTopRow(u16 bufX, u16 bufY, s16 mapX, s16 mapY);
void MapView_DrawRightColumn(u16 bufX, u16 bufY, s16 mapX, s16 mapY);
void MapView_DrawLeftColumn(u16 bufX, u16 bufY, s16 mapX, s16 mapY);
BOOL func_020f788c(void);

#define VIEW (data_027c027c->unk_48[data_027c0284]->unk_1c4)
/* needed to match: the original adds the scaled index before the base. */
#define GAME_IDX_FIRST                                                    \
  (*(Unk_02080440_48**)((u8*)(data_027c0284 * sizeof(Unk_02080440_48*)) + \
                            (u32)data_027c027c + (u32) &                  \
                        ((Unk_02080440*)0)->unk_48))
#define HOOKS (data_027c027c->unk_50[data_027c0284])

/* The scroll (in 1/16 tiles) moved from prev to cur into another tile. */
#define CROSSED_TILE(cur, prev)                     \
  (((cur) < (prev) && (cur) >> 4 != (prev) >> 4) || \
   ((cur) > (prev) && (cur) >> 4 != ((prev) - 1) >> 4))

#define RUN_EDGE_HOOKS()           \
  do {                             \
    if (HOOKS->unk_08 != NULL) {   \
      HOOKS->unk_08();             \
    }                              \
    if (HOOKS->unk_10 != NULL) {   \
      HOOKS->unk_10();             \
    }                              \
    if (HOOKS->unk_0c != NULL) {   \
      HOOKS->unk_0c();             \
    }                              \
    if (func_020f788c()) {         \
      if (HOOKS->unk_14 != NULL) { \
        HOOKS->unk_14();           \
      }                            \
    }                              \
  } while (0)

/* When the map view has scrolled into a new tile column or row since the
 * last call, draws the tiles that came into view: a column at the left or
 * right edge of the 16-tile-wide view, a row at the top or bottom of the
 * 12-tile-high view. unk_26 and unk_28 give the map tile the background
 * buffer starts at; unk_22 and unk_24 keep the scroll seen last. */
void MapView_DrawScrolledEdges(void) {
  Unk_02080440_1c4* view;
  s16 prev;
  s16 cur;

  view = GAME_IDX_FIRST->unk_1c4;
  prev = view->unk_22;
  /* needed to match: a cast read, so that the calls below load scrollX again */
  cur = *(s16*)&view->scrollX;
  if (CROSSED_TILE(cur, prev)) {
    if (cur < prev) {
      MapView_DrawLeftColumn((view->scrollX >> 4) - view->unk_26,
                             (view->scrollY >> 4) - view->unk_28,
                             view->scrollX >> 4, view->scrollY >> 4);
    } else {
      MapView_DrawRightColumn((view->scrollX >> 4) - view->unk_26 + 16,
                              (view->scrollY >> 4) - view->unk_28,
                              (view->scrollX >> 4) + 16, view->scrollY >> 4);
    }
    RUN_EDGE_HOOKS();
  }

  view = GAME_IDX_FIRST->unk_1c4;
  prev = view->unk_24;
  cur = *(s16*)&view->scrollY;
  if (CROSSED_TILE(cur, prev)) {
    if (cur < prev) {
      MapView_DrawTopRow((view->scrollX >> 4) - view->unk_26,
                         (view->scrollY >> 4) - view->unk_28,
                         view->scrollX >> 4, view->scrollY >> 4);
    } else {
      MapView_DrawBottomRow((view->scrollX >> 4) - view->unk_26,
                            (view->scrollY >> 4) - view->unk_28 + 12,
                            view->scrollX >> 4, (view->scrollY >> 4) + 12);
    }
    RUN_EDGE_HOOKS();
  }

  VIEW->unk_22 = VIEW->scrollX;
  VIEW->unk_24 = VIEW->scrollY;
}
