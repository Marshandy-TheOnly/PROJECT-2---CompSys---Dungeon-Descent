#pragma once
#include "MonsterStage.hpp"
#include "OpportunityStage.hpp"
#include "../entities/MonsterTypes.hpp"
#include "../ui/UIManager.hpp"
#include "../save/SaveManager.hpp"
#include <vector>
#include <memory>
#include <random>
#include <algorithm>
#include <iostream>

// ============================================================================
// STAGE MANAGER
// Generates and runs all 10 stages of the dungeon.
//
// Stage layout:
//   Stages 1-4  : shuffled from pool [M,M,M,M,M,O,O,O]
//   Stage  5    : Mini-Boss (The Warden)
//   Stages 6-9  : remaining slots from the same pool
//   Stage  10   : Final Boss (Abyss Lord)
//
// Constraint: at least 5 of the 8 random stages are Monster stages.
// This is guaranteed by the pool composition.
// ============================================================================
class StageManager {
private:
    vector<unique_ptr<Stage>> stages;
    bool skipNext = false;

    inline static mt19937 rng{ random_device{}() };

    unique_ptr<Monster> randomMonster(int stageNum) {
        uniform_int_distribution<int> pick(0, 4);
        switch (pick(rng)) {
            case 0: return make_unique<Goblin>(stageNum);
            case 1: return make_unique<Skeleton>(stageNum);
            case 2: return make_unique<VenomousSnake>(stageNum);
            case 3: return make_unique<StoneGolem>(stageNum);
            default: return make_unique<DarkMage>(stageNum);
        }
    }

public:
    void generateStages() {
        stages.resize(10);

        // Fixed boss stages
        stages[4] = make_unique<BossStage>(
            make_unique<Boss>(makeWarden()), StageType::MINI_BOSS);
        stages[9] = make_unique<BossStage>(
            make_unique<Boss>(makeAbyssLord()), StageType::FINAL_BOSS);

        // Pool: 5 Monster, 3 Opportunity — guaranteed ≥5 monster stages
        vector<bool> pool = { true, true, true, true, true, false, false, false };
        shuffle(pool.begin(), pool.end(), rng);

        int poolIdx = 0;
        for (int i = 0; i < 10; i++) {
            if (i == 4 || i == 9) continue;
            if (pool[poolIdx])
                stages[i] = make_unique<MonsterStage>(randomMonster(i + 1));
            else
                stages[i] = make_unique<OpportunityStage>();
            poolIdx++;
        }
    }

    void setSkipNext(bool val) { skipNext = val; }

    void runGame(Player& player, int startStage = 0) {
        for (int i = startStage; i < 10; i++) {

            if (skipNext) {
                skipNext = false;
                UIManager::showOpportunityEvent("Stage Skipped", "The void rift consumed this stage.");
                continue;
            }

            UIManager::showStageTransition(i + 1, stages[i]->getType());

            stages[i]->execute(player);

            if (!player.isAlive()) return;

            // Auto-save after each stage
            player.setCurrentStage(i + 1);
            try {
                SaveManager::save(player, SaveManager::saveFileName(player.getClassType()));
            } catch (...) {
                // Save failed — warn but don't crash
            }
        }

        if (player.isAlive())
            UIManager::showVictory(player);
    }
};
