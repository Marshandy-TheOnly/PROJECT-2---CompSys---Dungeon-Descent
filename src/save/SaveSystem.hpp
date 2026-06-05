#pragma once
#include "../entities/Player.hpp"

// ============================================================================
// SAVE SYSTEM — Answer Sheet
// ============================================================================

// Q1: WHAT TO SAVE — What changes after each floor?
// ────────────────────────────────────────────────────────────────────────────
// After each floor, these Player state variables change:
//   1. hp, maxHp              (from combat damage + OpportunityStage healing)
//   2. attackPower, defense   (from OpportunityStage::permanentlyBuff calls)
//   3. skillLevels[4]         (leveled up after monster/boss victories)
//   4. currentStage           (incremented after completing each stage)
//   5. classType              (player class: Prince/Priest/Berserker/Mage)
//
// Monsters and Stages are NOT saved — they're regenerated fresh each floor.
// Turn flags (isDodging, blockThisTurn) are ephemeral and reset each turn.

// Q2: FILE I/O & LOAD BEHAVIOR
// ────────────────────────────────────────────────────────────────────────────
// WHEN TO LOAD?
//   - At game start: before main game loop, check for save.json
//   - If exists: load into hero object instead of starting fresh
//   - If not exists: new game (choose class, start stage 0)
//
// OVERWRITE INTO WHICH DATA STRUCTURE?
//   - The active Player* (hero object in main)
//   - Specifically: hp, maxHp, attackPower, defense, skillLevels[], currentStage
//
// IN WHICH FILES?
//   - SaveSystem.hpp (this file)      — function declarations
//   - SaveSystem.cpp                  — file I/O implementation
//   - Integrated into main.cpp        — call loadGame() before stage loop
//
// WHY?
//   - Save file acts as persistent checkpoint across program runs
//   - Load overwrites Player state so progression resumes mid-dungeon
//   - Encapsulated in SaveSystem to keep main.cpp clean

// ============================================================================
// FUNCTION DECLARATIONS
// ============================================================================

// Writes hero's current state to "save.json"
void saveGame(const Player& hero);

// Reads "save.json" and restores hero's state
// Returns true if load succeeded; false if file not found
bool loadGame(Player& hero);

// Clears the save file (for new game)
void deleteSave();

// Checks if save.json exists
bool saveExists();
