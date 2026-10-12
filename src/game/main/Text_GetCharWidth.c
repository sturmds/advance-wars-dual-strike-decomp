#include "types.h"

int Text_GetCharWidth(int ch) {
  if (ch == '!' || ch == 'I') {
    return 8;
  }
  return 16;
}
