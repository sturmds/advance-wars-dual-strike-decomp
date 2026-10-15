# Markers and game state for emu.py scenarios

Functions to anchor on, functions that confirm a press, and memory to assert on, for the US ROM. Every name is in `config/AWRE/arm9/**/symbols.txt` and in the ELF of a build. Entries marked **(run)** were checked in a melonDS run (`tools/scenarios/war_room_deploy.inp`); the rest come from reading the source and the task scripts. Entries marked **(guess)** are inferred and not checked.

Paths: `src/game/main/menus/` is shortened to `menus/`, `src/game/main/map/` to `map/`. Task scripts live in `extracted/AWRE/data/` (written by `make setup`). In a task script, opcode 2 (`TASK_CMD_CALL`) calls a function once and opcode 3 (`TASK_CMD_SET_UPDATE`) calls it every frame until the step ends (`include/game/task.h`). In the tables, **once** means once each time the screen opens, **frame** every frame while it is up.

ov000 and ov004 load at 0x022ad560; ov001, ov002, ov003, ov005 and ov006 at 0x02350560 (retail addresses). A breakpoint on a function in one of them also stops in the others' code; `emu.py` compares the code at such a stop with the ROM's and ignores stops in another overlay's code. In a Design Room run, one stop at `Ov005_SetUpSoundRoom`'s address was ov006 code. Main anchors cost fewer stops.

`emu.py record` writes a comment for each function in `RECORD_MARKERS` (`tools/emu.py`), most of them from the tables below.

## 1. Screen anchors

| Screen | Anchor | Runs |
|---|---|---|
| Title, waiting for a press | `Ov004Task_RunPressStartScreen` (ov004) **(run)** | frame; A, Start or touch ends it |
| Title, before the press screen | `Ov004Task_InitBlendedScene` (ov004) | once |
| Main menu built | `MainMenu_Setup` **(run)** | once, before the 56-frame opening; presses made then are lost |
| Main menu page usable | `MainMenu_AddEntryButtons` | once per page shown, after the opening and after each page flip |
| Main menu page N built | `MainMenu_BuildPageN` (`MainMenu_BuildPage2` **(run)**) | once at setup (page 0, 10 frames after `MainMenu_Setup` **(run)**) and once at a flip's midpoint; the page is not usable yet |
| Map select (War Room, Versus, Survival) | `MapSelect_Init` **(run)**, then `MapSelect_Update` | once, then frame |
| War Room started from the menu | `func_0210322c` **(run)** | once; sets gameMode 1 |
| Versus | `Versus_Start` | once |
| Survival | `Survival_StartRun` | once |
| Campaign world map | `CampaignMap_Init` | once |
| Combat manual | `CombatManual_Init` | once |
| Design Room (map editor) | `DesignRoom_Enter` (main), then `DesignRoom_Init` (ov006) **(run)** | once each; section 6 |
| Battle Maps shop | `Ov005_SetUpShopScreen` (ov005) | once |
| Sound Room | `Ov005_SetUpSoundRoom` (ov005) | once |
| History | `HistoryScreen_Init` | once |
| CO select | `CoSelectScreen_Create` **(run)** | once per entry |
| CO select ready | `CoSelectScreen_RestoreButtons` | once after the slide-in; again on return from Force Rank |
| Setup phase (War Room, before Deploy) | `@ mem data_02183b08+0xba u8 == 1` **(run)**; no function marks it | |
| Setup ends (Deploy accepted) | `func_020e88ac` **(run)** | once |
| Turn start, any player | `Game_State14_PickTurnState` **(run)** | once per turn, the Setup phase included; it sets state 15 (human) or 16 (AI). `CurrentPlayer_StartTurn` also runs once per turn, but twice at battle start and once in `DesignRoom_Init` |
| Human turn | `Game_State15_RunStep` **(run)** | frame, after `Game_State14_PickTurnState` |
| Free map cursor | `Game_State15_FreeCursor` | frame, in sub-state 0 |
| AI turn | `AiTurn_Start` once, then `AiTurn_Update` **(run)** | once, then frame |
| Turn ends (human or AI) | `func_020ddb38` | once per turn |
| Next player chosen | `Turn_AdvanceToNextPlayer` | once per turn |
| New day | `Turn_IncrementDay` **(run)** | once per day |
| Unit picked to move | `MapCursor_SelectTile` **(run)** | once per A on a unit |
| Unit action menu | `UnitMenu_Open` | once per opening |
| Build menu (factory) | `BuildMenu_Open` | once |
| Battle menu (A on an empty tile, or Select) | `func_020bc56c` | once |
| Setup menu (A in the Setup phase) | `func_020bc5c0` **(run)** | once |
| Attack starts | `ActingUnit_StartAttack` | once per attack, human and AI |
| Battle animation (Visual A to C) | `AttackTask_StartScene` (main), then `Battle_StartScreen` (ov002) | once |
| Attack with Visual off | `func_020dc698` | once |
| Damage applied | `Combat_ApplyResult` | once per attack, after the animation if one plays |
| Victory or defeat | `Battle_ShowResult`, then `func_020d69e4` (win) or `func_020d69c0` (loss) | once |
| Results screen | `ResultScreen_Init` | once |
| Yes/No prompt ("Are you ready?", Design Room save) | `ChoicePrompt_Init` **(run)**, then `ChoicePrompt_Update` **(run)** | once, then frame |

