# WT De-Rename

Bring back original names to the localized weapon copies (like: `PL-2` -> `R-3S`, `RB24` -> `AIM-9B`, `R-60MK` -> `R-60M`, ...). Full list can be seen in [units_weaponry-rename.csv](units_weaponry-rename.csv).

### How to apply localization mod:
#### Option 1 (replace)
0. generate `lang/` folder (Steps: 0-2 from _manual_ manual below)
0. Dawnload `units_weaponry.csv` (like: from [Release WT_DeRename v1.1 · Challanger524/WT_DeRename](https://github.com/Challanger524/WT_DeRename/releases#:~:text=4-,units_weaponry.csv))
0. Copy `units_weaponry.csv` to `War Thunder/lang/` folder

#### Option 2 (append)
0. generate `lang` folder (Steps: 0-2 from _manual_ manual below)
0. create folder `lang-mod`
0. place there `units_weaponry-de_rename.csv` (from https://github.com/Challanger524/WT_DeRename/releases)
0. open with notepad (text editor) `lang/localization.blk`
	- to the `locTable{` list - append (**to the bottom**):
	- `file:t="%lang/../lang-mod/units_weaponry-de_rename.csv"`
	- result should look like:
		```
		locTable{
		  ...
		  file:t="%lang/_missing.csv"
		  file:t="%lang/_online.csv"
		  file:t="%lang/pc_UiMessages.csv"
		  file:t="%lang/../lang-mod/units_weaponry-de_renamed.csv"
		}
		```


### How to DeRename (manually):

0. you must have enabled **custom localization** in `War Thunder/config.blk`
	- https://wiki.warthunder.com/Instructions_for_the_user_localizations
	- then start the game to generate `War Thunder/lang/` folder
0. enter `War Thunder/lang/` folder
0. rename `units_weaponry.csv` -> `_units_weaponry.csv` (prepend frontal underscore) to preserve original file
0. copy the renamed `_units_weaponry.csv` and (re)place it near the `WT_DeRename.exe`
0. make sure `units_weaponry-rename.csv` is near `WT_DeRename.exe` too
	- `units_weaponry-rename.csv` table format is self-explainable
0. run the `WT_DeRename.exe` (will produce/rewrite `units_weaponry.csv`)
0. copy `units_weaponry.csv` to `War Thunder/lang/` folder

Hints:
- You can use text compare tool/editor (like: `VSCode`) to see the difference between `.csv` files

## Credits:
- thanks to the **gszabi99** for his [Guided weaponry data (in-game values)](https://docs.google.com/spreadsheets/d/1SsOpw9LAKOs0V5FBnv1VqAlu3OssmX7DJaaVAUREw78/edit) spreadsheet
- thanks to the **jaek898**  for his [wt-localization](https://github.com/jaek898/wt-localization) with better vehicle names and roundels
- thanks to the **war_Tinder** for his [A better guide to localization mods](https://wiki.warthunder.com/cdk/1819-a-better-guide-to-localization-mods) _longread_
- **WT De-Rename** designed and programmed by **Me**
