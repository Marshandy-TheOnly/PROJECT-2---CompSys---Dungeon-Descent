#pragma once
#include "../entities/Player.hpp"
#include "../entities/Monster.hpp"

// ============================================================================
// COMBAT SYSTEM
// Manages the turn-by-turn flow of a battle between Player and Monster.
// Player always acts first, then enemy responds.
// Dodge and block are resolved inside Player::takeDamage() automatically.
// ============================================================================
class CombatSystem {
private:
    Player&  hero;
    Monster& enemy;
    string   battleLog;
    bool     isPlayerTurn;

public:
    CombatSystem(Player& p, Monster& m);

    // --- 4 Player Actions ---
    void playerAttack();
    void playerDefend();
    void playerDodge();
    void playerUltimate();

    // --- Enemy Response ---
    void executeEnemyTurn();

    // --- State Queries ---
    bool   isCombatOver()     const;
    bool   isHeroVictorious() const;
    bool   getIsPlayerTurn()  const { return isPlayerTurn; }
    string getBattleLog()     const { return battleLog; }
};
