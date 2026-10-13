# 0081: Migrate to GitHub issues

- **Status:** open
- **Area:** docs
- **Opened:** 2026-10-13
- **Milestone:** later

## Problem

The repository is public on GitHub now (issue 0071), but issues are still plain files in `docs/issues/`, with the index in `README.md`. The README says this tracker is kept "until the project moves to a real one".

## Notes

To decide before moving:

- **Which issues.** 52 of the 81 are not closed (open, in progress or worked around). Move only those, or all of them so the history is in one place.
- **Numbers.** GitHub numbers its issues itself, and they will not match ours. Code, docs, skills and about 25 tools say "issue 0071" and the like, and so do commit messages, which cannot change. Keep a table from our number to the GitHub one, or leave a stub file per issue that points to it.
- **Who files issues.** The loop's wrap-up and triage jobs (`.claude/skills/decomp-orchestrate/jobs/`) write issue files today. On GitHub they would need `gh` and network access, which `CLAUDE.md` allows only for the tools in `tools.lock`, and the posts would be outward-facing. Either agents keep writing files and sturmds moves them, or the rule changes.
- **Privacy.** Issue text on GitHub does not go through `make pnc-check` or the `commit-msg` style check. Run both over each body before posting it.
- **Contributions.** The README says we take no contributions. Decide whether outside users can open issues, or turn issues off for them.

## Done when

Open issues live on GitHub, `docs/issues/` either points to them or is gone, `CLAUDE.md` and the loop's skills say where to file new issues, and references to old numbers still lead somewhere.
