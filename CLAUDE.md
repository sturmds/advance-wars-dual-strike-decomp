# advance-wars-dual-strike-decomp

Private, local, matching decompilation of *Advance Wars: Dual Strike*, US/AUS cartridge (game code `AWRE`). Matching means the C in this repo, compiled with the original Metrowerks compiler and flags, rebuilds the retail ROM byte for byte. `advance-wars-dual-strike-decomp` is a working code name.

The plan is in `docs/early-phase-project-plan.md` (edit that copy). The rules it follows (D1–D7) come from the NDS decomp survey linked at the top of the plan. The owner, sturmds, is new to game decompilation: explain DS and decomp terms briefly when they come up.

## Build and test

```sh
mkdir -p orig && ln -s /path/to/awds.nds orig/AWRE.nds   # once; never copy the ROM into git
make release    # setup if needed, build, then every check
make            # build build/AWRE.nds (ninja, incremental)
make check      # per-module hashes, then the whole-ROM SHA-1
make extract    # unpack orig/AWRE.nds into extract/ (make does this when needed)
make setup      # install git hooks, fetch tools.lock entries into .cache/tools/
```

- `make release` must work as the first command after cloning, and must work offline once `make setup` has run.
- The ROM is checked against `config/AWRE/rom.sha1` before anything else. That SHA-1 and the MD5 match TASVideos' "Good" entry for "Advance Wars - Dual Strike (USA, Australia).nds" (https://tasvideos.org/Games/2894/Versions/View/3068), checked 2026-10-06.
- Tools: `tools.lock` pins dsd, objdiff-cli and metroskrew (Linux). `tools/setup.py` downloads each once into `.cache/` and checks SHA-256 on every run.
- System packages needed: `make`, `ninja-build`, `python3` (3.11+), `libc6:i386` (the metroskrew binaries are 32-bit).
- Pipeline (`tools/configure.py` writes `build.ninja`): `dsd rom extract` → `dsd delink` (config in `config/AWRE/arm9/`) → `dsd lcf` → `mwldarm` → `dsd rom config` → `dsd rom build` → `tools/fix_header.py`. ARM7 and the file system come from `extract/` unchanged.
- `make check` runs `dsd check modules`, then `config/AWRE/modules.sha1`, then compares `build/AWRE.nds` with `config/AWRE/rom.sha1`.
- Compiler: mwccarm 2.0-72, `-O4,p -proc arm946e -interworking`; linker mwldarm 2.0-72. Evidence in `docs/awre-facts.md`.
- `config/AWRE/arm9/` was generated once by `tools/dsd_init.py` (it works around a `dsd init` abort, see `docs/awre-facts.md`). Edit those files by hand or with dsd; rerun `dsd_init.py --force` only on purpose, and check `git diff` afterwards.
- Status: Milestone 0 done (2026-10-06). The build matches retail with three functions in C: `PackNibblePairs` (game, ARM), `FS_GetOverlayFileID` (SDK, with a relocation), `MTX_RotX43_` (SDK Thumb assembly).

## Hard rules

- Matching build only. `AWRE` only. ARM7 and the file system are copied from the ROM until a later milestone.
- Linux only. No Windows executables, no wine, no wibo. The metroskrew compilers are native Linux ELF binaries that keep `.exe` names, so they are allowed.
- Every tool is pinned to an exact version and SHA-256 in `tools.lock`. Upgrades go through that file and must keep `make release` passing.
- Everything goes through `make`.
- Every commit passes `make release`. `hooks/pre-commit` runs it, and `make setup` installs the hooks through `core.hooksPath`. Do not bypass hooks with `--no-verify`.
- Git holds only what we write. The ROM, extracted files, objects, built ROMs, compilers and SDK files never enter git (`orig/`, `extract/`, `build/`, `.cache/` are gitignored, and `pre-commit` refuses `.nds`, `.bin`, `.o`, `.exe`). No cracked or patched license files.
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
| `tools/configure.py`, `tools/dsd_init.py`, `tools/fix_header.py` | Build generator, one-time dsd config generation, header CRC fix. |
| `tools/fn.py`, `docs/workflow.md` | One-function loop: show, diff. |
| `src/`, `include/` | Our C and headers. `src/sdk/` for library code, `src/game/` for game code. |
| `tools/style_check.py` | Checks prose against `docs/style/`. Used by `hooks/commit-msg`. |
| `hooks/` | `pre-commit` (forbidden files, then `make release`), `commit-msg` (banned words). |
| `config/AWRE/` | `rom.sha1`, `modules.sha1`, and dsd's `arm9/**` config (delinks, symbols, relocations). |
| `docs/awre-facts.md` | ROM layout, function counts, compiler fingerprint, dsd problems. |
| `orig/AWRE.nds` | Symlink to your ROM dump. Gitignored. |
| `extract/`, `build/`, `.cache/` | Generated output and downloaded tools. Gitignored. |

Reference repos in `~/repos/` are read-only: `pokeplatinum` (make wrapper, tool fetching, layered hash checks), `khdays-decomp` (dsd pipeline, one-function loop), `ds-decomp` (dsd source), `metroskrew` (compilers). `awds-decomp` is someone else's EU repo: never a base and never copied from; its notes are unverified outside data.

## Decompiling one function

See `docs/workflow.md`. In short: `tools/fn.py show` → add a `delinks.txt` entry and rename symbols → write C in `src/` → `tools/fn.py diff` until 100% → mark the file `complete` → tenets review → `make release` → commit.

## Writing

Applies to code comments, docs, commit messages and chat.

- Plain and short. No hype, no filler.
- Never use the "X is the Y" aphorism ("the hash is the test"). Say what the thing does.
- No words from `docs/style/banned.txt`. Treat `docs/style/claudeisms.csv` words with suspicion.
- Commit messages end with the `Co-Authored-By` trailer when Claude wrote the change.
