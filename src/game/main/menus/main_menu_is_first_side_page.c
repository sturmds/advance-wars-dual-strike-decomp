#include "types.h"

/* 1 for pages 0-5 and 9-13, which flip to page 6; 0 for pages 6-8, which flip to page 0. */
int MainMenu_IsFirstSidePage(int page) {
  switch (page) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
      return 1;
    default:
      return 0;
  }
}
