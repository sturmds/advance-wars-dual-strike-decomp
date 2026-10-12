# advance-wars-dual-strike-decomp

Private, local, matching decompilation of *Advance Wars: Dual Strike*, US/AUS cartridge (game code `AWRE`). Matching means the C in this repo, compiled with the original Metrowerks compiler and flags, rebuilds the retail ROM byte for byte. `advance-wars-dual-strike-decomp` is a working code name.

The plan is in `docs/early-phase-project-plan.md` (edit that copy). The rules it follows (D1–D7) come from the NDS decomp survey linked at the top of the plan. The owner, sturmds, is new to game decompilation: explain DS and decomp terms briefly when they come up.

## Build and test

```sh
mkdir -p orig && ln -s /path/to/awds.nds orig/AWRE.nds   # once; never copy the ROM into git
make release    # setup if needed, format check, build, then every check
make format     # clang-format src/ and include/ (style in .clang-format)
make            # build build/AWRE.nds (ninja, incremental)
make check      # per-module hashes, then the whole-ROM SHA-1
make extract    # unpack orig/AWRE.nds into extract/ (make does this when needed)
make setup      # install git hooks, fetch tools.lock entries into .cache/tools/
```

- `make release` must work as the first command after cloning, and must work offline once `make setup` has run.
- The ROM is checked against `config/AWRE/rom.sha1` before anything else. That SHA-1 and the MD5 match TASVideos' "Good" entry for "Advance Wars - Dual Strike (USA, Australia).nds" (https://tasvideos.org/Games/2894/Versions/View/3068), checked 2026-10-06.
- Tools: `tools.lock` pins dsd, objdiff-cli, metroskrew (Linux), clang-format (PyPI wheel) and decomp-permuter (Python source, run by `tools/permuter.py`). `tools/setup.py` downloads each once into `.cache/` and checks SHA-256 on every run.
- System packages needed: `make`, `ninja-build`, `python3` (3.11+), `python3-yaml` (PyYAML, for `tools/maps.py`), `libc6:i386` (the metroskrew binaries are 32-bit), `flock` from `util-linux` (part of every Ubuntu install; `make` and `fn.py` run ninja under `build/.ninja.lock`, one build per checkout at a time). `tools/permuter.py` also needs `binutils-arm-none-eabi`; the build does not.
- Pipeline (`tools/configure.py` writes `build.ninja`): `dsd rom extract` → `dsd delink` (config in `config/AWRE/arm9/`) → `dsd lcf` → `tools/lcf_absolute.py` → `tools/maps.py lcf` → `tools/lcf_align2.py` → `mwldarm` → `dsd rom config` → `dsd rom build` → `tools/fix_header.py`. ARM7 goes through the same steps from `config/AWRE/arm7/`, with `tools/arm7.py` around them, because dsd 0.12.1 cannot delink ARM7 on its own (`docs/tool-issues.md`). The file system comes from `extract/` unchanged. The map tables in ov000 come from `assets/maps/` (`tools/maps.py pack`, `docs/maps.md`).
- Format: Google style through clang-format (`.clang-format`, `tools/format.py`), with includes kept in order. `make release` fails on an unformatted file. Files that define an `asm` function are skipped, because clang-format breaks ARM assembly. `fn.py finish` and `apply_commit.py` format the files they touch.
- `make check` runs `dsd check modules` (ARM9), then `config/AWRE/modules.sha1` (ARM9 and `arm7.bin`), then compares `build/AWRE.nds` with `config/AWRE/rom.sha1`.
- Compiler: mwccarm 2.0-72, `-O4,p -proc arm946e -interworking` for ARM9 and `-O4,s -proc arm7tdmi -interworking` for ARM7 (optimised for size; the level is a guess); linker mwldarm 2.0-72. Evidence in `docs/awre-facts.md`.
- `config/AWRE/arm9/` was generated once by `tools/dsd_init.py` (it works around a `dsd init` abort, see `docs/awre-facts.md`). Edit those files by hand or with dsd; rerun `dsd_init.py --force` only on purpose, and check `git diff` afterwards. `config/AWRE/arm7/` was generated the same way by `tools/arm7_init.py`.
- Status: Milestone 0 done (2026-10-06). The build matches retail with three functions in C: `PackNibblePairs` (game, ARM), `FS_GetOverlayFileID` (SDK, with a relocation), `MTX_RotX43_` (SDK Thumb assembly).

## Hard rules

