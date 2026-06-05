#pragma once
#include "Player.hpp"

// HP: 180 | ATK: 22 | DEF: 5
// Specialty: Highest HP + ATK. Ultimate sacrifices own HP for massive damage.
//            Sacrifice only triggers if the ultimate HITS.
class Berserker : public Player {
public:
    Berserker(string n) : Player(n, 180, 22, 5) {}

    string doAttack(Entity& target) override {
        return "[Attack] Berserker swings!" + target.takeDamage(getScaledAttack());
    }
    string doDefend() override {
        blockThisTurn = 8 + skillLevels[1] * 4;
        return "[Defend] Berserker braces! Blocking " + to_string(blockThisTurn) + " dmg this turn.";
    }
    string doUltimate(Entity& target) override {
        if (!rollUltimate())
            return "[Ultimate: Desperate Outrage] Missed! Held back the sacrifice.";
        int sacrifice = 40;
        setHP(getHP() - sacrifice);
        return "[Ultimate: Desperate Outrage] Sacrificed " + to_string(sacrifice) + " HP! "
               + target.takeDamage(getScaledAttack() * 4);
    }
    int getClassType() const override { return 3; }
};
