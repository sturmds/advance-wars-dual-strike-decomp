#include "types.h"
#include "game/design_map_edit.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern u32 data_027c0284;

int Ov006_GetPipeSeamCell(int x,
                          int y); /* guess: the seam cell that fits tile x, y */
int Ov006_IsJoinedPieceConnected(
    int x, int y); /* nonzero: keep the cell at x, y as it is */
int Ov006_GetPipeNeighborHold(
    int x, int y,
    int dir); /* 0-2: how firmly the neighbour in dir links on; 2 off the map */
int Ov006_GetPipeLink(
    int x, int y,
    int dir); /* 0-3: what the neighbour in dir is; 0 nothing to join */

#define STATE (data_027c027c->unk_48[data_027c0284])
#define CELL_AT(x, y) \
  (STATE->mapTiles->mapCells[(x) + STATE->mapTiles->rowStart[y]])

/* Map cell codes. The cell set is 32 cells wide: the four corners form a 2x2
 * block at rows 10-11, with the straight pieces and the seams beside them.
 * Taken as pipes because Ov006_GetPipeSeamCell turns 0x142 into the seam
 * 0x162 and 0x143 into 0x163. */
#define PIPE_DOWN_RIGHT 0x140
#define PIPE_UP_RIGHT 0x141
#define PIPE_VERTICAL 0x142
#define PIPE_HORIZONTAL 0x143
#define PIPE_DOWN_LEFT 0x160
#define PIPE_UP_LEFT 0x161
#define PIPE_SEAM_VERTICAL 0x162 /* PIPE_SEAM_HORIZONTAL is 0x163 */
#define PIPE_CELL_122 0x122      /* fallbacks no neighbour count can reach */
#define PIPE_CELL_123 0x123

/* Neighbour directions, from the step tables of Ov006_GetPipeLink. */
#define DIR_UP 0
#define DIR_DOWN 1
#define DIR_LEFT 2
#define DIR_RIGHT 3

#define LINKS(dir) Ov006_GetPipeNeighborHold(x, y, dir)

/* Picks the pipe cell for tile x, y from its four neighbours. A seam is
 * kept unless replaceSeam is set. With one neighbour the pipe runs toward
 * it. With more, neighbours of kind 2 win, then the pair with the weakest
 * links (Ov006_GetPipeNeighborHold below 2), giving a straight piece or a corner. */