- Matching build only. `AWRE` only. ARM9 and ARM7 are linked from objects. The file system, including the Download Play child `files/boot/mb_combat_sgn.srl` (issue 0049), is copied from the ROM until a later milestone (issue 0050).
- Linux only. No Windows executables, no wine, no wibo. The metroskrew compilers are native Linux ELF binaries that keep `.exe` names, so they are allowed.
- Every tool is pinned to an exact version and SHA-256 in `tools.lock`. Upgrades go through that file and must keep `make release` passing.
- Everything goes through `make`.
- Every commit passes `make release`. `hooks/pre-commit` runs it, and `make setup` installs the hooks through `core.hooksPath`. Do not bypass hooks with `--no-verify`.
- Git holds only what we write. Game data we have reversed into a text form we edit (the maps in `assets/maps/`) counts as ours. The ROM, extracted files, objects, built ROMs, compilers and SDK files never enter git (`orig/`, `extract/`, `build/`, `.cache/` are gitignored, and `pre-commit` refuses `.nds`, `.bin`, `.o`, `.exe`). No cracked or patched license files.
- A build passes only if the whole ROM's SHA-1 equals the retail `AWRE` SHA-1 and every per-module hash matches. No exemption for header CRC bytes.
- 100% private and local. No decomp.me, no decomp.dev, no remote, no pushing. Network access is only for downloading tools listed in `tools.lock`.
- No sudo. If a system package is missing, give sturmds the exact `sudo apt install ...` line and wait. Never build toolchains or dependencies from source.
- Ask before installing anything or making outward-facing changes. Commit only when asked or as part of an agreed task.

## Where things live

