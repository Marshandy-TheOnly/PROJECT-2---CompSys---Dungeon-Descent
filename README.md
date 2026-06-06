# Dungeon Descent
A turn-based dungeon crawler in C++17. Choose from 4 classes, fight through 10 randomized stages, and defeat two bosses. Features an FTXUI terminal UI, per-class save files, and a 4-move combat system (Attack / Defend / Dodge / Ultimate).

## Requirements
- [MSYS2](https://www.msys2.org/) with the UCRT64 toolchain
- Git (for CMake to fetch FTXUI automatically)
- Internet connection on first build

## Build (PowerShell — run once per session)
**Step 1 — Install cmake (first time only)**
```powershell
& "C:\msys64\usr\bin\pacman.exe" -S mingw-w64-ucrt-x86_64-cmake --noconfirm
```
**Step 2 — Go to the project folder**
```powershell
cd "[file address]"
```
**Step 3 — Add MSYS2 tools to PATH (once per session)**
```powershell
$env:PATH = "C:\msys64\ucrt64\bin;C:\msys64\usr\bin;" + $env:PATH
```
**Step 4 — Configure (downloads FTXUI on first run)**
```powershell
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++
```
**Step 5 — Build**
```powershell
cmake --build build
```
**Step 6 — Run**
```powershell
.\build\DungeonDescent.exe
```
> After the first build, only Steps 2, 5, and 6 are needed when you make changes.

## Layout
```
GameCore.hpp                umbrella include
main.cpp                    entry point
src/
├── entities/               Entity, Player, Prince, Priest, Berserker, Mage
│                           Monster, Boss, MonsterTypes
├── stages/                 Stage, OpportunityStage, MonsterStage, StageManager
├── combat/                 CombatSystem
├── ui/                     UIManager, Sprites
└── save/                   SaveManager (CSV format, one file per class)
```
See `ARCHITECTURE.md` for system design details.
