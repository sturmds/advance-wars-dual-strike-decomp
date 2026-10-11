#ifndef GAME_DESIGN_MAP_GEN_H
#define GAME_DESIGN_MAP_GEN_H

#include "types.h"

/* Scratch marks of the Design Room random map generator, one byte per tile
 * in rows of 30 (Ov006_MarkMapBorder, Ov006_SettleSeedTiles,
 * Ov006_MarkLinkPath). Read signed. */
extern s8 Ov006_gGenMarks[][30];

#endif
