#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

void func_0208c46c(void *tileMap, int value); /* sets every tile of a per-tile map to value */

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Sets MapTiles.threatMax and threatSum to 0 for every tile. */
void Ai_ClearThreatMaps(void)
{
    func_0208c46c(STATE->mapTiles->threatMax, 0);
    func_0208c46c(STATE->mapTiles->threatSum, 0);
}
