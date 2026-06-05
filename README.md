# Dungeon Descent

A small console-based dungeon crawler written in C++17. A single `Hero` descends through a sequence of stages — opportunity events, monster fights, and a boss — using a 4-move combat system (Attack / Defend / Dodge / Ultimate).

This repository is the **minimal slice** of the full game: combat, two stage types, one playable class, and one boss. The full version (4 player classes, 10-stage manager, FTXUI menus, binary save/load) lives separately.

## Build

```sh
cmake -S . -B build
cmake --build build
```

The resulting executable is at `build/DungeonDescent` (or `build/Debug/DungeonDescent.exe` on Windows multi-config generators).

## Layout

```
GameCore.hpp           umbrella include
main.cpp               demo driver
src/
├── entities/          Entity, Player, Monster, Boss
├── stages/            Stage base + OpportunityStage
└── combat/            CombatSystem (turn loop)
```

See `ARCHITECTURE.md` for system design.
