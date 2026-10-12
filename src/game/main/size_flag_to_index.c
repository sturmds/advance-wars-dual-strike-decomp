#include "types.h"

/* Maps a power-of-two size flag (8, 16, 32, 64) to its index 0 to 3. */
int SizeFlag_ToIndex(int size) {
  switch (size) {
    case 8:
      return 0;
    case 16:
      return 1;
    case 32:
      return 2;
    case 64:
      return 3;
  }
  return 0;
}
