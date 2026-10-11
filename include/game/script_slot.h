#ifndef GAME_SCRIPT_SLOT_H
#define GAME_SCRIPT_SLOT_H

#include "types.h"

/* One 8-byte command of a slot script. Handlers in gScriptCommands read
 * the two arguments and advance ScriptSlot.pc. */
typedef struct ScriptCommand {
    u32 arg;                           /* opcode 1: function stored in ScriptSlot.callback */
    u16 arg16;                         /* opcode 0: frames to wait */
    u16 opcode;                        /* index into gScriptCommands */
} ScriptCommand;

/* One of 30 script slots at data_027c027c->scriptSlots. The handlers call
 * float and fixed-point helpers, so the scripts probably animate
 * something. */
typedef struct ScriptSlot {
    ScriptCommand *start;              /* NULL: slot unused */
    ScriptCommand *pc;                 /* next command */
    void (*callback)(struct ScriptSlot *slot);   /* set by opcode 1; while set, the script does not advance; ScriptSlot_RunPending calls it */
    void (*onStop)(struct ScriptSlot *slot);   /* called when the slot is stopped */
    u16 wait;                          /* frames left before the next command */
    u16 unk_12;                        /* set to 4 when a script starts, before its first run */
    u8 priority;                       /* low 7 bits: slots with lower values run first */
    u8 pad_15[0x18 - 0x15];
    u16 *textEntry;                    /* text printer: screen map entry the effect draws at */
    u8 pad_1c[2];
    s16 unk_1e;                        /* text printer effects: mode (0 or 1); 10 for the code 14 slot, which func_02015ea8 counts down */
    u16 unk_20;
    u8 pad_22[0x26 - 0x22];
    s16 unk_26;                        /* passed to func_0201751c when the script stops with bit 2 of unk_12 set */
    int unk_28;                        /* text printer: the cursor offset from func_02015ec8 */
    void *unk_2c;                      /* text printer: TextPrinter.unk_40 */
    u16 unk_30;                        /* text printer: TextPrinter.unk_30 */
    u8 pad_32[0x38 - 0x32];
    s16 unk_38;                        /* service cost popup (ServiceCostPopup_Start, ServiceCostPopup_SetShown): set from the last argument of ServiceCostPopup_Start; its meaning is a guess: whether the popup is shown */
    u8 pad_3a[0x3e - 0x3a];
    u8 unk_3e;
    u8 pad_3f[0x60 - 0x3f];
} ScriptSlot;

/* The same slot as the service cost popup script data_021666e8 uses it
 * (ServiceCostPopup_Start). */
typedef struct ServiceCostPopupSlot {
    u8 pad_00[0x1e];
    s16 tileX;                         /* tile of the serviced unit */
    s16 tileY;
    int cost;                          /* funds the service costs */
    int fundsAfter;                    /* the current player's funds less the cost */
    int step;                          /* taken off cost each frame while the popup counts down */
    u32 chargeFunds;                   /* 1: funds go down with the cost as it counts down */
    u8 zoomVariant;                    /* ZoomSpriteTask_Start variant */
    u8 unk_35;                         /* ServiceCostPopup_Begin: 0, or 16 less a cost below 16 (16 if that is negative) */
    u8 pad_36[0x38 - 0x36];
    int visible;                       /* nonzero: the tile is visible, so the zoom sprite and popup task run; see ScriptSlot.unk_38 */
    u8 pad_3c[0x60 - 0x3c];
} ServiceCostPopupSlot;

/* The same slot as the supply command uses it (Supply_FlagAdjacentTargets,
 * Supply_RefillAdjacentStep). */
typedef struct SupplySlot {
    u8 pad_00[0x1e];
    s16 side;                          /* next side of the acting unit to supply, 0-3 */
    s16 unk_20;                        /* 0: at the end, unk_28/unk_2a get the acting unit's tile and unit action 6 is sent when gLinkPlay is set */
} SupplySlot;

