#pragma once
#include "Stage.hpp"
#include "../entities/Monster.hpp"
#include "../entities/Boss.hpp"
#include "../combat/CombatSystem.hpp"
#include "../ui/UIManager.hpp"
#include "../save/SaveManager.hpp"
#include <memory>
#include <vector>

// ============================================================================
// MONSTER STAGE
// Runs a full combat encounter. Uses UIManager for all display and input.
// On victory: UIManager::showSkillLevelUp → player.levelUpSkill()
// On defeat:  UIManager::showGameOver()
// ============================================================================
class MonsterStage : public Stage {
protected:
    unique_ptr<Monster> monster;
    StageType           type;

public:
    MonsterStage(unique_ptr<Monster> m, StageType t = StageType::MONSTER)
        : monster(move(m)), type(t) {}

    StageType getType() const override { return type; }

    void execute(Player& player) override {
        CombatSystem   combat(player, *monster);
        vector<string> log = { "Combat begins!" };

        while (!combat.isCombatOver()) {
            int action = UIManager::getPlayerAction(player, *monster, log);

            switch (action) {
                case 1: combat.playerAttack();   break;
                case 2: combat.playerDefend();   break;
                case 3: combat.playerDodge();    break;
                case 4: combat.playerUltimate(); break;
            }
            log.push_back(combat.getBattleLog());

            if (!combat.isCombatOver()) {
                combat.executeEnemyTurn();
                log.push_back(combat.getBattleLog());
            }
        }

        if (combat.isHeroVictorious()) {
            int skill = UIManager::showSkillLevelUp(player);
            player.levelUpSkill(skill);
        } else {
            // Delete save so a dead run cannot be continued
            SaveManager::deleteSave(SaveManager::saveFileName(player.getClassType()));
            UIManager::showGameOver();
            player.setHP(0); // ensure isAlive() returns false for StageManager
        }
    }
};

// ============================================================================
// BOSS STAGE — MonsterStage typed as MINI_BOSS or FINAL_BOSS
// ============================================================================
class BossStage : public MonsterStage {
public:
    BossStage(unique_ptr<Boss> b, StageType t)
        : MonsterStage(move(b), t) {}
};
