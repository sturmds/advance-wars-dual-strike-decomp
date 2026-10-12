#include "types.h"
#include "game/terrain.h"

void Ov006_SetTerrain(int x, int y, int terrain);
void Ov006_StoreMapCell(int x, int y,
                        int cell); /* guess: sets the cell code of tile x, y */
int Ov006_FitPipeCell(int x, int y,
                      int flag); /* guess: picks the cell code for tile x, y */
void Ov006_RefitJoinedCellsInCross(int x, int y);
void Ov006_RefreshNeighborRiverCells(int x, int y);
void Ov006_ResetNeighborsToSea(int x, int y);
void Ov006_RefitCellsAround(int x, int y);
void Ov006_CheckReefsAround(int x, int y);
void Ov006_BreakCellBlock(int x, int y);

/* Puts a pipe at (x, y), picks its cell, refits the joined cells around it,
 * then runs the same follow-up fixes as Ov006_PlaceMountain. */
void Ov006_PlacePipe(int x, int y) {
  Ov006_SetTerrain(x, y, TERRAIN_PIPE);
  Ov006_StoreMapCell(x, y, Ov006_FitPipeCell(x, y, 1));
  Ov006_RefitJoinedCellsInCross(x, y);
  Ov006_RefreshNeighborRiverCells(x, y);
  Ov006_ResetNeighborsToSea(x, y);
  Ov006_RefitCellsAround(x, y);
  Ov006_CheckReefsAround(x, y);
  Ov006_BreakCellBlock(x, y);
}
