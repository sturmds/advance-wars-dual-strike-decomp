# AWDS Decomp: Early-Phase Project Plan

Plan for a matching decompilation of *Advance Wars: Dual Strike*, US/AUS cartridge (game code `AWRE`). The goal of this plan is not a 100% decompiled project with perfect translation. The goal is a repo that rebuilds the game byte for byte, grows C where it is useful, and eventually serves as a base for modding the game.

Background:

- NDS decomp survey (not in the repo): how pokeplatinum and khdays are built, plus the seven agreed decisions (D1–D7) this plan follows.
- pokefirered teardown (not in the repo): the GBA reference project.

## Ground rules (from the survey decisions)

| Rule | Source |
|---|---|
| Matching build only. US/AUS `AWRE` only. ARM9 and ARM7 linked from objects (ARM7 since 2026-10-11); the file system copied from the ROM until a later milestone. | D1, D3 |
| Linux only. No Windows executables, no wine, no wibo. Every tool locked to an exact version and hash. | D2 |
| `make release` works as the first command after cloning, and works offline after the first `make setup`. | D2, D6 |
| Every commit passes `make release`, enforced by a git hook in the repo. | D4 |
| Developed locally, without decomp.me or decomp.dev. The repo is published by sturmds only after its whole history passes the privacy checks (tenets 8-10). Agents never push. | D4, D6 |
| Git holds only what we write. The ROM, raw asset files, compiler and SDK files never enter git, and neither does game data in a text form we edit: the maps are unpacked from the builder's ROM into `extracted/` by `make setup`, the way zeldaret/oot handles its assets. Committed are only the files that say how to read and order the data (`assets/maps/legend.yaml` and the glue files). | D5 |
| Mostly automated matching, steered by sturmds. Generated code passes `make release` and an automated review before it is offered. | D7 |

## Milestone 0: Foundations

M0 gives every later milestone a repo to work in, rules the automation follows, and a build that already matches.

### 0.1 Tenets and prompts

When the repo is created (`git init` in `~/repos/`), its first commit includes this plan at `docs/early-phase-project-plan.md`. From then on, the copy in the repo is the one to edit.

Most of the work here is writing instructions, not code: the project tenets, `CLAUDE.md`, and the prompts that keep automated work inside the decisions above.

- **`TENETS.md`**: short, numbered, editable over time. Starting draft to revise:
  1. A change that breaks `make release` is not done, whatever else it achieves.
  2. Match first, then make it readable. Never trade a match for style. Never stop at a match when the meaning is known.
  3. No magic offsets once a struct is known. `actor->hp`, not `*(s16 *)(self + 0x2c)`.
  4. Names describe behavior. Address names (`func_02012345`) are allowed only until the behavior is understood.
  5. A comment explains why the code has an odd shape ("needed to match: the compiler keeps this temp in r4"), not what the next line does.
  6. Write plainly in comments, docs and commit messages. No filler words, no hype. Write it the way you'd say it to a teammate.
