#pragma once
#include "Monster.hpp"

// ============================================================================
// BOSS — Amplified monster with turn-counter mechanic
// Stats are significantly boosted from base Monster:
//   HP  *= 2.0   (was 2.5)
//   ATK *= 1.3   (was 1.5)
//   DEF += 4     (Abyss Lord overrides to 16 via setDefense() in factory)
//
// Turn counter: every 4th turn fires a devastating special skill
// that deals double ATK damage instead of the normal attack.
// ============================================================================
class Boss : public Monster {
private:
    int    turnCounter;
    string skillName;

public:
    Boss(string n, int stage, string skill = "Devastation")
        : Monster(n, stage), turnCounter(0), skillName(skill) {
        maxHp = hp = static_cast<int>(hp * 2.0f);   // was 2.5 — less of a damage sponge
        attackPower = static_cast<int>(attackPower * 1.3f); // was 1.5 — survivable hits
        defense += 4;
    }

    string executeSkillSet(Entity& target) override {
        turnCounter++;

        if (turnCounter % 4 == 0) {   // was every 3rd turn — now every 4th
            return "[" + name + " — " + skillName + "] "
                   "BOSS SKILL ACTIVATED! "
                   + target.takeDamage(getAttackPower() * 2);
        }

        return "[" + name + "] " + target.takeDamage(getAttackPower());
    }

    // Warn the player one turn before the special fires
    string getForeshadow() const override {
        if ((turnCounter + 1) % 4 == 0)
            return "!! " + name + " is winding up " + skillName + "... brace yourself!";
        return "";
    }

    int    getTurnCounter() const { return turnCounter; }
    string getSkillName()   const { return skillName;   }
};

// ============================================================================
// BOSS FACTORY FUNCTIONS
// ============================================================================
inline Boss makeWarden() {
    Boss b("The Warden", 5, "Iron Verdict");
    // Significantly toned down from formula-derived values (HP 210, ATK 29, DEF 16)
    // so the mid-boss is a learning fight, not a brick wall.
    b.setMaxHP(140);
    b.setHP(140);
    b.setAttackPower(20);
    b.setDefense(8);
    return b;
}

inline Boss makeAbyssLord() {
    Boss b("Abyss Lord", 10, "Void Collapse");
    // Significantly reduced from formula-derived values (HP 360, ATK 49, DEF 26)
    b.setMaxHP(220);
    b.setHP(220);
    b.setAttackPower(28);
    b.setDefense(10);
    return b;
}