/* The same slot as the callback of the hook script data_02166948 uses it. */
typedef struct MapViewSlot {
    u8 pad_00[0x20];
    s16 scrollX;                       /* map view scroll the callback compares with the current one */
    s16 scrollY;
} MapViewSlot;

/* The same slot as the map scroll script gMapScrollScript uses it. Positions
 * are map view scroll values in 1/16 tiles. */
typedef struct MapScrollSlot {
    u8 pad_00[0x1e];
    s16 startX;                        /* scroll when the script starts */
    s16 startY;
    s16 targetX;                       /* set by MapScroll_Start */
    s16 targetY;
    s16 distX;                         /* targetX - startX */
    s16 distY;
    u16 step;                          /* index into the per-frame speed table gMapScrollSpeeds */
    u16 travelled;                     /* distance moved so far along the longer axis */
    u8 pad_2e[0x60 - 0x2e];
} MapScrollSlot;

/* The same slot as the map overview script data_02166948 uses it. */
typedef struct MapOverviewSlot {
    u8 pad_00[0x1e];
    s16 frame;                         /* frames of the fade so far (MapOverview_DimStep) */
    s16 savedScrollX;                  /* map view scroll when the overview opens */
    s16 savedScrollY;
} MapOverviewSlot;

/* The same slot as the battle resume script data_02158468 uses it
 * (Battle_ResumeFromSave); func_02017cf8 sets recordId. */
typedef struct BattleResumeSlot {
    u8 pad_00[0x1e];
    s16 recordId;                      /* battle save record to restore, 0: none */
} BattleResumeSlot;

/* The same slot as the screen wipe callbacks (ScreenWipe_*) use it. */
typedef struct WipeSlot {
    u8 pad_00[0x08];
    void (*callback)(struct WipeSlot *slot); /* cleared when the wipe ends */
    u8 pad_0c[0x1e - 0x0c];
    s16 frame;                         /* wipe step, 0 to WIPE_LAST_FRAME */
} WipeSlot;

/* Argument block of the arc move command (ScriptSlot_CmdMoveArc),
 * reached through ScriptCommand.arg. */
typedef struct ArcMoveArgs {
    float dx;                          /* target, relative to the start position */
    float dy;
    float velX;                        /* constant horizontal speed per frame */
    float gravity;                     /* added to velY every frame */
} ArcMoveArgs;

/* Argument block of the accelerated move command, reached through
 * ScriptCommand.arg. */
typedef struct AccelMoveArgs {
    float velX;                        /* starting speed per frame */
    float velY;
    float accelX;                      /* added to the speed every frame */
    float accelY;
    float frames;
} AccelMoveArgs;

/* Ends the point list of the path move command. */
#define PATH_END (-1)

/* The same slot as the arc move command uses it. */
typedef struct ArcMoveSlot {
    ScriptCommand *start;
    ScriptCommand *pc;
    u8 pad_08[0x38 - 0x08];
    s16 started;                       /* 0: read the start position on the next run */
    u8 pad_3a[2];
    float x;
    float y;
    float targetX;
    float targetY;
    float velX;
    float velY;
    float accelX;
    float accelY;
    int framesLeft;
} ArcMoveSlot;

/* Slot `slot` seen as an ArcMoveSlot, for the move commands. Needed to
 * match: reading scriptSlots through this cast makes each access load the
 * slot address again, as the original does. */
#define MOVE_SLOT(slot) ((*(ArcMoveSlot **)&data_027c027c->scriptSlots)[slot])

/* The slot's script data area as ScriptSlot_Init clears it: four halfwords
 * set to 0 and four words set to -1. Each script uses the area in its own
 * way (see the views above). */
typedef struct ScriptSlotVars {
    u8 pad_00[0x1e];
    s16 shorts[4];
    u8 pad_26[2];
    int words[4];
    u8 pad_38[0x60 - 0x38];
} ScriptSlotVars;

/* Runs one command of the slot; nonzero: run the next one this frame too. */
typedef BOOL (*ScriptCommandFunc)(int slot);

#endif
