#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <iostream>
#include <string>
#include <vector>
#include <memory>

// Base Scaling Enemy - Inherits from your Character base
class Enemy : public Character {
protected:
    int level;

public:
    // Scales HP, ATK, and DEF dynamically based on the current stage number
    Enemy(std::string n, int stageLevel)
        : Character(n,
                    30 + stageLevel * 15,  // HP
                    20 + stageLevel * 5,   // Mana
                    8 + stageLevel * 3,    // Attack Power
                    2 + stageLevel * 2,    // Defense
                    5 + stageLevel * 1,    // Intelligence (Crit Chance)
                    5 + stageLevel * 1),   // Agility (Dodge Chance)
          level(stageLevel) {}

    // Polymorphic execution of standard turn attack mechanics
    std::string attack(Character& target) override {
        std::string log = name + " lunges forward to attack!";
        // Invokes your custom takeDamage function using our modified attack parameters
        log += target.takeDamage(getAttackPower(), intelligence, this);
        return log;
    }

    int getLevel() const { return level; }
};

// Specialized Boss Class (For Stage 5 and Stage 10 encounters)
class Boss : public Enemy {
private:
    std::string ultimateSkillName;
    int turnCounter;

public:
    Boss(std::string n, int stageLevel, std::string ultName)
        : Enemy(n, stageLevel), ultimateSkillName(ultName), turnCounter(0) {
        // Significantly amplify Boss parameters to make it a true milestone threat
        this->maxHp = this->hp = this->hp * 2.5;
        this->attackPower = this->attackPower * 1.5;
        this->defense = this->defense + 4;
    }

    std::string attack(Character& target) override {
        turnCounter++;
        std::string log = "";

        // Boss Mechanic: Casts a devastating boss ultimate ability every 3 turns
        if (turnCounter % 3 == 0) {
            log += "\n⚠️ BOSS SKILL ACTIVATED: " + name + " channels [" + ultimateSkillName + "]!\n";

            // 1. Inflicts direct heavy piercing damage (ignores 50% defense via forced critical)
            log += target.takeDamage(getAttackPower() * 2, intelligence, this, true);

            // 2. Applies a devastating lingering Burn effect to the player character
            target.applyBurn(5 + level * 2, 3);
            log += "\n> " + target.getName() + " was set ablaze by the Boss ultimate!";
        }
        // Standard turn cycle attack
        else {
            log += name + " heavy-slams his weapon down!";
            log += target.takeDamage(getAttackPower(), intelligence, this);
        }
        return log;
    }
};

#endif

