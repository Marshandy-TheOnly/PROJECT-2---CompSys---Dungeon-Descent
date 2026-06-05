#pragma once
#include "Monster.hpp"

// ============================================================================
// BOSS — Amplified monster with turn-counter mechanic
// Stats are significantly boosted from base Monster:
//   HP  *= 2.5
//   ATK *= 1.5
//   DEF += 4
//
// Turn counter: every 3rd turn fires a devastating special skill
// that deals double ATK damage instead of the normal attack.
// ============================================================================
class Boss : public Monster {
private:
    int    turnCounter;
    string skillName;

public:
    Boss(string n, int stage, string skill = "Devastation")
        : Monster(n, stage), turnCounter(0), skillName(skill) {
        maxHp = hp = static_cast<int>(hp * 2.5f);
        attackPower = static_cast<int>(attackPower * 1.5f);
        defense += 4;
    }

    string executeSkillSet(Entity& target) override {
        turnCounter++;

        if (turnCounter % 3 == 0) {
            return "[" + name + " — " + skillName + "] "
                   "BOSS SKILL ACTIVATED! "
                   + target.takeDamage(getAttackPower() * 2);
        }

        return "[" + name + "] " + target.takeDamage(getAttackPower());
    }

    int    getTurnCounter() const { return turnCounter; }
    string getSkillName()   const { return skillName;   }
};

// ============================================================================
// BOSS FACTORY FUNCTIONS
// ============================================================================
inline Boss makeWarden() {
    return Boss("The Warden", 5, "Iron Verdict");
}

inline Boss makeAbyssLord() {
    return Boss("Abyss Lord", 10, "Void Collapse");
}
