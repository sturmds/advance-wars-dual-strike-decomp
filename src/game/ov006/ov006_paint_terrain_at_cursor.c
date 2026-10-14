#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_ov006_0235f690.h"
#include "game/terrain.h"

int Ov006_GetTilePropertyKind(int x, int y);
void Ov006_RemoveProperty(int x, int y);
void Ov006_SetTerrain(int x, int y, int terrain);
void func_ov006_0235da70(int x, int y, int cell);
void Ov006_BreakCellBlock(int x, int y);
void Ov006_CheckReefsAround(int x, int y);
void Ov006_RemoveUnitOnImpassableTile(int x, int y);
int Ov006_PlaceShoal(int x, int y);
void Ov006_PlaceBridge(int x, int y);
void Ov006_PlaceMountain(int x, int y);
void Ov006_TurnToSea(int x, int y);
int Ov006_PlaceRiver(int x, int y);
int Ov006_CountMatchingNeighbors(int x, int y);
int Ov006_PlaceReef(int x, int y);
int Ov006_PlaceProperty(
    int x, int y, int tile); /* places a property; -1 at the property limit */
void Ov006_PlaceWood(int x, int y);
void Ov006_PlaceRoad(int x, int y);
void Ov006_PlacePipeSeam(int x, int y);
void Ov006_PlacePipe(int x, int y);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern Unk_ov006_0235f690* data_ov006_0236d640;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Turns a property on the tile back into plain first. */
#define CLEAR_PROPERTY(x, y)               \
  if (Ov006_GetTilePropertyKind(x, y)) {   \
    Ov006_RemoveProperty(x, y);            \
    Ov006_SetTerrain(x, y, TERRAIN_PLAIN); \
    func_ov006_0235da70(x, y, 1);          \
  }

/* Paints the chosen brush (toolTile: a tile byte, terrain and owner) on the
 * tile at cursorX, cursorY, after saving the cell it replaces in oldCell.
 * Each terrain goes through its own placing routine, then units that can no
 * longer stand there are removed. The sound effect to play is left in
 * pendingSe. */
void Ov006_PaintTerrainAtCursor(void) {
  int x;
  int y;
  int result;
  int brush;

  y = data_ov006_0236d640->cursorY;
  x = data_ov006_0236d640->cursorX;
  data_ov006_0236d640->oldCell =
      STATE->mapTiles->mapCells[STATE->mapTiles->rowStart[y] + x];
  data_ov006_0236d640->unk_00 |= 0x1000;
  brush = data_ov006_0236d640->toolTile;
  switch (brush & 0x1f) {
    case TERRAIN_SEA:
      Ov006_TurnToSea(x, y);
      Ov006_RemoveUnitOnImpassableTile(x, y);
      data_ov006_0236d640->pendingSe = 73;
      break;
    case TERRAIN_SHOAL:
      if (Ov006_CountMatchingNeighbors(x, y) >= 0) {
        CLEAR_PROPERTY(x, y);
      }
      Ov006_PlaceShoal(x, y);
      Ov006_RemoveUnitOnImpassableTile(x, y);
      data_ov006_0236d640->pendingSe = 91;
      break;
    case TERRAIN_ROAD:
      CLEAR_PROPERTY(x, y);
      Ov006_PlaceRoad(x, y);
      Ov006_RemoveUnitOnImpassableTile(x, y);
      data_ov006_0236d640->pendingSe = 72;
      break;
    case TERRAIN_RIVER:
      CLEAR_PROPERTY(x, y);
      if (Ov006_PlaceRiver(x, y) == 0) {
        return;
      }
      Ov006_RemoveUnitOnImpassableTile(x, y);
      data_ov006_0236d640->pendingSe = 92;
      break;
    case TERRAIN_BRIDGE:
      Ov006_PlaceBridge(x, y);
      Ov006_RemoveUnitOnImpassableTile(x, y);
      data_ov006_0236d640->pendingSe = 72;
      break;
    case TERRAIN_MOUNTAIN:
      CLEAR_PROPERTY(x, y);
      Ov006_PlaceMountain(x, y);
      Ov006_RemoveUnitOnImpassableTile(x, y);
      data_ov006_0236d640->pendingSe = 91;
      break;
    case TERRAIN_REEF:
      if (Ov006_PlaceReef(x, y)) {
        data_ov006_0236d640->pendingSe = 73;
      }
      break;
    case TERRAIN_WOOD:
      Ov006_PlaceWood(x, y);
      Ov006_RemoveUnitOnImpassableTile(x, y);
      data_ov006_0236d640->pendingSe = 91;
      break;
    case TERRAIN_HQ:
      if (Ov006_PlaceProperty(x, y, brush) > 0) {
        data_ov006_0236d640->pendingSe = 74;
      }
      Ov006_RemoveUnitOnImpassableTile(x, y);
      break;
    case TERRAIN_CITY:
    case TERRAIN_AIRPORT:
    case TERRAIN_PORT:
    case TERRAIN_BASE:
    case TERRAIN_SILO:
    case TERRAIN_COM_TOWER:
      result = Ov006_PlaceProperty(x, y, brush);
      Ov006_RemoveUnitOnImpassableTile(x, y);
      if (result > 0) {
        data_ov006_0236d640->pendingSe = 75;
      } else if (result < 0) {
        data_ov006_0236d640->pendingSe = 0x10008; /* refused */
      }
      break;
    case TERRAIN_PIPE:
      CLEAR_PROPERTY(x, y);
      Ov006_PlacePipe(x, y);
      Ov006_RemoveUnitOnImpassableTile(x, y);
      data_ov006_0236d640->pendingSe = 75;
      break;
    case TERRAIN_PIPE_SEAM:
      CLEAR_PROPERTY(x, y);
      Ov006_PlacePipeSeam(x, y);
      Ov006_RemoveUnitOnImpassableTile(x, y);
      data_ov006_0236d640->pendingSe = 75;
      break;
    case TERRAIN_PLAIN:
      if (Ov006_GetTilePropertyKind(x, y)) {
        Ov006_RemoveProperty(x, y);
      }
      Ov006_SetTerrain(x, y, TERRAIN_PLAIN);
      func_ov006_0235da70(x, y, 1);
      Ov006_BreakCellBlock(x, y);
      Ov006_CheckReefsAround(x, y);
      Ov006_RemoveUnitOnImpassableTile(x, y);
      data_ov006_0236d640->pendingSe = 91;
      break;
  }
}
