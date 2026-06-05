#pragma once
#include "Entity.hpp"
#include <random>

// ============================================================================
// LỚP NGƯỜI CHƠI (PLAYER BASE)
// skillLevels[4]: 0=ATK  1=DEF  2=DODGE  3=ULT
// All start at level 1, max level 5.
// ============================================================================
class Player : public Entity {
protected:
    int  skillLevels[4];
    int  blockThisTurn;
    bool isDodging;
    int  dodgeCooldown;
    int  currentStage;

    inline static mt19937 rng{ random_device{}() };

public:
    Player(string n, int h, int atk, int def)
        : Entity(n, h, atk, def), blockThisTurn(0), isDodging(false),
          dodgeCooldown(0), currentStage(0) {
        skillLevels[0] = skillLevels[1] = skillLevels[2] = skillLevels[3] = 1;
    }

    void levelUpSkill(int index) {
        if (index >= 0 && index < 4 && skillLevels[index] < 5)
            skillLevels[index]++;
    }
    int getSkillLevel(int index) const { return skillLevels[index]; }

    int getScaledAttack() const { return attackPower + skillLevels[0] * 3; }

    // Entity::attack() satisfied here so subclasses only need doAttack/doDefend/doUltimate
    string attack(Entity& target) override { return doAttack(target); }

    virtual string doAttack(Entity& target) = 0;
    virtual string doDefend() = 0;

    string doDodge() {
        if (dodgeCooldown > 0)
            return "[Dodge] Still recovering — can't dodge this turn!";

        int chance = 15 + skillLevels[2] * 5;
        uniform_int_distribution<int> roll(1, 100);
        dodgeCooldown = 1;

        if (roll(rng) <= chance) {
            isDodging = true;
            return "[Dodge] Success! (" + to_string(chance) + "% chance) — next hit will be completely negated.";
        }
        return "[Dodge] Failed! (" + to_string(chance) + "% chance) — couldn't get into position.";
    }

    virtual string doUltimate(Entity& target) = 0;

    virtual int getClassType() const = 0;

    string takeDamage(int incomingDmg) override {
        if (isDodging)
            return " -> " + name + " completely evaded the attack!";
        int afterBlock = incomingDmg - blockThisTurn;
        if (afterBlock < 0) afterBlock = 0;
        return Entity::takeDamage(afterBlock);
    }

    int  getBlockThisTurn()  const { return blockThisTurn; }
    bool getIsDodging()      const { return isDodging; }
    bool canDodge()          const { return dodgeCooldown == 0; }
    int  getCurrentStage()   const { return currentStage; }
    void setCurrentStage(int s)    { currentStage = s; }

    void resetTurnFlags() {
        blockThisTurn = 0;
        isDodging     = false;
        if (dodgeCooldown > 0) dodgeCooldown--;
    }

    bool rollUltimate() {
        int missChance = 35 - skillLevels[3] * 5;
        if (missChance < 5) missChance = 5;
        uniform_int_distribution<int> roll(1, 100);
        return roll(rng) > missChance;
    }
};