| Path | What |
|---|---|
| `TENETS.md` | Readability and writing rules. Every change is reviewed against them. |
| `.claude/skills/tenets-review/` | The review pass. Run it before offering any generated change. |
| `docs/early-phase-project-plan.md` | Milestones and open questions. |
| `docs/style/` | `banned.txt` (blocks commits), `claudeisms.csv` (warnings), and their source. |
| `tools.lock`, `tools/setup.py` | Tool versions, URLs, hashes, and the fetcher. |
| `tools/configure.py`, `tools/lcf_absolute.py`, `tools/lcf_align2.py`, `tools/dsd_init.py`, `tools/fix_header.py` | Build generator, absolute symbols for the ARM9 linker script (`ARM9_ABSOLUTE_SYMBOLS`, see `docs/workflow.md`), 2-byte alignment for ARM9 main and ARM7 objects that start at 2 mod 4 (`docs/tool-issues.md`), one-time dsd config generation, header CRC fix. |
| `tools/arm7.py`, `tools/arm7_init.py` | ARM7 build steps around dsd (a stand-in ROM with the parts moved into dsd's range), and the one-time generation of `config/AWRE/arm7/`. |
| `tools/fn.py`, `docs/workflow.md` | One-function loop: pick, show, add, diff, finish, rename. The near-miss cookbook and the parking rule for partials are in `docs/workflow.md`. |
| `tools/fnindex.py`, `tools/tests/`, `fn.py check` | Function index (`build/fnindex.json`) and the fast `fn.py check`, with its test against `fn.py diff`. `fn.py pick --families` ranks same-shape families. |
| `tools/ds.py`, `tools/dsindex.py` | Data structures (issue 0041): `ds.py show STRUCT\|data_ADDR` for a struct's layout from the compiler or a data symbol's type, references and bytes; `ds.py field Struct.field` for a field's users and how the original loads it. Index in `build/dsindex.json`. |
| `tools/propagate.py`, `tools/trivial.py`, `tools/permute.py`, `tools/pragmas.py`, `tools/triage_partials.py`, `tools/protocheck.py`, `tools/permuter.py` | Match twins of matched functions, very short functions, and near-misses by variant search or by optimisation pragmas, with no model tokens; sort the partials by kind of difference; find declarations that disagree with the callee's definition; run decomp-permuter on a near miss. See `docs/workflow.md`. |
| `docs/loop.md`, `.claude/skills/decomp-function/`, `tools/apply_commit.py`, `tools/wt_setup.sh` | Automated loop: agents in worktrees, replaying their commits onto mainline. |
| `.claude/skills/decomp-orchestrate/`, `tools/loop_state.py` | The loop without a human: ticks from a session cron job, state outside git in `work/loop/`, job templates for planning, partial retries, renames, wrap-up and triage. |
| `assets/maps/`, `tools/maps.py`, `docs/maps.md`, `docs/map-terrain.md` | Campaign, Versus, War Room and Survival maps, one YAML file per map plus three glue files that set the order in ov000 and `legend.yaml`, the token for each tile code. `maps.py pack` turns them into the C that `src/game/ov000/map/` includes; `maps.py unpack` writes them from the ROM. `docs/map-terrain.md` lists the tile codes of large objects (cannons, volcanoes, the Grand Bolt). |
| `tools/header_merge.py`, `tools/rebase_onto_mainline.py`, `tools/land_self.py`, `tools/review_stamp.py`, `tools/tests/test_landing.py` | Header merge by struct offset (used by `apply_commit.py`), with sizes and offsets checked by compiling each version with mwccarm (a layout that differs from the compiler's stops the replay); an agent rebases its branch onto mainline and lands it with a fast-forward under a lock. `land_self.py` lands one branch at a time: it waits in a queue (`<git common dir>/land-queue.lock`) from before the replay to the fast-forward. A field rename from the merge is applied to the commit's C only when no other struct has a field of the old name; otherwise the replay stops and lists the uses. Enum constants and `#define`s merge as a union; a name with two values stops the replay. A field or struct size that one side removed or shrank from the commit's parent stays that way; if the other side changed that field, the replay stops. `land_self.py` exits 4 while a replay is open or another run holds the worktree's lock, and 5 when a commit that touches `src/` or `include/` has no tenets review stamp (`tools/review_stamp.py`, kept under the git directory by patch ID; the tenets-review skill writes it with a findings file, and a commit fixed by hand during a replay needs a new one). A replay that changes `land_self.py` or `review_stamp.py` restarts the run from the new copy. `make test` runs `test_landing.py` on throwaway repos and `test_propagate.py` and `test_loop_state.py` (no ROM needed). |
| `src/`, `include/` | Our C and headers. `src/sdk/` for library code, `src/game/` for game code, `src/arm7/` for ARM7 (built with the ARM7 flags). |
| `tools/shift_build.py`, `tools/emu_smoke.py`, `tools/scan_pointers.py`, `tools/resolve_overlay_relocs.py`, `tools/modmap.py` | Shiftable build checks (issues 0012, 0019, 0006): `make shift` builds `build/shift/AWRE.nds` with padding in main and checks that symbols moved; `make emu-smoke` starts the ROMs in DeSmuME without a window; pointer scanner; overlay relocation narrowing. See `docs/workflow.md`. |
| `.clang-format`, `tools/format.py` | C formatting: `make format` rewrites, `make format-check` (part of release) reports. `docs/third-party/clang-format.md`. |
| `tools/style_check.py` | Checks prose against `docs/style/`. Used by `hooks/commit-msg`. |
| `hooks/` | `pre-commit` (forbidden files, then `make release`, then `PROGRESS.md`), `commit-msg` (banned words). |
| `PROGRESS.md`, `tools/progress.py` | Functions and bytes linked from C, per module, and C files not linked yet. Written by `pre-commit`; `apply_commit.py` ignores it. |
| `config/AWRE/` | `rom.sha1`, `modules.sha1`, and dsd's `arm9/**` and `arm7/` config (delinks, symbols, relocations). |
| `config/AWRE/names-provenance.csv`, `tools/import_names.py` | Where each imported name came from, and the importer that fills it. `fn.py rename` keeps it current. |
| `docs/third-party/` | Credit pages for outside sources we take facts from. |
| `docs/awre-facts.md` | ROM layout, function counts, compiler fingerprint. |
| `docs/tool-issues.md` | Problems in dsd and other tools, and our workarounds. |
| `docs/issues/` | Issue tracker: one file per issue, index in `README.md`. Add an issue for any problem found and not fixed in the same change. |
| `orig/AWRE.nds` | Symlink to your ROM dump. Gitignored. |
| `extract/`, `build/`, `.cache/` | Generated output and downloaded tools. Gitignored. |

Reference repos in `~/repos/` are read-only: `pokeplatinum` (make wrapper, tool fetching, layered hash checks), `khdays-decomp` (dsd pipeline, one-function loop), `ds-decomp` (dsd source), `metroskrew` (compilers). `awds-decomp` is someone else's EU repo: never a base and never copied from; its notes are unverified outside data.

## Source layout

Group code by what it does. A reader should find the CO powers, the map loader or the unit code by directory, not by grepping a flat list.

- `src/game/<module>/` holds code from one module (`main`, `itcm`, `ovNNN`). The module decides where the code links, so it stays the top level.
- ARM7 code goes in `src/arm7/sdk/`, grouped by SDK library the same way. ARM7 has no game code so far; `src/arm7/game/` is for any that turns up. `configure.py` picks the ARM7 flags for every file under `src/arm7/`.
- Inside a module, a known topic gets a directory: `src/game/main/copowers/` (CO Power routines and the unit filters, unit bonuses and calculation bonuses that `CoPowerBlock` points to), `src/game/main/skills/` (day-to-day skill and Force Rank routines).
- When a decompiled function belongs to an existing topic, put it there. When several functions share a new topic that the code makes clear, create a directory for it and say so in the commit message.
- A function whose purpose is unknown stays at the module level (`src/game/main/func_XXXXXXXX.c`) until it is understood; move it then.
- Headers in `include/game/` follow the same topics when they grow (one header per struct or table for now).
- Moving a file means `git mv` plus its path in `delinks.txt`; `make release` must still pass.

## Decompiling one function

See `docs/workflow.md`. In short: `tools/fn.py show` → add a `delinks.txt` entry and rename symbols → write C in `src/` → `tools/fn.py diff` until 100% → mark the file `complete` → tenets review → `make release` → commit.

## Writing

Applies to code comments, docs, commit messages and chat.

- Plain and short. No hype, no filler.
- Never use the "X is the Y" aphorism ("the hash is the test"). Say what the thing does.
- No words from `docs/style/banned.txt`. Treat `docs/style/claudeisms.csv` words with suspicion.
- Commit messages end with the `Co-Authored-By` trailer when Claude wrote the change.
