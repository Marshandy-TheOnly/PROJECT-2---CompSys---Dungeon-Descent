#pragma once
#include "Player.hpp"

// HP: 120 | ATK: 12 | DEF: 6
// Specialty: Sustain. Defend heals HP. Ultimate heals + deals damage.
class Priest : public Player {
public:
    Priest(string n) : Player(n, 120, 12, 6) {}

    string doAttack(Entity& target) override {
        return "[Attack] Priest strikes!" + target.takeDamage(getScaledAttack());
    }
    string doDefend() override {
        blockThisTurn  = 10 + skillLevels[1] * 4;
        int healAmt    = 15;
        setHP(getHP() + healAmt);
        return "[Defend] Priest prays! Blocks " + to_string(blockThisTurn) + " dmg and heals " + to_string(healAmt) + " HP.";
    }
    string doUltimate(Entity& target) override {
        if (ultCooldown > 0)
            return "[Ultimate: Divine Light] Recharging — " + to_string(ultCooldown) + " turn(s) left.";
        int healAmt = 30;
        setHP(getHP() + healAmt);
        ultCooldown = 2;
        if (!rollUltimate())
            return "[Ultimate: Divine Light] Healed " + to_string(healAmt) + " HP — but the smite missed!";
        return "[Ultimate: Divine Light] Healed " + to_string(healAmt) + " HP! "
               + target.takeDamage(getScaledAttack() * 2);
    }
    int getClassType() const override { return 2; }
};
