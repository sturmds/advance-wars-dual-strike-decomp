#ifndef GAME_MAP_OBJECT_H
#define GAME_MAP_OBJECT_H

#include "types.h"

/* A large object on the map that units can attack (kinds 0-13, for
 * Combat_CalcDamageToTarget), found by tile with Map_GetObjectAt. 8 bytes
 * in Unk_02080440_48.objects. */
typedef struct MapObject {
    u8 x;
    u8 y;
    u16 width : 3;                     /* footprint in tiles */
    u16 height : 3;
    u16 kind : 4;                      /* damage chart target kind */
    u16 startState : 4;                /* state copied to state when that is 0 (MapObjects_InitState) */
    u16 variant : 2;                   /* index into data_02167e98 for kind 2 */
    u8 hp;                             /* 0: destroyed (Map_GetObjectAt then returns NULL) */
    u8 unk_05;
    s8 state;                          /* Ai_AddObjectThreat handles the objects where this is 1 */
    u8 unk_07;
} MapObject;

/* Object kinds named so far (terrain id the kind is placed for). */
enum MapObjectKind {
    MAP_OBJECT_VOLCANO = 2,            /* terrain 28 */
    MAP_OBJECT_BLACK_CANNON = 3,       /* terrain 26 */
    MAP_OBJECT_MINICANNON = 4,         /* terrain 21 */
    MAP_OBJECT_DEATHRAY = 5,           /* terrain 30 */
    MAP_OBJECT_BLOCKED = 8,            /* terrain 31: a missile platform or fortress, created with 0 HP */
    MAP_OBJECT_BLACK_CRYSTAL = 9,      /* terrain 23 */
    MAP_OBJECT_BLACK_OBELISK = 10,     /* terrain 29 */
    MAP_OBJECT_GRAND_BOLT_WEAK_POINT = 11  /* terrain 24; 11 to 13, one per owner */
};

/* TRUE when the current screen's map has a live object of this kind. */
BOOL MapObjects_HasKind(int kind);
void MapObject_ClearVariant(MapObject *object);
BOOL MapObject_IsStanding(MapObject *object);
MapObject *Map_GetObjectAt(int x, int y);

#endif