- **`CLAUDE.md`**: how the project builds and tests, the ground rules above, where things live, and the "decompile one function" workflow from 0.4.
- **Review pass**: a prompt or skill that every generated change goes through before it reaches sturmds. It checks the C against the tenets (types, names, offsets, needless casts) and the writing against a banned-word list. The list starts from [archiewood/claudeisms](https://github.com/archiewood/claudeisms) (for example: load-bearing, verdict, scaffold, settles, genuinely, drift, wired, canonical, seamless).
  - Banned constructions, not just words. Above all the aphorism "X is the Y" ("the hash is the test", "the checksum is the test suite", "the linker script is the map"). Say what the thing does instead: "`make release` fails if any byte differs."
- **Git hooks** in `hooks/`, installed by `make setup` through `core.hooksPath`:
  - `pre-commit`: refuses `.nds`, `.bin`, `.o`, `.exe` and anything under `orig/`, `extract/`, `build/`.
  - `pre-commit` also runs `make release` (decided 2026-10-06; `pre-push` would never run, as there is no remote).
  - `commit-msg`: refuses words from `docs/style/banned.txt`.

Open question: what the first tenets are. Treat the list above as a draft for sturmds to rewrite.

### 0.2 Toolchain and compiler fingerprint

This step runs before the build exists, because it decides whether the project is feasible (D6).

1. Write `tools.lock`: tool name, version, download URL and SHA-256 for dsd and objdiff-cli, then the compiler once known. `make setup` downloads into a gitignored cache and does nothing if every hash already matches.
2. Place the ROM at `orig/AWRE.nds` (gitignored). Check its hash against the value recorded in the repo.
3. Run `dsd rom extract` and `dsd init`. Record the real `AWRE` facts: overlay count, function count, code size, SDK version string.
4. Fingerprint the compiler: compile small candidate functions with every native-Linux `mwccarm` build available (metroskrew's set first) and flag set, until one reproduces the original bytes. The EU notes claim "2.0 build 56", which metroskrew does not include. If the claim holds for `AWRE`, finding a native source for that build becomes the next task.

Done when: one compiler build and flag set reproduces at least one function, and it runs natively on Linux.

Stop and reassess if no native-Linux compiler matches anything.

Result (2026-10-06): mwccarm 2.0-72 (builds 72–82 are equivalent) with `-O4,p -proc arm946e -interworking` matches game, SDK and ITCM functions natively. Details in `docs/awre-facts.md`.

### 0.3 Matching rebuild with zero C

- `configure.py` generates `build.ninja` from dsd's `config/AWRE/arm9/**` (`delinks.txt`, `symbols.txt`, `relocs.txt`).
- ARM9 main and every overlay are delinked into objects and linked with the original `mwldarm`, using the LCF from `dsd lcf`.
- ARM7 and the file system are copied from the ROM as-is. (ARM7 has been linked from objects since 2026-10-11, issue 0013.)
- The ROM is repacked, and `make check` compares per-module hashes, then the whole-ROM SHA-1. No exemption for header CRC bytes (khdays allows 4).

Done when (M0 expected outcome): with a valid ROM at the gitignored path, `make release` on a new clone produces an `.nds` whose SHA-1 equals the retail `AWRE` SHA-1, and that file boots in an emulator.

### 0.4 One-function workflow

- `dsd objdiff` generates `objdiff.json`. `objdiff-cli` diffs one compiled object against its original.
- A documented loop, adapted from khdays (`tools/getcand.py`, `tools/verify_idx.py`), all local:
  1. Pick a function. Print its disassembly and the file it belongs to.
  2. Write or generate C.
  3. Compile that one file and diff it against the original, relocations included.
  4. Iterate until the diff is empty.
  5. Run the review pass, then `make release`, then commit.
- Prove the loop on the three D6 functions: one SDK function, one ARM game function, one Thumb game function.

Done when: all three match through the real `make` pipeline. If the Thumb function only matches as hand-written assembly, record the evidence and continue (D6 "go with caveat").

Result (2026-10-06): done, with the caveat. `PackNibblePairs` (game, ARM, only matches mwccarm 2.0 builds 72–82), `FS_GetOverlayFileID` (SDK, with a relocation to the ROM archive) and `MTX_RotX43_` (SDK Thumb) match through `make release`. AWRE's game code contains no Thumb at all; its only Thumb functions are hand-written SDK assembly, so the Thumb proof is an `asm void` function in a `.c` file. Loop documented in `docs/workflow.md`.

## Milestone 1: Initial decomp

M0 gives a working repo. M1 starts real decompilation and tunes the automated loop on a small, chosen set of functions before scaling it.

- **Automation.** Build the loop khdays used, with our rules on top: candidate C is generated, compiled and diffed in a loop until it matches. The review pass runs on every match, and nothing is offered to sturmds unless `make release` passes. Keep each generated change small: one function, or one file once file boundaries are known.
- **Choosing functions.** sturmds's interests come first. Advance Wars candidates: damage calculation, unit and terrain tables, CO powers, AI turn logic, map loading. The automation fills in around them, favouring leaf functions (no calls) and the callers of functions already matched, so each match makes the next one easier.
- **File boundaries.** Start coarse. Move functions into recovered files as evidence appears: shared static data, functions that always sit next to each other, source file names left in assert strings. If the compiler flags include `-ipa file`, some functions will only match once they share a file with their real neighbours.
- **Progress report.** A local report that counts matched C, library assembly and data separately, plus an informational count of address-named symbols (D4).

Done when: the functions sturmds picks for M1 are matched and readable under the tenets, and the loop's speed and review quality are known well enough to plan M2.

Open question: how sturmds marks a function as "interesting". Options: a list in a file, a tag in `symbols.txt`, or naming it in a session.

## Milestone 2: Further decomp

Mostly sprinting on decomp and keeping the repo in working order along the way.

- Run the M1 loop at scale, module by module.
- Readability passes over automated code: shared structs in `include/`, names, constants, recovered file boundaries.
- Keep tools current: upgrades go through `tools.lock` and must keep `make release` passing.
- Revisit the deferred items when they start to matter:
  - ~~ARM7 built from objects instead of copied (D1)~~: done 2026-10-11 (issue 0013). dsd cannot delink ARM7 alone, so `tools/arm7.py` runs it on a stand-in ROM. Decompiling ARM7 to C follows the plan in issue 0042.
  - The DS Download Play child in the file system, a second program with its own ARM9 and ARM7 (issue 0049).
  - Asset extraction into editable forms (D5, issue 0050). Maps done: unpacked into `extracted/` by `make setup`, `docs/maps.md`.
  - Proper CI with a self-hosted runner that holds the ROM (D4).

## Milestone 3: Automated QA through emulation

Critical for the end goal: modding the game with the decomp as the base. The hash shows the matching build is correct. Once the code changes, the hash no longer applies, and testing has to happen in an emulator.

- **Emulator.** Pick a Linux-native emulator that supports Lua scripting and a GDB stub. Candidates to evaluate: melonDS, DeSmuME, BizHawk (which runs melonDS as its DS core). Which of them support Lua and GDB on Linux is not yet confirmed.
- **Scripted input.** Lua scripts that drive known inputs (menus, a battle, a full map) and record RAM values or screenshots at checkpoints. Run them against the retail ROM and our build, then compare.
- **Debugging.** GDB with overlay awareness. pokeplatinum already has `make debug` plus a GDB fork that understands DS overlays (`joshua-smith-12/binutils-gdb-nds`, linked from its `INSTALL.md`). Reuse that approach.
- **Shiftable build: the hard part for modding.** A matching build can pass its hash while still containing hard-coded addresses: a pointer written as the number `0x020a1234` instead of a symbol. Once code moves, every such number points at the wrong place. Modding needs every pointer expressed as a symbol so the linker can relocate it. dsd's `relocs.txt` already covers code. Pointers inside data tables are the long tail. A useful test: build with padding inserted in one module, then check that the game still runs through the M3 scripts. This test can start early in M2 and does not need to wait for M3.

Done when: one Lua-driven scenario runs the same on the retail ROM and on our build, and one shifted build passes the same scenario.

## Risks

| Risk | Effect | What we do |
|---|---|---|
| The compiler build is not available as a native Linux binary | Blocks M0 | Decided at 0.2 before other work. Stop and reassess if it can't be found. |
| Some Thumb code matches from no compiler build | Part of the game remains assembly | Accept it with evidence (D6 caveat). The decomp.dev report, and PROGRESS.md with it, counts it only when `config/AWRE/asm_functions.txt` lists it with its evidence and portable C sits beside it. |
| ARM7 is harder than ARM9 because dsd focuses on ARM9 | Delays ARM7-from-objects | Resolved 2026-10-11: `tools/arm7.py` works around dsd (issue 0013) |
| Automated C is unreadable (khdays' `int self` style) | Mod base is hard to use | Tenets plus review pass from M0. Readability passes in M2. |
| Hard-coded addresses hide in data tables | Modded builds crash | Shift test starting in M2 |
| One person, learning as we go | Slow early progress | M0 and M1 are sized for learning. Timeboxes are sturmds's call. |

## Open questions

- ~~Hook placement~~: decided, `pre-commit`.
- Draft tenets were committed as-is (2026-10-06); sturmds edits them over time.
- ~~ROM SHA-1 source~~: matches TASVideos' "Good" `AWRE` entry (SHA-1 and MD5), checked 2026-10-06.
- How "interesting" functions are marked (M1).
- Emulator choice (M3).
- Timeboxes for each milestone: sturmds's call.