int Ov006_FitPipeCell(int x, int y, int replaceSeam) {
  u8 n[4];
  int count = 0;
  int i;

  /* needed to match: the seam test adds a u16 constant */
  if (replaceSeam == 0 &&
      (u16)(CELL_AT(x, y) + (u16)-PIPE_SEAM_VERTICAL) <= 1) {
    return Ov006_GetPipeSeamCell(x, y);
  }
  if (Ov006_IsJoinedPieceConnected(x, y)) {
    return CELL_AT(x, y);
  }
  for (i = 0; i <= DESIGN_DIR_COUNT - 1; i++) {
    n[i] = Ov006_GetPipeLink(x, y, i);
    if (n[i] != 0) {
      count++;
    }
  }
  if (count == 0) {
    return PIPE_VERTICAL;
  }
  if (count == 1) {
    if (n[DIR_UP] != 0 || n[DIR_DOWN] != 0) {
      return PIPE_VERTICAL;
    }
    return PIPE_HORIZONTAL;
  }
  if (count == 2) {
    if (n[DIR_UP] != 0 && n[DIR_DOWN] != 0) {
      return PIPE_VERTICAL;
    }
    if (n[DIR_LEFT] != 0 && n[DIR_RIGHT] != 0) {
      return PIPE_HORIZONTAL;
    }
    if (n[DIR_UP] != 0 && n[DIR_LEFT] != 0) {
      if (LINKS(DIR_UP) < 2 && LINKS(DIR_LEFT) < 2) {
        return PIPE_UP_LEFT;
      }
      if (LINKS(DIR_UP) < 2) {
        return PIPE_VERTICAL;
      }
      return LINKS(DIR_LEFT) < 2 ? PIPE_HORIZONTAL : PIPE_VERTICAL;
    }
    if (n[DIR_UP] != 0 && n[DIR_RIGHT] != 0) {
      if (LINKS(DIR_UP) < 2 && LINKS(DIR_RIGHT) < 2) {
        return PIPE_UP_RIGHT;
      }
      if (LINKS(DIR_UP) < 2) {
        return PIPE_VERTICAL;
      }
      return LINKS(DIR_RIGHT) < 2 ? PIPE_HORIZONTAL : PIPE_VERTICAL;
    }
    if (n[DIR_DOWN] != 0 && n[DIR_LEFT] != 0) {
      if (LINKS(DIR_DOWN) < 2 && LINKS(DIR_LEFT) < 2) {
        return PIPE_DOWN_LEFT;
      }
      if (LINKS(DIR_DOWN) < 2) {
        return PIPE_VERTICAL;
      }
      return LINKS(DIR_LEFT) < 2 ? PIPE_HORIZONTAL : PIPE_VERTICAL;
    }
    if (n[DIR_DOWN] != 0 && n[DIR_RIGHT] != 0) {
      if (LINKS(DIR_DOWN) < 2 && LINKS(DIR_RIGHT) < 2) {
        return PIPE_DOWN_RIGHT;
      }
      if (LINKS(DIR_DOWN) < 2) {
        return PIPE_VERTICAL;
      }
      return LINKS(DIR_RIGHT) < 2 ? PIPE_HORIZONTAL : PIPE_VERTICAL;
    }
    return PIPE_CELL_122;
  }
  if (count == 3) {
    if (n[DIR_UP] == 2 && n[DIR_DOWN] == 2) {
      return PIPE_VERTICAL;
    }
    if (n[DIR_LEFT] == 2 && n[DIR_RIGHT] == 2) {
      return PIPE_HORIZONTAL;
    }
    if (n[DIR_UP] == 2 && n[DIR_LEFT] == 2) {
      return PIPE_UP_LEFT;
    }
    if (n[DIR_UP] == 2 && n[DIR_RIGHT] == 2) {
      return PIPE_UP_RIGHT;
    }
    if (n[DIR_DOWN] == 2 && n[DIR_LEFT] == 2) {
      return PIPE_DOWN_LEFT;
    }
    if (n[DIR_DOWN] == 2 && n[DIR_RIGHT] == 2) {
      return PIPE_DOWN_RIGHT;
    }
    if (n[DIR_UP] == 2) {
      if (n[DIR_DOWN] != 0) {
        if (LINKS(DIR_DOWN) < 2) {
          return PIPE_VERTICAL;
        }
        if (n[DIR_LEFT] != 0) {
          return LINKS(DIR_LEFT) < 2 ? PIPE_UP_LEFT : PIPE_VERTICAL;
        }
        if (n[DIR_RIGHT] != 0) {
          return LINKS(DIR_RIGHT) < 2 ? PIPE_UP_RIGHT : PIPE_VERTICAL;
        }
        return PIPE_CELL_122;
      }
      if (LINKS(DIR_LEFT) < 2 && LINKS(DIR_RIGHT) < 2) {
        return LINKS(DIR_LEFT) < LINKS(DIR_RIGHT) ? PIPE_UP_LEFT
                                                  : PIPE_UP_RIGHT;
      }
      if (LINKS(DIR_LEFT) < 2) {
        return PIPE_UP_LEFT;
      }
      return LINKS(DIR_RIGHT) < 2 ? PIPE_UP_RIGHT : PIPE_VERTICAL;
    }
    if (n[DIR_DOWN] == 2) {
      if (n[DIR_UP] != 0) {
        if (LINKS(DIR_UP) < 2) {
          return PIPE_VERTICAL;
        }
        if (n[DIR_LEFT] != 0) {
          return LINKS(DIR_LEFT) < 2 ? PIPE_DOWN_LEFT : PIPE_VERTICAL;
        }
        if (n[DIR_RIGHT] != 0) {
          return LINKS(DIR_RIGHT) < 2 ? PIPE_DOWN_RIGHT : PIPE_VERTICAL;
        }
        return PIPE_CELL_122;
      }
      if (LINKS(DIR_LEFT) < 2 && LINKS(DIR_RIGHT) < 2) {
        return LINKS(DIR_LEFT) < LINKS(DIR_RIGHT) ? PIPE_DOWN_LEFT
                                                  : PIPE_DOWN_RIGHT;
      }
      if (LINKS(DIR_LEFT) < 2) {
        return PIPE_DOWN_LEFT;
      }
      return LINKS(DIR_RIGHT) < 2 ? PIPE_DOWN_RIGHT : PIPE_VERTICAL;
    }
    if (n[DIR_LEFT] == 2) {
      if (n[DIR_RIGHT] != 0) {
        if (LINKS(DIR_RIGHT) < 2) {
          return PIPE_HORIZONTAL;
        }
        if (n[DIR_UP] != 0) {
          return LINKS(DIR_UP) < 2 ? PIPE_UP_LEFT : PIPE_HORIZONTAL;
        }
        if (n[DIR_DOWN] != 0) {
          return LINKS(DIR_DOWN) < 2 ? PIPE_DOWN_LEFT : PIPE_HORIZONTAL;
        }
        return PIPE_CELL_123;
      }
      if (LINKS(DIR_UP) < 2 && LINKS(DIR_DOWN) < 2) {
        return LINKS(DIR_UP) < LINKS(DIR_DOWN) ? PIPE_UP_LEFT : PIPE_DOWN_LEFT;
      }
      if (LINKS(DIR_UP) < 2) {
        return PIPE_UP_LEFT;
      }
      return LINKS(DIR_DOWN) < 2 ? PIPE_DOWN_LEFT : PIPE_HORIZONTAL;
    }
    if (n[DIR_RIGHT] == 2) {
      if (n[DIR_LEFT] != 0) {
        if (LINKS(DIR_LEFT) < 2) {
          return PIPE_HORIZONTAL;
        }
        if (n[DIR_UP] != 0) {
          return LINKS(DIR_UP) < 2 ? PIPE_UP_RIGHT : PIPE_HORIZONTAL;
        }
        if (n[DIR_DOWN] != 0) {
          return LINKS(DIR_DOWN) < 2 ? PIPE_DOWN_RIGHT : PIPE_HORIZONTAL;
        }
        return PIPE_CELL_123;
      }
      if (LINKS(DIR_UP) < 2 && LINKS(DIR_DOWN) < 2) {
        return LINKS(DIR_UP) < LINKS(DIR_DOWN) ? PIPE_UP_RIGHT
                                               : PIPE_DOWN_RIGHT;
      }
      if (LINKS(DIR_UP) < 2) {
        return PIPE_UP_RIGHT;
      }
      return LINKS(DIR_DOWN) < 2 ? PIPE_DOWN_RIGHT : PIPE_HORIZONTAL;
    }
    if (LINKS(DIR_UP) < 2 && LINKS(DIR_DOWN) < 2) {
      return PIPE_VERTICAL;
    }
    if (LINKS(DIR_LEFT) < 2 && LINKS(DIR_RIGHT) < 2) {
      return PIPE_HORIZONTAL;
    }
    if (LINKS(DIR_UP) < 2 && LINKS(DIR_LEFT) < 2) {
      return PIPE_UP_LEFT;
    }
    if (LINKS(DIR_UP) < 2 && LINKS(DIR_RIGHT) < 2) {
      return PIPE_UP_RIGHT;
    }
    if (LINKS(DIR_DOWN) < 2 && LINKS(DIR_LEFT) < 2) {
      return PIPE_DOWN_LEFT;
    }
    if (LINKS(DIR_DOWN) < 2 && LINKS(DIR_RIGHT) < 2) {
      return PIPE_DOWN_RIGHT;
    }
    if (LINKS(DIR_UP) < 2 || LINKS(DIR_DOWN) < 2) {
      return PIPE_VERTICAL;
    }
    if (LINKS(DIR_LEFT) < 2 || LINKS(DIR_RIGHT) < 2) {
      return PIPE_HORIZONTAL;
    }
    return PIPE_VERTICAL;
  }
  if (count == 4) {
    if (n[DIR_UP] == 2 && n[DIR_DOWN] == 2) {
      return PIPE_VERTICAL;
    }
    if (n[DIR_LEFT] == 2 && n[DIR_RIGHT] == 2) {
      return PIPE_HORIZONTAL;
    }
    if (n[DIR_UP] == 2 && n[DIR_LEFT] == 2) {
      return PIPE_UP_LEFT;
    }
    if (n[DIR_UP] == 2 && n[DIR_RIGHT] == 2) {
      return PIPE_UP_RIGHT;
    }
    if (n[DIR_DOWN] == 2 && n[DIR_LEFT] == 2) {
      return PIPE_DOWN_LEFT;
    }
    if (n[DIR_DOWN] == 2 && n[DIR_RIGHT] == 2) {
      return PIPE_DOWN_RIGHT;
    }
    if (n[DIR_UP] == 2) {
      if (LINKS(DIR_DOWN) < 1) {
        return PIPE_VERTICAL;
      }
      if (LINKS(DIR_LEFT) < 1) {
        return PIPE_UP_LEFT;
      }
      if (LINKS(DIR_RIGHT) < 1) {
        return PIPE_UP_RIGHT;
      }
      if (LINKS(DIR_DOWN) < 2) {
        return PIPE_VERTICAL;
      }
      if (LINKS(DIR_LEFT) < 2) {
        return PIPE_UP_LEFT;
      }
      return LINKS(DIR_RIGHT) < 2 ? PIPE_UP_RIGHT : PIPE_VERTICAL;
    }
    if (n[DIR_DOWN] == 2) {
      if (LINKS(DIR_UP) < 1) {
        return PIPE_VERTICAL;
      }
      if (LINKS(DIR_LEFT) < 1) {
        return PIPE_DOWN_LEFT;
      }
      if (LINKS(DIR_RIGHT) < 1) {
        return PIPE_DOWN_RIGHT;
      }
      if (LINKS(DIR_UP) < 2) {
        return PIPE_VERTICAL;
      }
      if (LINKS(DIR_LEFT) < 2) {
        return PIPE_DOWN_LEFT;
      }
      return LINKS(DIR_RIGHT) < 2 ? PIPE_DOWN_RIGHT : PIPE_VERTICAL;
    }
    if (n[DIR_LEFT] == 2) {
      if (LINKS(DIR_RIGHT) < 1) {
        return PIPE_HORIZONTAL;
      }
      if (LINKS(DIR_UP) < 1) {
        return PIPE_UP_LEFT;
      }
      if (LINKS(DIR_DOWN) < 1) {
        return PIPE_DOWN_LEFT;
      }
      if (LINKS(DIR_RIGHT) < 2) {
        return PIPE_HORIZONTAL;
      }
      if (LINKS(DIR_UP) < 2) {
        return PIPE_UP_LEFT;
      }
      return LINKS(DIR_DOWN) < 2 ? PIPE_DOWN_LEFT : PIPE_HORIZONTAL;
    }
    if (n[DIR_RIGHT] == 2) {
      if (LINKS(DIR_LEFT) < 1) {
        return PIPE_HORIZONTAL;
      }
      if (LINKS(DIR_UP) < 1) {
        return PIPE_UP_RIGHT;
      }
      if (LINKS(DIR_DOWN) < 1) {
        return PIPE_DOWN_RIGHT;
      }
      if (LINKS(DIR_LEFT) < 2) {
        return PIPE_HORIZONTAL;
      }
      if (LINKS(DIR_UP) < 2) {
        return PIPE_UP_RIGHT;
      }
      return LINKS(DIR_DOWN) < 2 ? PIPE_DOWN_RIGHT : PIPE_HORIZONTAL;
    }
    if (LINKS(DIR_UP) < 2 && LINKS(DIR_DOWN) < 2) {
      return PIPE_VERTICAL;
    }
    if (LINKS(DIR_LEFT) < 2 && LINKS(DIR_RIGHT) < 2) {
      return PIPE_HORIZONTAL;
    }
    if (LINKS(DIR_UP) < 2 && LINKS(DIR_LEFT) < 2) {
      return PIPE_UP_LEFT;
    }
    if (LINKS(DIR_UP) < 2 && LINKS(DIR_RIGHT) < 2) {
      return PIPE_UP_RIGHT;
    }
    if (LINKS(DIR_DOWN) < 2 && LINKS(DIR_LEFT) < 2) {
      return PIPE_DOWN_LEFT;
    }
    if (LINKS(DIR_DOWN) < 2 && LINKS(DIR_RIGHT) < 2) {
      return PIPE_DOWN_RIGHT;
    }
    if (LINKS(DIR_UP) < 2 || LINKS(DIR_DOWN) < 2) {
      return PIPE_VERTICAL;
    }
    if (LINKS(DIR_LEFT) < 2 || LINKS(DIR_RIGHT) < 2) {
      return PIPE_HORIZONTAL;
    }
    return PIPE_VERTICAL;
  }
  return -1;
}
