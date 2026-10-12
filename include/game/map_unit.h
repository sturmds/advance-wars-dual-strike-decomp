#ifndef GAME_MAP_UNIT_H
#define GAME_MAP_UNIT_H

#include "types.h"

/* MapUnit.flags. From romhacking.net doc 890 (RadioTails); only the bits
 * named in code comments have been seen used. */
#define MAP_UNIT_WAITED 0x01
#define MAP_UNIT_SELECTED 0x02
#define MAP_UNIT_LOCKED 0x04
#define MAP_UNIT_CARRIED 0x08
#define MAP_UNIT_CARRYING 0x10
#define MAP_UNIT_DIVED 0x20
#define MAP_UNIT_PARALYSED 0x40

/* MapUnit.captureProgress at which a tile changes owner. */
#define MAP_UNIT_CAPTURE_POINTS 20

/* Unit slots per player in Unk_02080440_48.units. */
#define MAP_UNITS_PER_PLAYER 64

/* First slot in Unk_02080440_48.units for each player slot: 0, 0, 64,
 * 128, 192. */
extern u16 Ov000_gPlayerFirstUnitSlot[5];

/* MapUnit.aiTargetX and aiTargetY when the unit has no target. */
#define MAP_UNIT_NO_TARGET 63

/* A unit on the map, 0xe bytes. */
typedef struct MapUnit {
  u8 type;  /* enum UnitType */
  u8 flags; /* MAP_UNIT_* */
  u8 x;
  u8 y;
  u8 cargo[2]; /* slot of each carried unit */
  u16 hp : 7;  /* 100 = full */
  u16 ammo : 4;
  u16 captureProgress : 5;
  u8 fuel : 7;
  u8 unk_08_7 : 1;
  u8 pad_09;
  u16 aiTargetX : 6; /* MAP_UNIT_NO_TARGET when unused */
  u16 aiTargetY : 6;
  u16 serviceWanted
      : 2; /* set by the AI: 1 the unit wants supplies (AiUnit_GoToSupply), 2 repairs (AiUnit_HeadForRepairTile) */
  u16 transportWanted
      : 2; /* set by the AI on a foot unit: 1 it waits for an APC, 2 for a T-Copter; the Lander and Black Boat builders count foot units (Ai_CountFootWantingTransport) and vehicles (Ai_CountVehiclesWantingTransport) with 2 as waiting for sea transport */
  u8 aiOrder : 4; /* 8, 9 and 10 use aiTargetX and aiTargetY */
  u8 unk_0c_4 : 1;
  u8 unk_0c_5 : 1;
  u8 unk_0c_6 : 1;
  u8 claimedLandmass
      : 1; /* AI: the unit stays to hold its landmass (AiUnit_ClaimLandmass) */
  u8 pad_0d;
} MapUnit;

typedef char MapUnit_size_check[sizeof(MapUnit) == 0xe ? 1 : -1];

#endif
