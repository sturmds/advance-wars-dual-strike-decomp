#!/bin/sh
# Prepare a git worktree of this repo for building: link the ROM and the
# tool cache from the main checkout and copy the unpacked maps and text, so
# nothing is downloaded.
# Run from the worktree root. Safe to run twice.
set -eu

main=$(dirname "$(git rev-parse --path-format=absolute --git-common-dir)")
here=$(git rev-parse --show-toplevel)
if [ "$main" = "$here" ]; then
    echo "wt_setup: this is the main checkout, nothing to do"
    exit 0
fi
[ -e "$main/orig/AWRE.nds" ] || { echo "error: $main/orig/AWRE.nds is missing" >&2; exit 1; }
[ -d "$main/.cache/tools" ] || { echo "error: run make setup in $main first" >&2; exit 1; }
for d in maps text; do
    [ -d "$main/extracted/AWRE/assets/$d" ] || { echo "error: run make setup in $main first (extracted/AWRE/assets/$d is missing)" >&2; exit 1; }
done

mkdir -p "$here/orig"
ln -sfn "$(readlink -f "$main/orig/AWRE.nds")" "$here/orig/AWRE.nds"
[ -e "$here/.cache" ] || ln -s "$main/.cache" "$here/.cache"
# The maps and text are copied, not linked: a worktree that edits or
# re-unpacks them must not change the main checkout's copy (about 1 MB and
# 0.6 MB). The build needs both. A link left by an
# older wt_setup.sh is replaced. Other assets are unpacked in the worktree
# when it needs them. extracted/AWRE/data stays per worktree too:
# the build rewrites it from config/, which can differ between branches.
mkdir -p "$here/extracted/AWRE"
[ -L "$here/extracted/AWRE/assets" ] && rm "$here/extracted/AWRE/assets"
mkdir -p "$here/extracted/AWRE/assets"
for d in maps text; do
    [ -e "$here/extracted/AWRE/assets/$d" ] || cp -R "$main/extracted/AWRE/assets/$d" "$here/extracted/AWRE/assets/$d"
done
echo "wt_setup: linked orig/AWRE.nds and .cache, copied extracted/AWRE/assets/maps and text from $main"
