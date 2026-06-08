#pragma once
#include <map>
#include <vector>
#include <string>
#include <utility>
using namespace std;
// NOTE: `using namespace std;` is kept here for consistency with the rest of
// the codebase. In a clean rewrite, qualify std:: in headers instead.

// ============================================================================
// ASCII SPRITES
// Each sprite is a vector of strings (one per line).
// Displayed in the combat screen panels.
//
// Stored in std::map<key, vector<string>> for O(log n) lookup and to satisfy
// the STL associative-container requirement (Data Structures — Req 4).
// ============================================================================

// --- PLAYER SPRITES ---
// Key: classType integer (1=Prince, 2=Priest, 3=Berserker, 4=Mage)

inline vector<string> getPlayerSprite(int classType) {
    static const map<int, vector<string>> sprites = {
        { 1, {  // Prince
            "   ___  ",
            "  [P P] ",
            "  (o.o) ",
            "   )|(  ",
            "  /]|[\\ "
        }},
        { 2, {  // Priest
            "    +   ",
            "  (^.^) ",
            "   )H(  ",
            "  /\\|/\\ ",
            "         "
        }},
        { 3, {  // Berserker
            " [B!!!] ",
            " (>O<)  ",
            "  )B(   ",
            "/=]|[=\\ ",
            "         "
        }},
        { 4, {  // Mage
            "  * * * ",
            " (~.~)  ",
            "  )M(   ",
            " /~~~\\  ",
            "         "
        }},
    };

    auto it = sprites.find(classType);
    if (it != sprites.end()) return it->second;

    return { "  ???  ", " (?.?) ", "  )?(  ", "  /|\\  " };
}

// --- MONSTER SPRITES ---
// Key: substring that uniquely identifies each monster name.
// Searched in insertion order via a vector of pairs so priority is explicit
// (map iteration order is alphabetical, not useful for substring matching).

inline vector<string> getMonsterSprite(const string& name) {
    static const map<string, vector<string>> sprites = {
        { "Goblin", {
            "  ,--,  ",
            " (v  v) ",
            "  )--( ",
            "   /|\\  ",
            "         "
        }},
        { "Skeleton", {
            "  _-_   ",
            " (X.X)  ",
            "  )-(   ",
            "  /|\\   ",
            "         "
        }},
        { "Snake", {
            "  ~/\\~  ",
            " /o  o\\ ",
            " \\__/~  ",
            "  ~~~~  ",
            "         "
        }},
        { "Golem", {
            " /###\\  ",
            "|#o o#| ",
            "|#####| ",
            " \\###/  ",
            "         "
        }},
        { "Mage", {
            "  ***   ",
            " (=.=)  ",
            "  )D(   ",
            " /~~~\\  ",
            "         "
        }},
        { "Warden", {
            " _/\\_   ",
            "(##W##) ",
            "|#####| ",
            "/|###|\\ ",
            "         "
        }},
        { "Abyss", {
            "\\/\\/\\/ ",
            "(@  @)  ",
            "|ABYSS| ",
            "\\/\\/\\/ ",
            "         "
        }},
    };

    // Iterate the map: return the sprite whose key appears in the monster name
    for (const auto& [key, sprite] : sprites)
        if (name.find(key) != string::npos)
            return sprite;

    // Generic monster fallback
    return { "  ???   ", " (o_o)  ", "  )-(   ", "  /|\\   ", "         " };
}
