#pragma once
#include "Player.hpp"
#include <algorithm>   // for std::max

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
        if (ultCooldown > 0)
            return "[Ultimate: Desperate Outrage] Recharging — " + to_string(ultCooldown) + " turn(s) left.";
        if (!rollUltimate()) {
            ultCooldown = 2;
            return "[Ultimate: Desperate Outrage] Missed! Held back the sacrifice.";
        }
        // 25% of current HP, but never enough to KO the Berserker outright —
        // leave at least 1 HP no matter what. (Previously this could kill at HP=1.)
        int sacrifice = max(1, getHP() / 4);
        if (sacrifice >= getHP()) sacrifice = getHP() - 1;
        if (sacrifice < 0) sacrifice = 0;
        setHP(getHP() - sacrifice);
        ultCooldown = 2;
        return "[Ultimate: Desperate Outrage] Sacrificed " + to_string(sacrifice) + " HP! "
               + target.takeDamage(getScaledAttack() * 4);
    }
    int getClassType() const override { return 3; }
};
