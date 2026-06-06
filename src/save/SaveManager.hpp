#pragma once
#include "../entities/Player.hpp"
#include <string>
#include <memory>
#include <stdexcept>

// ============================================================================
// SAVE MANAGER — CSV save/load
// Format: name,classType,hp,maxHp,attackPower,defense,skill0-3,currentStage
// Location: one file per class (save_prince.csv, save_priest.csv, etc.)
// ============================================================================

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
    static void                    save(const Player& player, const std::string& filename);
    static std::unique_ptr<Player> load(const std::string& filename);
    static bool                    saveExists(const std::string& filename);
    static void                    deleteSave(const std::string& filename);
    static std::string             saveFileName(int classType);
};
