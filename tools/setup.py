#!/usr/bin/env python3
"""Fetch the tools in tools.lock into .cache/ and check their hashes.

Downloads only what is missing. With every tool present and matching, it
makes no network calls. Layout:

    .cache/downloads/<name>-<version>-<file>   the file as downloaded
    .cache/tools/<name>/                       binary or unpacked archive
    .cache/tools/<name>/.sha256                hash of the download it came from
"""

import hashlib
import shutil
import sys
import tarfile
import tomllib
import urllib.request
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CACHE = ROOT / ".cache"
DOWNLOADS = CACHE / "downloads"
TOOLS = CACHE / "tools"


def sha256(path: Path) -> str:
    h = hashlib.sha256()
    with path.open("rb") as f:
        for chunk in iter(lambda: f.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def download(name: str, url: str, dest: Path) -> None:
    print(f"setup: downloading {name} from {url}")
    tmp = dest.with_suffix(dest.suffix + ".part")
    try:
        with urllib.request.urlopen(url, timeout=60) as r, tmp.open("wb") as f:
            shutil.copyfileobj(r, f)
    except OSError as e:
        tmp.unlink(missing_ok=True)
        sys.exit(f"error: could not download {name} ({e}).\n"
                 f"  The first `make setup` needs internet; later builds do not.")
    tmp.rename(dest)


def install(name: str, spec: dict) -> None:
    want = spec["sha256"]
    archive = DOWNLOADS / f"{name}-{spec['version']}-{spec['url'].rsplit('/', 1)[1]}"
    target = TOOLS / name
    stamp = target / ".sha256"

    if not archive.exists():
        DOWNLOADS.mkdir(parents=True, exist_ok=True)
        download(name, spec["url"], archive)
    got = sha256(archive)
    if got != want:
        sys.exit(f"error: {archive} has SHA-256 {got}, tools.lock expects {want}.\n"
                 f"  Delete it and rerun `make setup`.")

    if stamp.exists() and stamp.read_text().strip() == want:
        return

    shutil.rmtree(target, ignore_errors=True)
    target.mkdir(parents=True)
    if spec["type"] == "binary":
        exe = target / name
        shutil.copyfile(archive, exe)
        exe.chmod(0o755)
    elif spec["type"] in ("tar.xz", "tar.gz"):
        with tarfile.open(archive) as t:
            t.extractall(target, filter="tar")
        # Archives with a single top-level directory are flattened.
        entries = [p for p in target.iterdir()]
        if len(entries) == 1 and entries[0].is_dir():
            inner = entries[0]
            for p in inner.iterdir():
                p.rename(target / p.name)
            inner.rmdir()
    else:
        sys.exit(f"error: unknown type {spec['type']!r} for {name} in tools.lock")
    stamp.write_text(want + "\n")
    print(f"setup: installed {name} {spec['version']}")


def main() -> int:
    lock = tomllib.loads((ROOT / "tools.lock").read_text())
    for name, spec in lock.items():
        install(name, spec)
    return 0


if __name__ == "__main__":
    sys.exit(main())
