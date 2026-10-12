#include "types.h"
#include "game/tile_point.h"

void Ov006_MarkMapBorder(void);
void Ov006_PickLinkTiles(int x1, int y1, int x2, int y2, int range,
                         TilePoint* out1, TilePoint* out2, int* vertical);
void Ov006_MarkLinkPath(
    int x1, int y1, int x2, int y2,
    int vertical); /* marks a road path with 2 in Ov006_gGenMarks */
void Ov006_PlaceRoadsAtMarks(void);

/* x of a TilePoint that was never set (Ov006_GenerateRandomMap's city tiles). */
#define NO_TILE 255

/* Random map step: a road joins tiles near the two HQs; when all three
 * city tiles exist, roads also join each army's city to the middle one. */
void Ov006_AddRoads(TilePoint* hq1, TilePoint* hq2, TilePoint* city1,
                    TilePoint* city2, TilePoint* middle) {
  TilePoint from;
  TilePoint to;
  int vertical;

  Ov006_MarkMapBorder();
  Ov006_PickLinkTiles(hq1->x, hq1->y, hq2->x, hq2->y, 4, &from, &to, &vertical);
  Ov006_MarkLinkPath(from.x, from.y, to.x, to.y, vertical);
  if (city1->x != NO_TILE && city2->x != NO_TILE && middle->x != NO_TILE) {
    Ov006_PickLinkTiles(city1->x, city1->y, middle->x, middle->y, 2, &from, &to,
                        &vertical);
    Ov006_MarkLinkPath(from.x, from.y, to.x, to.y, vertical);
    Ov006_PickLinkTiles(city2->x, city2->y, middle->x, middle->y, 2, &from, &to,
                        &vertical);
    Ov006_MarkLinkPath(from.x, from.y, to.x, to.y, vertical);
  }
  Ov006_PlaceRoadsAtMarks();
}
