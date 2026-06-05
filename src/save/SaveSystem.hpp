#pragma once
#include "../entities/Player.hpp"

// SaveSystem API contract:
// - Persists/restores Player progression to/from "save.json".
// - Save data is limited to persistent hero state needed to resume a run.
// - Transient per-turn/per-stage runtime state is not part of the save contract.

// Writes hero's current state to "save.json"
void saveGame(const Player& hero);

// Reads "save.json" and restores hero's state
// Returns true if load succeeded; false if file not found
bool loadGame(Player& hero);

// Clears the save file (for new game)
void deleteSave();

// Checks if save.json exists
bool saveExists();
