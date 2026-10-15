#include "types.h"
#include "game/Unk_0229c734.h"
#include "game/math.h"

void ResultPoint_QueueDigitPopIn(int x, int y, int digit, int matrixIndex,
                                 int frame, int palette);
void ResultPoint_QueueNumber(int x, int y, int value, int palette);

#define SCORES data_0229c734

/* Draws the points of score row `row`, x being the left of a three digit
 * number. While the tally is below the row's points it shows the tally;
 * then the row's own digits pop in through the row's three affine
 * matrices. */
void ScoreList_DrawRowPoints(int x, int y, int row, int tally) {
  u16 points = SCORES->rows[row].points;
  int matrixIndex;
  int rest;
  int digit;

  if (points > tally) {
    ResultPoint_QueueNumber(x, y, tally, 0);
    return;
  }
  matrixIndex = (row - SCORES->firstRow) * 3;
  /* Needed to match: tally becomes the frames since it reached the
     * points, which time each digit's pop-in, 4 frames apart. */
  tally -= points;
  if (points >= 100) {
    digit = HwDiv32(points, 100);
    rest = HwMod32(SCORES->rows[row].points, 100);
    ResultPoint_QueueDigitPopIn(x, y, digit, matrixIndex, tally, 0);
    digit = HwDiv32(rest, 10);
    rest = HwMod32(rest, 10);
    ResultPoint_QueueDigitPopIn(x + 12, y, digit, matrixIndex + 1, tally - 4,
                                0);
    ResultPoint_QueueDigitPopIn(x + 24, y, rest, matrixIndex + 2, tally - 8, 0);
  } else if (points >= 10) {
    digit = HwDiv32(points, 10);
    rest = HwMod32(SCORES->rows[row].points, 10);
    ResultPoint_QueueDigitPopIn(x + 12, y, digit, matrixIndex, tally, 0);
    ResultPoint_QueueDigitPopIn(x + 24, y, rest, matrixIndex + 1, tally - 4, 0);
  } else {
    ResultPoint_QueueDigitPopIn(x + 24, y, points, matrixIndex, tally, 0);
  }
}
