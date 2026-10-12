#include "types.h"
#include "game/screen_other_map.h"
#include "game/terrain.h"

void Sound_PlaySe(u32 soundId);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Plays the sound for a finished capture by `owner` of a tile (tile byte as
 * in MapTiles). Reads bit 1 of the owner's visionGrids through the first screen's
 * state. With the bit set it plays sound 241, and sound 153 first for an
 * HQ (returns 1 then); without it, sound 242. */
int Capture_PlayCompleteSound(int owner, int tile) {
  int result;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  int flag;

  saved0280 = data_027c0280;
  result = 0;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  flag = STATE->players[owner].visionGrids & 2;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  if (flag) {
    if (MAP_TILE_TERRAIN(tile) == TERRAIN_HQ) {
      Sound_PlaySe(153);
      result = 1;
    }
    Sound_PlaySe(241);
  } else {
    Sound_PlaySe(242);
  }
  return result;
}
