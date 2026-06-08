#pragma once
#include "../entities/Player.hpp"
#include <string>
#include <memory>
#include <stdexcept>

// ============================================================================
// SAVE MANAGER — CSV save/load
// Format: name,classType,hp,maxHp,attackPower,defense,skill0-3,currentStage,
//         stageType0-9
// Single file: save.csv
// ============================================================================

#include <vector>

class SaveNotFoundException : public std::runtime_error {
public:
    SaveNotFoundException(const std::string& msg) : std::runtime_error(msg) {}
};

class CorruptSaveException : public std::runtime_error {
public:
    CorruptSaveException(const std::string& msg) : std::runtime_error(msg) {}
};

class SaveManager {
public:
    // stageTypes: 10 integers (StageType cast to int) for all 10 stage slots
    static void                    save(const Player& player, const std::string& filename,
                                        const std::vector<int>& stageTypes);
    static std::unique_ptr<Player> load(const std::string& filename);
    // Returns empty vector if save predates stage-layout storage
    static std::vector<int>        loadStageTypes(const std::string& filename);
    static bool                    saveExists(const std::string& filename);
    static void                    deleteSave(const std::string& filename);
    static std::string             saveFileName(int classType);
};
