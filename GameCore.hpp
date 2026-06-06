#pragma once

// ============================================================================
// UMBRELLA INCLUDE — pulls in the full game engine in one line
// Individual files live in src/entities/, src/stages/, src/combat/,
// src/ui/, and src/save/
// ============================================================================

// --- Entities ---
#include "src/entities/Entity.hpp"
#include "src/entities/Player.hpp"
#include "src/entities/Prince.hpp"
#include "src/entities/Priest.hpp"
#include "src/entities/Berserker.hpp"
#include "src/entities/Mage.hpp"
#include "src/entities/Monster.hpp"
#include "src/entities/Boss.hpp"
#include "src/entities/MonsterTypes.hpp"

// --- Stages ---
#include "src/stages/Stage.hpp"
#include "src/stages/OpportunityStage.hpp"
#include "src/stages/MonsterStage.hpp"
#include "src/stages/StageManager.hpp"

// --- Combat ---
#include "src/combat/CombatSystem.hpp"

// --- UI ---
#include "src/ui/UIManager.hpp"
#include "src/ui/Sprites.hpp"

// --- Save ---
#include "src/save/SaveManager.hpp"
