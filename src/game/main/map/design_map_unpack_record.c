#include "types.h"
#include "game/Unk_02080440.h"
#include "game/design_map.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern Unk_02080440_48 data_02183b08;

void String_Copy(char *dest, const char *src);
void Map_BuildRowStarts(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Copies a player-made map's record into the current map state: its name
 * into name, the player count, the map size and every tile's cell code.
 * The army colours are copied only for the first screen's state. */
void DesignMap_UnpackRecord(char *name, DesignMapRecord *record)
{
    int i;
    int cell;
    int x;
    int y;

    String_Copy(name, record->name);
    STATE->unk_1c4->playerCount = record->playerCount;
    if (STATE == &data_02183b08) {
        for (i = 0; i < 5; i++) {
            STATE->army[i] = record->armyColours[i];
        }
    }
    STATE->unk_1c4->mapWidth = record->width;
    STATE->unk_1c4->mapHeight = record->height;
    Map_BuildRowStarts();
    cell = 0;
    for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
        for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
            STATE->mapTiles->mapCells[STATE->mapTiles->rowStart[y] + x] = record->tileCodes[cell++];
        }
    }
}
