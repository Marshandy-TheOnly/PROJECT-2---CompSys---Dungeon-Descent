#pragma once
#include "../entities/Player.hpp"

// ============================================================================
// STAGE — Abstract base for all dungeon stages
// Concrete subclasses: MonsterStage, OpportunityStage, BossStage
// ============================================================================

enum class StageType { MONSTER, OPPORTUNITY, MINI_BOSS, FINAL_BOSS };

class Stage {
public:
    virtual ~Stage() = default;
    virtual void      execute(Player& player) = 0;
    virtual StageType getType() const = 0;
};
