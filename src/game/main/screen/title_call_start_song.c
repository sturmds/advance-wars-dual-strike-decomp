#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_header.h"
#include "game/song.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Title call script step: starts the song of the loaded campaign map: song
 * 43 for missions 0 to 8, 50 for 19 to 24, 49 for the others. */
void TitleCall_StartSong(void) {
  u32 mapId = data_027c027c->unk_48[data_027c0284]->unk_1c4->mapId;

  if (mapId <= MAP_CAMPAIGN_FIRST + 8) {
    Sound_StartSong(43);
  } else if (mapId <= MAP_CAMPAIGN_FIRST + 18 ||
             mapId >= MAP_CAMPAIGN_FIRST + 25) {
    Sound_StartSong(49);
  } else {
    Sound_StartSong(50);
  }
}
