#pragma once
#include "Player.hpp"

// HP: 100 | ATK: 10 | DEF: 4
// Specialty: Glass cannon. Highest ultimate multiplier (5x). Dies fast if hit.
class Mage : public Player {
public:
    Mage(string n) : Player(n, 100, 10, 4) {}

    string doAttack(Entity& target) override {
        return "[Attack] Mage fires a spark!" + target.takeDamage(getScaledAttack());
    }
    string doDefend() override {
        blockThisTurn = 12 + skillLevels[1] * 4;
        return "[Defend] Mage conjures a barrier! Blocking " + to_string(blockThisTurn) + " dmg this turn.";
    }
    string doUltimate(Entity& target) override {
        if (!rollUltimate())
            return "[Ultimate: Absolute Zero] Spell fizzled! The incantation failed.";
        return "[Ultimate: Absolute Zero] " + target.takeDamage(getScaledAttack() * 5);
    }
    int getClassType() const override { return 4; }
};