Run twice at battle start (once for Setup, once after Deploy): `Map_InitBattle`, `CurrentPlayer_StartTurn`, `Battle_InitMapState`, and a few more.

Run every frame: the `Game_StateN_*` handler of the current state, `*_Update`, `MainMenu_DrawFrame`, `MapCursor_MoveByKeysFree`. `Game_RunStateFrame` skips every state handler while a menu is open (`MenuDepth_Get() != 0`, `menus/game_run_state_frame.c`), so `Game_State15_*` does not run while the unit, battle or setup menu is up.

Run only while a key is down: every `*_HandleKeys` (`MainMenu_HandleKeys`, `PopupMenu_HandleKeys`, `CoSelectScreen_HandleKeys`). They need `gKeypad->repeat != 0`, the selected button fully slid in and no touch (`menus/menu_dispatch_keys.c`).

## 2. Confirm functions

Use these after `until`: each runs when a press did what it should.

| Screen | Confirm |
|---|---|
| Title | `Ov004_SkipOnInput` (skips the intro), then `MainMenu_Setup` **(run)** |
| Main menu, any entry | `MainMenu_ConfirmEntry` |
| Main menu, up or down | `MainMenu_HandleKeys` **(run)** |
| Main menu page flip (left, right, L, R) | `MainMenu_StartPageFlip` |
| Map select | `MapSelect_PickMap`, then `MapSelect_ConfirmMap` **(run)** |
| CO select grid, A | `CoSelectScreen_OpenCoList` **(run)** |
| CO list, A | `CoSelectScreen_ConfirmCoList` **(run)** |
| CO select, Start (leave) | `CoSelectScreen_BeginExit` **(run)** |
| Setup menu: CO | `func_020bd50c`, then `func_020e8848`, then `CoSelectScreen_Create` **(run)** |
| Setup menu: Deploy | `func_020bd538`, then `func_020e8878` (the event script with the Yes/No prompt) |
| Any pop-up menu move | `PopupMenu_HandleKeys` **(run)** |
| Any pop-up menu item | `PopupMenu_CallItemHandler` |
| Yes/No prompt, left or right | `ChoicePrompt_DrawCursor` **(run)**; the answer goes to `gEventChoiceAnswer` (u16) |
| Map, A on own idle unit | `MapCursor_SelectTile` |
| Move destination, A | `UnitMoveSelect_TryMoveTo` |
| Unit menu entries | `UnitMenu_Wait`, `UnitMenu_Fire`, `UnitMenu_Capture`, `UnitMenu_Join`, `UnitMenu_Board`, `UnitMenu_Dive`, `UnitMenu_Emerge`, `UnitMenu_Send` |
| Build menu row | `BuildMenu_OnConfirm` (also runs when the unit costs too much) |
| Results screen | `ResultScreen_Confirm` |
| Campaign map (A or Start) | `CampaignMap_Confirm` |

