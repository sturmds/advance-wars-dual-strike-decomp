---
name: emu-drive
description: Test a change to the game's behaviour by playing it in melonDS with tools/emu.py. Write a scenario script that presses keys and waits on game functions, run it with no window, check game memory with expect lines and look at the screenshots. Use it after a mod that changes what the game does, or to reproduce a bug the user reports.
---

# Drive the game in melonDS

`tools/emu.py` plays a scenario script in melonDS with no window, through melonDS's GDB stub. It holds keys at the game's keypad read, waits for functions to run or for memory to hold a value, checks memory, and saves screenshots. Its docstring describes every option and step; read it once before writing a script. Facts about the game's screens, functions and memory are in `reference.md` beside this file.

## When to use it

- A mod changes what the game does (a rule, a unit, a menu, a map) and you need to see that it works.
- The user reports a bug that shows up while playing.
- Not for byte matches (`make release` checks those) or for crashes on boot (`make emu-check`).

## Rules

- Assert on game state (`expect mem`, `expect reach`), never on frame numbers. Frames vary by about 15 between runs of one ROM and by hundreds between ROMs.
- Anchor each screen on a function that runs when it opens, and retry each press `until` something shows it arrived.
- Scripts that belong in git go in `tools/scenarios/` and name only functions and data symbols, never addresses. Throwaway scripts go in your scratch directory.
- Never `pkill melonDS` or kill it by name: the user may have their own melonDS open. `emu.py` runs melonDS in a session of its own and stops it on exit, Ctrl-C included. If you must stop a run, kill the `emu.py` process or the process group you started.
- Writing: plain and short, as in `CLAUDE.md`.

## 1. Build the ROM

Build the ROM with the branch's usual target. `emu.py` takes `build/AWRE.nds` with `build/arm9.elf` by default. For retail, pass `orig/AWRE.nds`; its symbols come from `config/AWRE/arm9/`. Running both shows what the change did.

## 2. Find markers and fields

- `reference.md` lists the anchors, confirm functions and memory fields that were checked.
- `python3 tools/fn.py show NAME` shows a function's callers and callees. Read the caller to see whether a function runs once per screen or every frame (task scripts in `extracted/AWRE/data/`: opcode 2 calls once, opcode 3 calls every frame).
- `python3 tools/ds.py show STRUCT` gives a struct's offsets; `ds.py show data_XXXXXXXX` gives a data symbol's type; `ds.py field Struct.field` lists who reads and writes a field.
- `grep NAME config/AWRE/arm9/symbols.txt config/AWRE/arm9/overlays/*/symbols.txt` gives the address and module.
- `mem` reads a data symbol plus an offset. It cannot follow a pointer, so read the object the pointer points to (`reference.md` names some).

## 3. Write the scenario

Start from `tools/scenarios/war_room_deploy.inp`. The shape of each screen:

```
@ MapSelect_Init                               # wait for the screen to open
+60 press a until MapSelect_ConfirmMap         # press, retry every 30 frames until it runs
expect mem data_02183b08+0xba u8 == 1          # check the game state now
```

- `@ WHAT` waits for a function or a `mem` condition. `+N` counts frames from the end of the previous step.
- `press KEYS until WHAT [tries N] [every N]` holds the keys for 4 polled frames, then presses again until WHAT happens. Raise `tries` where dialogue or an animation eats presses.
- `press KEYS until mem ...` drives a cursor: one tap per tile until the cursor field holds the value.
- `expect reach FUNCTION within N` fails the run when the function does not run in time.
- `shot NAME` saves both screens; `end` stops with a pass.

## 4. Run and read

```sh
python3 tools/emu.py run tools/scenarios/war_room_deploy.inp                   # build/AWRE.nds
python3 tools/emu.py run tools/scenarios/war_room_deploy.inp orig/AWRE.nds     # retail
```

Exit 0: every step passed. 1: a step failed. 2: the script or a name in it is wrong, or melonDS did not start. Output goes to `build/emu/<script stem>/`:

- `<tag>.log`: each step with the frame it ended at, and on a failure the step, the value read and where the ARM9 was (pc, lr).
- `<tag>-NAME.png` for each `shot`, and `<tag>-fail.png` at a failure. Look at them with the Read tool.

The tag comes from the ROM's path (`build-AWRE`, `orig-AWRE`), so runs on two ROMs keep their own files. Screenshots need PIL (`python3-pil`). The War Room scenario takes 15 to 20 s.

When a step fails, read the fail PNG first: it shows which screen the game is on. Then check the log's pc and lr against `fn.py show`.

## 5. Check a mod's effect

Drive to where the mod acts, then assert on the field it changes:

```
expect mem data_02183b08+0x57f8 s32 == 9000    # player 1's funds
expect mem data_02183b08+0x5864 u8 == 26       # player 1's CO: Flak
```

Run it on retail too and expect the old value there, or keep a second script; a test that passes on both ROMs does not show the mod did anything. Add a `shot` where a person would want to see the result.

## 6. Ask for a recording

Ask the user to record when the steps are hard to write by hand: a long menu path, a bug that needs a sequence of moves, or a screen with no clear anchor.

```sh
python3 tools/emu.py record tools/scenarios/NAME.inp [ROM[=ELF]]
```

It opens a window with the user's key bindings and writes a `VBLANK KEYS` line at every key change, plus `# @ FUNCTION (frame N)` comments when a known screen function runs. `emu.py run` replays it, but only on the ROM and save it was made on: a ROM change that moves load timing breaks it. To make it last, turn it into anchored steps:

1. Split the key lines at each `# @` comment.
2. Replace each comment with `@ FUNCTION`, and each press after it with `+N press KEYS until CONFIRM`, where N is its frame minus the comment's frame and CONFIRM is the function or memory change that shows the press worked (`reference.md`, section 2).
3. Add `expect` lines for the state the user cared about, and run it on both ROMs.

`emu.py play [ROM]` opens a window with nothing scripted; the save it leaves in `build/emu/play/save/` can start a run with `--save`.

## Pitfalls

- **Fresh hits.** A wait or `until` counts only a hit after its step started. A function that ran just before does not count, and a breakpoint that is never hit again waits until `within` or `--frames` runs out.
- **Functions that run twice.** `Map_InitBattle`, `CurrentPlayer_StartTurn`, `Battle_InitMapState` and a few others run once for the Setup phase and again after Deploy. Anchor on `func_020e88ac` (Setup ends) instead.
- **`*_HandleKeys` runs only while a key is down** (and the selected button has slid in). It shows a press reached the screen, not that the screen is ready.
- **State handlers stop while a menu is open.** `Game_State15_*` and the other `Game_StateN_*` handlers do not run while the unit, battle or setup menu is up, so a wait on them after opening a menu never ends.
- **Overlays share addresses.** ov000 and ov004 load at one address; ov001, ov002, ov003, ov005 and ov006 at another. A breakpoint on an overlay function also stops in whichever overlay is loaded there. `emu.py` prints a note for each one; anchor on main functions where you can.
- **Retail's empty save.** Only Jake can be picked in the War Room, and Rachel's tutorial plays on the first visit to CO select. Retries with A clear it (`tries 40` in the War Room scenario). A ROM that unlocks every CO has no tutorial.
- **Recordings are exact only for their ROM and save.** Keep anchored scripts for anything that must outlive one build.
- **Pop-up menus wrap only on a new press.** A held key stops at the last entry.
