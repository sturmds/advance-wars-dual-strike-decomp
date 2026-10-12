#include "types.h"

/* Maps a row's co (Ov003Row.co) to a group: 1 for 0 to 3, 5 for 4 to 7, 0 otherwise. */
int Ov003_RowKindGroup(int rowKind) {
  if (rowKind <= 3) {
    return 1;
  }
  if (rowKind <= 7) {
    return 5;
  }
  return 0;
}