## 3. Memory

### The map state, `data_02183b08`

The game reaches it through two pointers (`data_027c027c->unk_48[data_027c0284]`), but in a one-map battle it always means `data_02183b08` (0x61d0 bytes, struct `Unk_02080440_48` in `include/game/Unk_02080440.h`), so read that symbol directly. The second map of a two-map battle is `data_02189cd8`. `tools/ds.py show Unk_02080440_48` prints the whole layout. Its `unk_1c4` pointer always points at `+0x184` (`src/game/main/screen/screens_init_contexts.c`), so the fields of `Unk_02080440_1c4` sit at fixed offsets too.

| What | Offset | Type | Values |
|---|---|---|---|
| day | +0x0 | u16 | 1 on the first day **(run)** |
| current player | +0x2 | u16 | slot 1 to 4 **(run)** |
| open menus (menuDepth) | +0x8 | s8 | state handlers run only at 0 |
| game state | +0x18 | u16 | 1 day-limit check, 2 army icon, 3 next-turn screen, 4 song fade, 5 to 13 turn-start steps, 14 pick turn, 15 human turn **(run)**, 16 AI turn, 18 end check, 20 map over, 21 link, 22 reopen pop-up |
| state-15 sub-state | +0x1c | u16 | 0 free cursor, 1 picking a destination, 2 delete unit, 3 open unit menu, 4 unit menu up, 5 blocked move, 6 to 8 scroll |
| cursor x, y | +0x28, +0x2a | s16 | tiles |
| gameMode | +0x58 | u8 | 0 Campaign, 1 War Room **(run)**, 2 Versus, 3 Survival, 4 Design Room |
| visualMode | +0x82 | u8 | 0 off, 1 A, 2 B, 3 C, 4 D (`include/game/visual_mode.h`) |
| army colour of slot s (setup copy) | +0x8d+s | u8 | 1 Orange Star to 5 Black Hole (`include/game/player.h`) |
| controller of slot s (setup copy) | +0x92+s | u8 | 0 none, 1 human, 2 AI |
| COs of slot s (setup copy) | +0x97+s*2, +1 for the back CO | u8 | `enum CoId` |
| coSelectPhase | +0xba | u8 | 1 in the Setup phase, 0 after Deploy **(run)** |
| map width, height | +0x19e, +0x1a0 | u16 | |
| scrollFlags | +0x1ae | u16 | nonzero while the view scrolls |
| playerCount | +0x1b0 | u8 | |
| mapId | +0x1b8 | u16 | Spann Island = 0x8b **(run)** (`docs/map-terrain.md`) |
| fog | +0x1bc | u8 | 1 = Fog of War |
| weather | +0x1be | u8 | 0 clear, 1 snow, 2 rain, 3 sand |

The day shown on screen is forced to 1 in the Setup phase (`src/game/main/game_get_shown_day.c`).

### Players

`Player` structs (0x98 bytes, `include/game/player.h`) start at `+0x5760`, slot 0 being neutral. Player p is at `+0x5760 + p*0x98`. They are filled at battle start (`map/players_apply_battle_setup.c`).

| Field | In Player | p1 | p2 | p3 | p4 |
|---|---|---|---|---|---|
| funds (s32) | +0x00 | +0x57f8 | +0x5890 | +0x5928 | +0x59c0 |
| armyColour (u8) | +0x19 | +0x5811 | +0x58a9 | +0x5941 | +0x59d9 |
| controller (u8) | +0x1a | +0x5812 | +0x58aa | +0x5942 | +0x59da |
| unitCount (u8) | +0x35 | +0x582d | +0x58c5 | +0x595d | +0x59f5 |
| front CO id (u8) | +0x6c | +0x5864 **(run)** | +0x58fc | +0x5994 | +0x5a2c |
| back CO id (u8) | +0x70 | +0x5868 | +0x5900 | +0x5998 | +0x5a30 |

