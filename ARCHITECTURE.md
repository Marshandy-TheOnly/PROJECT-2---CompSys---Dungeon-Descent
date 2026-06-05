# Dungeon Descent — Architecture (minimal slice)

## File Structure

```
PROJECT-2---CompSys---Dungeon-Descent/
├── GameCore.hpp                    Umbrella include (one header pulls in everything)
├── main.cpp                        Entry point + minimal Hero class
├── README.md
├── ARCHITECTURE.md                 This file
├── CMakeLists.txt
└── src/
    ├── entities/
    │   ├── Entity.hpp              Abstract base for all characters
    │   ├── Player.hpp              Player base with skillLevels[4]
    │   ├── Monster.hpp             Stage-scaled enemy base
    │   └── Boss.hpp                Amplified monster with turn-counter mechanic
    ├── stages/
    │   ├── Stage.hpp               Abstract stage base + StageType enum
    │   └── OpportunityStage.hpp    3 random safe-zone events
    └── combat/
        ├── CombatSystem.hpp        Combat interface
        └── CombatSystem.cpp        Turn resolution logic
```

## Class Hierarchy

```
Entity  (abstract)
├── Player  (abstract)
│   └── Hero          (defined inline in main.cpp)
└── Monster
    └── Boss

Stage  (abstract)
└── OpportunityStage
```

## Key Systems

### Skill Level System (`Player.hpp`)
Every Player has 4 skills indexed 0–3, each leveling 1 → 5.

| Index | Skill | Effect at Lv1 | Effect at Lv5 |
|-------|-------|--------------|--------------|
| 0 | ATK   | +3 bonus damage | +15 bonus damage |
| 1 | DEF   | +4 bonus block  | +20 bonus block  |
| 2 | DODGE | 20% success chance | 40% success chance |
| 3 | ULT   | 30% miss chance | 10% miss chance |

- `levelUpSkill(int index)` — call after a monster/boss victory
- `getScaledAttack()` — `attackPower + skillLevels[0] * 3`
- `rollUltimate()` — RNG check; true = ultimate hits

### 4 Moves
- **Attack** — `getScaledAttack()` damage
- **Defend** — sets `blockThisTurn = classBase + skillLevels[1] * 4`
- **Dodge** — probabilistic (DODGE skill%); on success completely negates the next hit; 1-turn cooldown
- **Ultimate** — class-unique; must pass `rollUltimate()`

### Dodge + Block Resolution (`Player::takeDamage`)
```
incoming damage
    → if isDodging: negate completely
    → subtract blockThisTurn
    → subtract defense (Entity base)
    → apply remainder as HP loss
```
`resetTurnFlags()` is called by CombatSystem after each enemy turn to tick the dodge cooldown.

### Boss Mechanic (`Boss.hpp`)
Boss extends Monster with amplified stats (HP ×2.5, ATK ×1.5, DEF +4) and a turn counter: every 3rd turn fires a devastating special skill at double ATK damage.

### Opportunity Events (`OpportunityStage.hpp`)
One of 3 events selected randomly per stage:

| Event | Effect |
|-------|--------|
| Healing Pool | Restore HP to full |
| Forbidden Altar | 65%: +15HP/+4ATK/+2DEF; 35%: -30HP |
| Ancient Cache | Permanently +10 max HP |

## Differences from the Full Game

This slice omits — by design — the full project's UI layer (FTXUI), binary save/load, the 4 distinct player classes (Prince/Priest/Berserker/Mage), MonsterTypes, MonsterStage, and the 10-stage StageManager loop. `main.cpp` orchestrates a 3-stage demo directly instead.
