#pragma once

// ============================================================================
// UMBRELLA INCLUDE (minimal slice)
// Pulls in only the subsystems currently implemented in src/.
// Full version also wires UI, save, StageManager, and 4 player classes —
// those are intentionally omitted from this build.
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

// --- Stages ---
#include "src/stages/Stage.hpp"
#include "src/stages/OpportunityStage.hpp"

// --- Combat ---
#include "src/combat/CombatSystem.hpp"