CO ids (`include/game/co_data.h`): 0 none, 1 Nell, 2 Andy, 3 Max, 4 Olaf, 5 Sami, 6 Grit, 7 Kanbei, 8 Sonja, 9 Eagle, 10 Drake, 11 Von Bolt, 12 Jugger, 13 Lash, 14 Koal, 15 Hawke, 16 Hachi, 17 Colin, 18 Jess, 19 Sensei, 20 Jake **(run)**, 21 Rachel, 22 Sasha, 23 Javier, 24 Grimm, 25 Kindle, 26 Flak, 27 Adder.

### Units

The unit array starts at `+0x495c` (absolute 0x02188464; `STATE_UNIT_SLOTS` in `include/game/battle_save.h`). Each unit is 14 bytes (`MapUnit`, `include/game/map_unit.h`). Each army owns a block of 64 slots: player p's block starts at slot `(p-1)*64`, and the game uses the 50 slots after the block's first, so player p's first unit is at `+0x496a + (p-1)*0x380` (p1 +0x496a, p2 +0x4cea, p3 +0x506a, p4 +0x53ea). A slot with `type == 0` is empty. The owner is not stored; it follows from the block (`MAP_UNIT_OWNER` in `include/game/Unk_02080440.h`).

| Field | Offset | Notes |
|---|---|---|
| type | +0 u8 | 1 Infantry, 2 Mech, 3 Md Tank, 4 Megatank, 5 Tank, 6 Recon, 7 APC, 8 Neotank, 9 Piperunner, 10 Artillery, 11 Rockets, 12 Stealth, 13 Black Bomb, 14 Anti-Air, 15 Missiles, 16 Fighter, 17 Bomber, 18 Black Boat, 19 B Copter, 20 T Copter, 21 Battleship, 22 Cruiser, 23 Lander, 24 Sub, 25 Carrier, 26 Oozium (`include/game/unit_stats.h`) |
| flags | +1 u8 | 0x01 waited, 0x08 carried, 0x20 dived |
| x, y | +2, +3 u8 | tile |
| hp | +6 u16, mask 0x7f | 100 = full |

No field counts all units on the map; `Player.unitCount` counts one army's. To find the unit on a tile, scan the slots for x and y.

### Other globals

| Symbol | Use |
|---|---|
| `data_0229c720` (`MainMenu`) | `cursors[14]` at +0, the cursor of each page; the current page is byte +0x0e & 0x0f (`include/game/main_menu.h`) |
| `gNextGameMode` (s32) | the mode chosen in the main menu |
| `data_0216e594` (`Keypad`) | what `gKeypad` points to: held +0, repeat +2, pressed +4 (u16 each, `include/game/keypad.h`) |
| `data_0229e178` (`AiTurnState`) | what `gAiTurnState` points to; state u8 at +0: 0 start, 1 phase, 2 queue, 3 show, 4 end actions, 5 power, 6 restart (`include/game/ai_unit_action.h`) |
| `gEventChoiceAnswer` (u16) | answer of the last Yes/No prompt; Yes is 0 and No is 1 **(guess)** |
| `gRngState` | the RNG |
| `gPlayRecords` | save-wide records (`include/game/play_records.h`) |

## 4. Menus

**Main menu** (`menus/main_menu_handle_keys.c`): up and down step on `repeat`, skip hidden entries and wrap. Left, right, L and R flip between page 0 and page 6. A confirms, B cancels. On an empty save the cursor starts on entry 0 of page 0 **(run)**. On pages 1 to 5 it starts on entry 1 (continue) when that mode has a battle save, else on entry 0 (`menus/main_menu_setup.c`).

