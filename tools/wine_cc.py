#!/usr/bin/env python3
"""Run mwccarm 2.0 build 56 under wine, with the arguments of the metroskrew
compilers (issue 0061).

    tools/wine_cc.py -c FLAGS... [-gccdep] [-MD] -o OUT.o SRC.c

configure.compiler_for() gives this script as the compiler for the files in
config/AWRE/compilers.txt, so ninja and the fn.py tools call it like any
other mwccarm. Build 56 differs from the metroskrew builds in three ways
this script hides:

- It is a Windows program. wine runs it, with its own prefix in .cache/wine
  (made once, under a lock, without network).
- It has no -gccdep. With -MD it writes SRC.d into the current directory,
  with Windows paths (Z:\\home\\...). The script compiles in a temporary
  directory and writes OUT.d next to the object, with Linux paths relative to
  the repo, as -gccdep -MD does.
- Path arguments are made absolute, since the compiler runs in that
  temporary directory.

wine, not wibo: wibo 1.2.0 leaves the x87 FPU at Linux's 64-bit precision,
and mwccarm then lowers some switches differently (__pformatter).
"""
import fcntl
import os
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
EXE = ROOT / ".cache/tools/mwccarm-b56/mwccarm.exe"
PREFIX = ROOT / ".cache/wine"
PATH_OPTIONS = {"-o", "-i", "-I", "-ir", "-include", "-prefix"}


def wine_env() -> dict:
    env = dict(os.environ)
    env["WINEPREFIX"] = str(PREFIX)
    env["WINEDEBUG"] = "-all"
    # No Mono or Gecko install prompt (they would need the network).
    env["WINEDLLOVERRIDES"] = "mscoree,mshtml="
    return env


def ensure_prefix(env: dict) -> None:
    """Make the wine prefix once. Parallel ninja jobs wait on the lock."""
    if (PREFIX / "system.reg").exists():
        return
    PREFIX.parent.mkdir(parents=True, exist_ok=True)
    with open(PREFIX.parent / "wine.lock", "w") as lock:
        fcntl.flock(lock, fcntl.LOCK_EX)
        if not (PREFIX / "system.reg").exists():
            subprocess.run(["wineboot", "-i"], env=env, check=True,
                           stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)


def linux_path(p: str, cwd: Path) -> str:
    """A path from the compiler's depfile, relative to the repo when inside it."""
    if p[:2].upper() == "Z:":
        path = Path(p[2:].replace("\\", "/"))
    else:
        path = (cwd / p.replace("\\", "/")).resolve()
    return path.relative_to(ROOT).as_posix() if path.is_relative_to(ROOT) else path.as_posix()


def write_depfile(raw: str, out: Path, cwd: Path) -> None:
    """Rewrite build 56's depfile for `out`: target, then one dependency per line."""
    text = raw.replace("\\\n", " ").replace("\r", "")
    _, _, deps = text.partition(": ")
    # Windows paths in this file hold no spaces (the repo path has none).
    names = [linux_path(d, cwd) for d in deps.split()]
    lines = [f"{out.relative_to(ROOT).as_posix() if out.is_relative_to(ROOT) else out}: \\"]
    lines += [f"\t{n} \\" for n in names[:-1]] + [f"\t{names[-1]}"] if names else []
    out.with_suffix(".d").write_text("\n".join(lines) + "\n")


def main(argv: list[str]) -> int:
    if shutil.which("wine") is None:
        sys.exit("error: wine is not installed. It runs mwccarm 2.0 build 56 for the files in\n"
                 "  config/AWRE/compilers.txt (issue 0061). Install it with:\n"
                 "  sudo apt install wine wine32:i386")
    if not EXE.exists():
        sys.exit(f"error: {EXE.relative_to(ROOT)} is missing; run `make setup`.")
    args, out, depfile = [], None, False
    it = iter(argv)
    for a in it:
        if a == "-gccdep":
            continue
        if a == "-MD":
            depfile = True
            continue
        if a in PATH_OPTIONS:
            p = str(Path(next(it)).resolve())
            if a == "-o":
                out = Path(p)
            args += [a, p]
        elif not a.startswith("-") and Path(a).suffix in (".c", ".cpp", ".s"):
            args.append(str(Path(a).resolve()))
        else:
            args.append(a)
    env = wine_env()
    ensure_prefix(env)
    with tempfile.TemporaryDirectory() as d:
        cwd = Path(d)
        r = subprocess.run(["wine", str(EXE), *args, *(["-MD"] if depfile else [])],
                           cwd=cwd, env=env, capture_output=True, text=True)
        sys.stdout.write(r.stdout.replace("\r", ""))
        sys.stderr.write(r.stderr.replace("\r", ""))
        if r.returncode == 0 and depfile and out is not None:
            made = list(cwd.glob("*.d"))
            if len(made) != 1:
                sys.exit(f"error: mwccarm 2.0-56 wrote {len(made)} depfiles, expected one")
            write_depfile(made[0].read_text(errors="replace"), out, cwd)
        return r.returncode


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
