#pragma once
#include "Monster.hpp"

// ============================================================================
// MONSTER TYPES
// Enemy stats scale based on stage number (1–10).
// Scaling formula adapted from nguyenminh1113's Enemy class:
//   HP  = baseHp  + stage * hpScale
//   ATK = baseAtk + stage * atkScale
//   DEF = baseDef + stage * defScale
// Each monster type has a unique executeSkillSet() flavour.
// Add new monsters here — no need to create a new file per monster.
// ============================================================================

// ----------------------------------------------------------------------------
// GOBLIN — Low HP, fast. Tries to attack twice occasionally.
// ----------------------------------------------------------------------------
class Goblin : public Monster {
public:
    Goblin(int stage)
        : Monster("Goblin", 25 + stage * 8, 7 + stage * 2, 1 + stage, stage) {}

    string executeSkillSet(Entity& target) override {
        return "[Goblin] Scurries forward and stabs! " + target.takeDamage(getAttackPower());
    }
};

// ----------------------------------------------------------------------------
// SKELETON — Balanced. Standard attacker.
// ----------------------------------------------------------------------------
class Skeleton : public Monster {
public:
    Skeleton(int stage)
        : Monster("Skeleton", 35 + stage * 10, 9 + stage * 3, 3 + stage, stage) {}

    string executeSkillSet(Entity& target) override {
        return "[Skeleton] Swings its rusty sword! " + target.takeDamage(getAttackPower());
    }
};

// ----------------------------------------------------------------------------
// VENOMOUS SNAKE — Low DEF, high ATK. Glass cannon.
// ----------------------------------------------------------------------------
class VenomousSnake : public Monster {
public:
    VenomousSnake(int stage)
        : Monster("Venomous Snake", 28 + stage * 8, 10 + stage * 3, 1, stage) {}

    string executeSkillSet(Entity& target) override {
        return "[Venomous Snake] Lunges with venomous fangs! " + target.takeDamage(getAttackPower());
    }
};

// ----------------------------------------------------------------------------
// STONE GOLEM — High HP and DEF. Hits hard but slow (lower ATK scaling).
// ----------------------------------------------------------------------------
class StoneGolem : public Monster {
public:
    StoneGolem(int stage)
        : Monster("Stone Golem", 60 + stage * 15, 10 + stage * 2, 5 + stage * 2, stage) {}

    string executeSkillSet(Entity& target) override {
        return "[Stone Golem] Slams its rocky fist! " + target.takeDamage(getAttackPower());
    }
};

// ----------------------------------------------------------------------------
// DARK MAGE — Highest ATK scaling, lowest DEF. Maximum threat, minimum armor.
// ----------------------------------------------------------------------------
class DarkMage : public Monster {
public:
    DarkMage(int stage)
        : Monster("Dark Mage", 30 + stage * 10, 13 + stage * 4, 1 + stage, stage) {}

    string executeSkillSet(Entity& target) override {
        return "[Dark Mage] Fires a dark bolt! " + target.takeDamage(getAttackPower());
    }
};
