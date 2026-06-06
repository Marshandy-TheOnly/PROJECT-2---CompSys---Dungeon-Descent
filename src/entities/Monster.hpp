#pragma once
#include "Entity.hpp"

// ============================================================================
// MONSTER BASE — Stage-scaled enemy
// Stats scale dynamically based on current stage number (1-10).
// Scaling formula:
//   HP  = 30 + stage * 15
//   ATK =  8 + stage * 3
//   DEF =  2 + stage * 2
// ============================================================================
class Monster : public Entity {
protected:
    int stageLevel;

public:
    // Stage-formula constructor: stats auto-calculated from stage number
    Monster(string n, int stage)
        : Entity(n,
                 30 + stage * 15,
                  8 + stage * 3,
                  2 + stage * 2),
          stageLevel(stage) {}

    // Raw-stats constructor: used by MonsterTypes with custom scaling formulas
    Monster(string n, int hp, int atk, int def, int stage)
        : Entity(n, hp, atk, def), stageLevel(stage) {}

    string attack(Entity& target) override {
        return "[" + name + "] lunges forward! " + target.takeDamage(getAttackPower());
    }

    // Overridden by specific monster types for unique AI behaviour
    virtual string executeSkillSet(Entity& target) {
        return attack(target);
    }

    int getStageLevel() const { return stageLevel; }
};
