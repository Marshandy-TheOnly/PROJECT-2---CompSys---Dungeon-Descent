#pragma once
#include "../entities/Player.hpp"
#include "../entities/Monster.hpp"
#include "../stages/Stage.hpp"
#include "Sprites.hpp"
#include <vector>
#include <string>

// ============================================================================
// UI MANAGER — FTXUI terminal UI
// All game I/O goes through here. MonsterStage, OpportunityStage, and
// StageManager call these static methods instead of using cout/cin directly.
// ============================================================================
class UIManager {
public:
    // Main screens
    static int  showMainMenu();
    static int  showClassSelect();

    // Combat
    static int  getPlayerAction(Player& player, Monster& enemy,
                                const std::vector<std::string>& log);

    // Post-combat / events
    static int  showSkillLevelUp(Player& player,
                                 const std::string& title = "VICTORY!");
    static bool showConfirm(const std::string& prompt);
    static void showOpportunityEvent(const std::string& eventName,
                                     const std::string& desc);

    // Navigation
    static void showStageTransition(int stageNum, StageType type);

    // End screens
    static void showVictory(Player& player);
    static void showGameOver();
};
