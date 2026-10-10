#ifndef GAME_MAP_SCRIPT_H
#define GAME_MAP_SCRIPT_H

#include "types.h"

/* One 8-byte command of a map script list. Opcodes 0-25 are handled by
 * EventTriggers_Run; 26 ends a list. */
typedef struct MapScriptCommand {
    u8 opcode;
    u8 pad_01[7];
} MapScriptCommand;

/* What MapHeader.unk_00 points to: lists of commands for the situations a
 * campaign map reacts to. Which situation each list belongs to is not known;
 * the runners are named by list offset. */
typedef struct MapScript {
    const MapScriptCommand *list00;    /* run by func_020ba198 when a turn starts (after the player theme) */
    u8 pad_04[0x10 - 0x04];
    const MapScriptCommand *list10;    /* run with an id and a unit by MapScript_RunList10 */
    const MapScriptCommand *list14;    /* run by func_020cf840 */
    const MapScriptCommand *list18;    /* run by func_020c05d8 with a flag */
} MapScript;

#endif
