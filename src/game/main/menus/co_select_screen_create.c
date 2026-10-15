#include "types.h"
#include "game/Unk_02080440.h"
#include "game/co_select_screen.h"
#include "game/map_header.h"
#include "game/screen_other_map.h"

/* Needed to match: coGrid read as rows of 2, flags 6 bytes on. */
#define CELL_CO(column, row) \
  (((u8(*)[2])data_0229c73c->coGrid)[(column) - 1][row])
#define CELL_FLAGS(column, row) \
  (((u8(*)[2])data_0229c73c->coGrid)[(column) - 1] + (row))[6]
#define CELL_LOCKED 1
#define MAP_CO_PICKED 0x1c /* MapHeader.co: the player picks */

extern u8 data_0215abcc[]; /* coList table on a normal-mode campaign map */
extern u8 data_0215abe4[]; /* coList table everywhere else */

void* Mem_Alloc(u32 size);
int Campaign_IsHardMode(void);
u8 func_020c3d08(u32 mapId);
BOOL CoSelect_HasTagRow(void);
int CoSelectScreen_ListSlotOfCo(int co);

/* Allocates the CO select work area and fills the grid: one column per
 * player of the map, two rows unless tag COs are off (a campaign map with
 * no back CO for player 1, or CoSelect_HasTagRow says no). On a normal-mode
 * campaign map, cells whose CO the map header sets are locked. The grid
 * starts with the players' current COs and the cursor on the first cell.
 * Needed to match: each loop has its own counters. */
void CoSelectScreen_Create(void) {
  int column;
  int row;
  int lockColumn;
  int lockRow;
  int copyColumn;
  int copyRow;

  data_0229c73c = Mem_Alloc(sizeof(CoSelectScreen));
  data_0229c73c->columnCount = func_020c3d08(MAP_STATE->unk_1c4->mapId);
  data_0229c73c->rowCount = 2;
  for (column = 1; column < data_0229c73c->columnCount + 1; column++) {
    for (row = 0; row < data_0229c73c->rowCount; row++) {
      CELL_FLAGS(column, row) = 0;
    }
  }

  if (MAP_STATE->gameMode == GAME_MODE_CAMPAIGN) {
    if (Campaign_IsHardMode() == 1) {
      data_0229c73c->coList = data_0215abe4;
    } else {
      data_0229c73c->coList = data_0215abcc;
      if (Ov000_gMapHeaders[MAP_STATE->unk_1c4->mapId].co[1][1] == 0) {
        data_0229c73c->rowCount = 1;
      } else {
        for (lockColumn = 1; lockColumn < data_0229c73c->columnCount + 1;
             lockColumn++) {
          for (lockRow = 0; lockRow < data_0229c73c->rowCount; lockRow++) {
            if (Ov000_gMapHeaders[MAP_STATE->unk_1c4->mapId]
                    .co[lockColumn][lockRow] != MAP_CO_PICKED) {
              CELL_FLAGS(lockColumn, lockRow) |= CELL_LOCKED;
            }
          }
        }
      }
    }
  } else {
    data_0229c73c->coList = data_0215abe4;
    if (CoSelect_HasTagRow() == 0) {
      data_0229c73c->rowCount = 1;
    }
  }

  for (copyColumn = 1; copyColumn < data_0229c73c->columnCount + 1;
       copyColumn++) {
    for (copyRow = 0; copyRow < data_0229c73c->rowCount; copyRow++) {
      CELL_CO(copyColumn, copyRow) =
          MAP_STATE->playerCos[copyColumn][copyRow] & 0x7f;
    }
  }

  data_0229c73c->cursorColumn = 1;
  data_0229c73c->cursorRow = 0;
  data_0229c73c->unk_30 = CoSelectScreen_ListSlotOfCo(
      CELL_CO(data_0229c73c->cursorColumn, data_0229c73c->cursorRow));
  data_0229c73c->coBeforeList = data_0229c73c->unk_30;
  data_0229c73c->coListOpen = 0;
  data_0229c73c->listAnchor = data_0229c73c->unk_30;
}
