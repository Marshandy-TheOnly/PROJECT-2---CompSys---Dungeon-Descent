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
    bool counterBonus;   // set when defend fully absorbs a hit; consumed on next attack
    int  ultCooldown;    // turns remaining before Ultimate can be used again

    inline static mt19937 rng{ random_device{}() };

public:
    Player(string n, int h, int atk, int def)
        : Entity(n, h, atk, def), blockThisTurn(0), isDodging(false),
          dodgeCooldown(0), currentStage(0), counterBonus(false), ultCooldown(0) {
        skillLevels[0] = skillLevels[1] = skillLevels[2] = skillLevels[3] = 1;
    }

    void levelUpSkill(int index) {
        if (index >= 0 && index < 4 && skillLevels[index] < 5)
            skillLevels[index]++;
    }
    int getSkillLevel(int index) const { return skillLevels[index]; }

    // ATK skill grants +8 per level so every point invested feels impactful.
    // The counter bonus (+10 flat) is consumed here the first time the player
    // uses any offensive action after a fully-blocked defend turn.
    int getScaledAttack() {
        int bonus   = counterBonus ? 10 : 0;
        counterBonus = false;
        return attackPower + skillLevels[0] * 8 + bonus;
    }

    // Entity::attack() satisfied here so subclasses only need doAttack/doDefend/doUltimate
    string attack(Entity& target) override { return doAttack(target); }

    virtual string doAttack(Entity& target) = 0;
    virtual string doDefend() = 0;

    string doDodge() {
        if (dodgeCooldown > 0)
            return "[Dodge] Still recovering — can't dodge this turn!";

        // Base 25 % (+7 % per skill level) — up from 15 % (+5 %).
        // Cooldown is only applied on SUCCESS so a failed attempt doesn't
        // burn the next turn — failing already costs the action itself.
        int chance = 25 + skillLevels[2] * 7;
        uniform_int_distribution<int> roll(1, 100);

        if (roll(rng) <= chance) {
            isDodging    = true;
            // 2 here, not 1: resetTurnFlags() runs at the end of THIS turn cycle
            // (after the enemy swings) and decrements once, leaving 1 for the
            // next player turn — so dodge is genuinely on cooldown for 1 turn.
            dodgeCooldown = 2;
            return "[Dodge] Success! (" + to_string(chance) + "% chance) — next hit will be completely negated.";
        }
        return "[Dodge] Failed! (" + to_string(chance) + "% chance) — couldn't get into position.";
    }

    virtual string doUltimate(Entity& target) = 0;

    virtual int getClassType() const = 0;

    string takeDamage(int incomingDmg) override {
        if (ultCooldown > 0) ultCooldown--;   // tick once per enemy hit regardless of dodge/block
        if (isDodging)
            return " -> " + name + " completely evaded the attack!";
        int afterBlock = incomingDmg - blockThisTurn;
        if (afterBlock <= 0) {
            // Full block — short-circuit Entity::takeDamage() so the
            // `finalDmg < 1 -> 1` clamp inside it doesn't leak 1 HP.
            counterBonus = true;
            return " -> " + name + " fully blocked the attack! Counter primed (+10 next hit).";
        }
        return Entity::takeDamage(afterBlock);
    }

    int  getBlockThisTurn()    const { return blockThisTurn; }
    bool getIsDodging()        const { return isDodging; }
    bool canDodge()            const { return dodgeCooldown == 0; }
    bool canUseUltimate()      const { return ultCooldown == 0; }
    int  getUltCooldown()      const { return ultCooldown; }
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
