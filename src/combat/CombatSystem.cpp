#include "CombatSystem.hpp"

using namespace std;

CombatSystem::CombatSystem(Player& p, Monster& m)
    : hero(p), enemy(m), isPlayerTurn(true), battleLog("A new conflict begins!") {}

// ============================================================================
// PLAYER ACTIONS
// Each action sets isPlayerTurn = false to hand control to the enemy.
// ============================================================================

void CombatSystem::playerAttack() {
    if (!isPlayerTurn || !hero.isAlive() || !enemy.isAlive()) return;
    battleLog    = hero.doAttack(enemy);
    isPlayerTurn = false;
}

void CombatSystem::playerDefend() {
    if (!isPlayerTurn || !hero.isAlive() || !enemy.isAlive()) return;
    battleLog    = hero.doDefend();
    isPlayerTurn = false;
}

void CombatSystem::playerDodge() {
    if (!isPlayerTurn || !hero.isAlive() || !enemy.isAlive()) return;
    battleLog    = hero.doDodge();
    isPlayerTurn = false;
}

void CombatSystem::playerUltimate() {
    if (!isPlayerTurn || !hero.isAlive() || !enemy.isAlive()) return;
    battleLog    = hero.doUltimate(enemy);
    isPlayerTurn = false;
}

// ============================================================================
// ENEMY TURN
// Player::takeDamage() handles dodge + block reduction internally.
// CombatSystem just fires the enemy action then resets player flags.
// ============================================================================

void CombatSystem::executeEnemyTurn() {
    if (isPlayerTurn || !hero.isAlive()) return;

    if (!enemy.isAlive()) {
        battleLog += "\n> The enemy has collapsed!";
        return;
    }

    battleLog = enemy.executeSkillSet(hero);

    hero.resetTurnFlags();
    isPlayerTurn = true;
}

// ============================================================================
// STATE CHECKS
// ============================================================================

bool CombatSystem::isCombatOver()     const { return !hero.isAlive() || !enemy.isAlive(); }
bool CombatSystem::isHeroVictorious() const { return  hero.isAlive() && !enemy.isAlive(); }
