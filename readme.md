# WT De-Rename

Bring back original names to the localized weapon copies (like: `PL-2` -> `R-3S`, `RB24` -> `AIM-9B`, `R-60MK` -> `R-60M`, ...)

### How to DeRename:

0. you must have enabled **custom localization** in `War Thunder/config.blk`: jaek898' [instruction](https://github.com/jaek898/wt-localization?tab=readme-ov-file#instructions)
0. enter `War Thunder/lang/` folder
0. rename `units_weaponry.csv` -> `_units_weaponry.csv` (prepend frontal underscore)
0. copy the renamed `_units_weaponry.csv` and (re)place it near the `WT_DeRename.exe`
0. make sure `units_weaponry-rename.csv` is near `WT_DeRename.exe` too
	- `units_weaponry-rename.csv` table format is self-explainable
	- if you receive error with _line number_ - add to this number count of empty and comment lines (above)
0. run the `WT_DeRename.exe` (will produce/rewrite `units_weaponry.csv`)
0. copy `units_weaponry.csv` to `War Thunder/lang/` folder

Hints:
- You can use file (text) compare tool (like: `VSCode`) to see the rename results

## Credits:
- programmed by Me
- thanks to the **jaek898**  for his [wt-localization](https://github.com/jaek898/wt-localization) of vehicle names
- thanks to the **gszabi99** for his [Guided weaponry data (in-game values)](https://docs.google.com/spreadsheets/d/1SsOpw9LAKOs0V5FBnv1VqAlu3OssmX7DJaaVAUREw78/edit) spreadsheet
