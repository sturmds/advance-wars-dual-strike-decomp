# advance-wars-dual-strike-decomp

Private, local, matching decompilation of *Advance Wars: Dual Strike*, US/AUS cartridge (game code `AWRE`). Matching means the C in this repo, compiled with the original Metrowerks compiler and flags, rebuilds the retail ROM byte for byte. `advance-wars-dual-strike-decomp` is a working code name.

The plan is in `docs/early-phase-project-plan.md` (edit that copy). The rules it follows (D1–D7) come from the NDS decomp survey linked at the top of the plan. The owner, sturmds, is new to game decompilation: explain DS and decomp terms briefly when they come up.

## Build and test

```sh
mkdir -p orig && ln -s /path/to/awds.nds orig/AWRE.nds   # once; never copy the ROM into git
make release    # setup if needed, build, then every check
make check      # hash checks only
make setup      # install git hooks, fetch tools.lock entries into .cache/tools/
```

- `make release` must work as the first command after cloning, and must work offline once `make setup` has run.
- The ROM is checked against `config/AWRE/rom.sha1` before anything else. That SHA-1 has not yet been compared with the No-Intro entry, so treat it as provisional until sturmds confirms.
- Tools: `tools.lock` pins dsd, objdiff-cli and metroskrew (Linux). `tools/setup.py` downloads each once into `.cache/` and checks SHA-256 on every run.
- Status: Milestone 0. The build currently only checks the ROM. Later steps add `configure.py` → `build.ninja`, delink and relink, and per-module plus whole-ROM hash checks.

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
| `tools/style_check.py` | Checks prose against `docs/style/`. Used by `hooks/commit-msg`. |
| `hooks/` | `pre-commit` (forbidden files, then `make release`), `commit-msg` (banned words). |
| `config/AWRE/` | Our config: ROM hash now; dsd's `arm9/**` analysis files later. |
| `orig/AWRE.nds` | Symlink to your ROM dump. Gitignored. |
| `extract/`, `build/`, `.cache/` | Generated output and downloaded tools. Gitignored. |

Reference repos in `~/repos/` are read-only: `pokeplatinum` (make wrapper, tool fetching, layered hash checks), `khdays-decomp` (dsd pipeline, one-function loop), `ds-decomp` (dsd source), `metroskrew` (compilers). `awds-decomp` is someone else's EU repo: never a base and never copied from; its notes are unverified outside data.

## Decompiling one function

Written in plan step 0.4. Outline: pick a function, write C, compile that one file, diff against the original with `objdiff-cli` (relocations included), iterate until the diff is empty, run the tenets review, `make release`, commit.

## Writing

Applies to code comments, docs, commit messages and chat.

- Plain and short. No hype, no filler.
- Never use the "X is the Y" aphorism ("the hash is the test"). Say what the thing does.
- No words from `docs/style/banned.txt`. Treat `docs/style/claudeisms.csv` words with suspicion.
- Commit messages end with the `Co-Authored-By` trailer when Claude wrote the change.
