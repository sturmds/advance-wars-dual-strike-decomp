#include "types.h"
#include "game/tile_point.h"

s32 Rng_Range(s32 min, s32 max);
BOOL Ov006_PickRandomCellNear(int* outX, int* outY, int cell, TilePoint* center,
                              int range);
BOOL Ov006_PickRandomCellBetween(int* outX, int* outY, int cell, TilePoint* a,
                                 TilePoint* b, int range);
void Ov006_SetMapCell(int x, int y, int cell);

#define CELL_PLAIN 1
#define CELL_COM_TOWER 441
#define CELL_CITY 450
#define CELL_CITY_ARMY1 455
#define CELL_CITY_ARMY2 460
#define NO_TILE 255

/* The steps of Ov006_AddCities, in order */
enum AddCitiesStep {
  STEP_ARMY_CITIES,     /* cities of each army near its HQ */
  STEP_NEUTRAL_NEAR_HQ, /* neutral cities near each HQ */
  STEP_NEUTRAL_MIDDLE,  /* neutral cities between the HQs */
  STEP_COM_TOWERS,      /* com towers between the HQs */
  STEP_COUNT
};

/* Random map step, on plain tiles: 0-5 cities for each army near its HQ;
 * 3-5 neutral cities near each HQ (the last ones go to *city1 and *city2);
 * 3-7 neutral cities about as far from one HQ as from the other, then 0-5
 * com towers the same way (the last of either goes to *middle). A step
 * ends early when no tile is found. Tiles not set keep x = NO_TILE. */
void Ov006_AddCities(TilePoint* hq1, TilePoint* hq2, TilePoint* city1,
                     TilePoint* city2, TilePoint* middle) {
  int i;
  int count = 0;
  int step;
  int x;
  int y;

  middle->x = NO_TILE;
  city2->x = middle->x;
  city1->x = city2->x;
  for (step = 0; step < STEP_COUNT; step++) {
    switch (step) {
      case STEP_ARMY_CITIES:
        count = Rng_Range(0, 5);
        break;
      case STEP_NEUTRAL_NEAR_HQ:
        count = Rng_Range(3, 5);
        break;
      case STEP_NEUTRAL_MIDDLE:
        count = Rng_Range(3, 7);
        break;
      case STEP_COM_TOWERS:
        count = Rng_Range(0, 5);
        break;
    }
    for (i = 0; i < count; i++) {
      switch (step) {
        case STEP_ARMY_CITIES:
          if (!Ov006_PickRandomCellNear(&x, &y, CELL_PLAIN, hq1, 10)) {
            goto next;
          }
          Ov006_SetMapCell(x, y, CELL_CITY_ARMY1);
          if (!Ov006_PickRandomCellNear(&x, &y, CELL_PLAIN, hq2, 10)) {
            goto next;
          }
          Ov006_SetMapCell(x, y, CELL_CITY_ARMY2);
          break;
        case STEP_NEUTRAL_NEAR_HQ:
          if (!Ov006_PickRandomCellNear(&x, &y, CELL_PLAIN, hq1, 8)) {
            goto next;
          }
          Ov006_SetMapCell(x, y, CELL_CITY);
          city1->x = x;
          city1->y = y;
          if (!Ov006_PickRandomCellNear(&x, &y, CELL_PLAIN, hq2, 8)) {
            goto next;
          }
          Ov006_SetMapCell(x, y, CELL_CITY);
          city2->x = x;
          city2->y = y;
          break;
        case STEP_NEUTRAL_MIDDLE:
          if (!Ov006_PickRandomCellBetween(&x, &y, CELL_PLAIN, hq1, hq2, 10)) {
            goto next;
          }
          Ov006_SetMapCell(x, y, CELL_CITY);
          middle->x = x;
          middle->y = y;
          break;
        case STEP_COM_TOWERS:
          if (!Ov006_PickRandomCellBetween(&x, &y, CELL_PLAIN, hq1, hq2, 5)) {
            goto next;
          }
          Ov006_SetMapCell(x, y, CELL_COM_TOWER);
          middle->x = x;
          middle->y = y;
          break;
      }
    }
  next:;
  }
}
