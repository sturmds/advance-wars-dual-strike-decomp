#include "types.h"
#include "game/oam_cells.h"
#include "game/second_screen.h"
#include "game/icon.h"
#include "game/screen_other_map.h"

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
int DoubleTerrainStars(int playerIndex, int unitType, int terrain);

extern int gTerrainStarOffsets[];

/* Second screen: draws the terrain panel at x offset x. Draws nothing when terrain is 0.
 * The defense stars are counted for the first screen's current player (the stars table
 * gTerrainStarOffsets holds an x, y pair per star, 10 percent each). The names are guesses. */
void SecondScreen_DrawTerrainPanel(int x, int terrain) {
  int defensePercent;
  int yi;
  int xi;
  Unk_027c0280* savedGfx;
  int n;
  Unk_02080440* savedScreen;

  if (terrain == 0) {
    return;
  }
  Sprite_QueueAt(0, x + 6, 8, &data_027c0018, 0x294, 0);
  Sprite_QueueAt(0, x + 38, 8, &data_027c0000, 0x2b4, 0);
  Sprite_QueueAt(0, x + 38, 40, &data_027c0000, 0x2bc, 0);
  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  defensePercent = DoubleTerrainStars(
      data_027c027c->unk_48[data_027c0284]->currentPlayer, 0, terrain & 0x1f);
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);
  /* needed to match: a switch compiles the unused compare with 1 */
  switch (data_ov000_02350548) {
    case 0:
      return;
    case 1:
      break;
  }
  if (defensePercent > 0) {
    xi = 0;
    yi = 1;
    n = 0;
    do {
      Icon_Draw(61, x + (gTerrainStarOffsets[xi] + 72),
                gTerrainStarOffsets[yi] + 24, 0, 0);
      n += 10;
      yi += 2;
      xi += 2;
    } while (n < defensePercent);
  }
  Sprite_QueueAt(0, x + 56, 40, &data_027c0060, 0xf0e8, 0);
}
