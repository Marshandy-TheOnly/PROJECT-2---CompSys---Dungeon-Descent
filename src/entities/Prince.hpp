#pragma once
#include "Player.hpp"

// HP: 150 | ATK: 18 | DEF: 10
// Specialty: Balanced. Best base block. Reliable ultimate with no drawback.
class Prince : public Player {
public:
    Prince(string n) : Player(n, 150, 18, 10) {}

    string doAttack(Entity& target) override {
        return "[Attack] Prince strikes!" + target.takeDamage(getScaledAttack());
    }
    string doDefend() override {
        blockThisTurn = 15 + skillLevels[1] * 4;
        return "[Defend] Prince raises his shield! Blocking " + to_string(blockThisTurn) + " dmg this turn.";
    }
    string doUltimate(Entity& target) override {
        if (ultCooldown > 0)
            return "[Ultimate: Royal Strike] Recharging — " + to_string(ultCooldown) + " turn(s) left.";
        if (!rollUltimate()) {
            ultCooldown = 2;
            return "[Ultimate: Royal Strike] Missed! The strike went wide.";
        }
        ultCooldown = 2;
        return "[Ultimate: Royal Strike] " + target.takeDamage(getScaledAttack() * 3);
    }
    int getClassType() const override { return 1; }
};
