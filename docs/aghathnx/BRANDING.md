# AghathNX branding

AghathNX is a fork of Fritchii v5.8.4 (tag `fritchii-v5.8.4`). The rebrand is kept apart from
emulator work so fixes can still be exchanged with Fritchii and Lichii. Branding commits carry
the trailer `AghathNX-Branding: yes`.

## What changed

- **Name**: the build name is `AghathNX` (CMakeModules/GenerateSCMRev.cmake) when
  `dist/frizti_version.txt` exists. That file holds AghathNX's own version (starting at 1.0).
  Every `g_build_name == "Fritchii"` check now tests `"AghathNX"`, so the Fritchii UI and its
  Ocarina of Time tweaks stay on.
- **User-visible text**: "Fritchii" in string literals and the app's own "Lichii" in dialogs and
  `.ui` files now say "AghathNX". Code identifiers (`FritchiiUi`, `Frizti::` settings,
  `frizti.ini`, Qt object names used by the stylesheet) keep their names.
- **Icon**: `dist/aghathnx.svg` (source), `dist/aghathnx.png` and `dist/aghathnx.ico`, used by the
  `.rc` and `.qrc` files.
- **Executable**: `aghathnx.exe` (src/yuzu/CMakeLists.txt). `eden-cli.exe` is unchanged.
- **Data folder**: `%APPDATA%\aghathnx` (src/common/fs/fs_paths.h). Fritchii's folder was added
  as an import source (`EmuPath::FritchiiDir`) next to Eden, Lichii and the rest.
- **Updater**: off. `RunningVersion()` returns nothing for AghathNX, because Fritchii's updater
  would install Fritchii's package over this build.
- **Discord and Donate** buttons are hidden while `DiscordInvite` / `DonateLink` in
  src/yuzu/fritchii/fritchii_ui.h are empty.
- **CI**: `.github/workflows/aghathnx-windows.yml` builds the Windows zip. Lichii's
  multi-platform `release-binaries.yml` was removed (it would publish under the Lichii name).

Not changed yet: Linux/macOS desktop files (`dist/io.github.zortos293.lichii.*`), translations
(`.ts` files still hold the old source strings, so those lines show in English), and the
`dist/branding/lichii_*` artwork used by Lichii-mode builds.