| Page | Entries (hidden ones in brackets) | Reached from |
|---|---|---|
| 0 | 0 Campaign, 1 War Room **(run)**, 2 Versus, 3 Survival, 4 Combat | first side |
| 1 Campaign | 0 New, [1 Continue] | page 0 entry 0 |
| 2 War Room | 0 New (`func_0210322c`) **(run)**, [1 Continue (`MainMenu_StartWarRoom`), shown when a War Room battle save exists] | page 0 entry 1 |
| 3 Versus | 0 New (`Versus_StartVsCpuOneMap` or `Versus_StartVsCpuTwoMaps`), [1 Continue] | page 0 entry 2 |
| 4 Survival | 0 New (funds, days or time), [1 Continue] | page 0 entry 3 |
| 5 Combat | 0 Start, [1], 2 Manual (`CombatManual_Init`) | page 0 entry 4 |
| 6 | 0 Battle Maps shop, 1 History, 2 Wireless, [3 Sound Room], [4 Gallery], 5 Design Room **(run)** | second side |
| 8 Design | 0 `Ov005_InitCoScreen` (Color Edit **(run)**), 1 `DesignRoom_Enter` (map editor) **(run)**, 2 `Ov005_SetUpCoListScreen` | page 6 entry 5 |

Page labels come from the functions each entry starts; those marked **(run)** were also read on screen. A save keeps each page's cursor **(run)**: after the Design Room session below, page 6 opens on Design Room, and up from there goes to Wireless.

**Pop-up menus** (setup, battle and unit menus, `menus/popup_menu_handle_keys.c`): up and down move on `repeat` and wrap only on a new press, so a held key stops at the end. A confirms only on a frame with no move.

**Setup menu**: CO, Intel, Options, Save, Deploy **(run)**. Deploy is one up press from CO. The "Are you ready?" prompt starts on No; left moves to Yes **(run)**.

**CO select grid** wraps (`menus/co_select_screen_handle_keys.c`). **Map select**: the d-pad moves within the page; in the War Room, left and right turn the sub strip instead of moving between map buttons (`menus/map_select_handle_keys.c`).

**Key repeat**: a held key repeats after 12 frames, then every 4 (`src/game/main/keypad_init.c`). A press shorter than 12 frames gives one step.

## 5. Map cursor

- In sub-state 0, `MapCursor_MoveByKeysFree` moves the cursor one tile per `repeat` event, kept on the map (`map/map_cursor_move_by_keys_free.c`).
- It does nothing while the view scrolls (`scrollFlags & 0xf`), so a press at the edge of the view can start a scroll and move nothing.
- Inside a shown move range, auto-repeat stops at the range's edge; a new press steps out (`map/map_cursor_move_by_keys.c`).
- L jumps to the current player's next unit that has not waited. A on an empty tile, or Select, opens the battle menu.
- Each turn the cursor starts on the player's saved tile, the HQ on the first day (`map/current_player_start_turn.c`).

No function runs only when the cursor moves, so drive it by memory:

```
@ Game_State15_FreeCursor
press right until mem data_02183b08+0x28 s16 == 5
press down until mem data_02183b08+0x2a s16 == 3
press a until MapCursor_SelectTile             # sub-state becomes 1
```

## 6. Design Room

Checked in a replay of a recorded session (**(run)** throughout):

- `DesignRoom_Enter` (main), then two frames later `DesignRoom_Init` (ov006) and `Ov006_NewMap`. A "Welcome to the map editor!" box opens on every entry; A closes it. An empty save also plays instructions on the first entry.
- Select opens a menu of File, Help, Fill and Exit. A on File runs `Ov006_OpenPopupMenu`: Load, Save, NameEntry. Load runs `Ov006_OpenLoadMenu`, Save `Ov006_OpenSaveMenu`; both list the three map slots, "No data exists." for an empty one.
- In the save menu, A on a slot and A again bring a Yes/No prompt (`ChoicePrompt_Init`); left and A on Yes run `DesignMap_SaveToSlot` (main), which writes the map as save record 7 + slot.
- `Ov006_UpdateMapEditor` runs every frame. While A is down it calls `Ov006_PaintTerrainAtCursor` (terrain and buildings) or `Ov006_PlaceUnitAtCursor` (units) every frame. B calls `Ov006_PickToolAtCursor` (the tool palette).

```
press a until Ov006_OpenPopupMenu              # after select, on File
press a until Ov006_OpenLoadMenu               # Load
```

Not checked: the order of unit menu items (built per unit), and the on-screen labels of menu entries.
