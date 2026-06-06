#pragma once
#include <vector>
#include <string>
using namespace std;

// ============================================================================
// ASCII SPRITES
// Each sprite is a vector of strings (one per line).
// Displayed in the combat screen panels.
// ============================================================================

// --- PLAYER SPRITES ---

inline vector<string> getPlayerSprite(int classType) {
    switch (classType) {
        case 1: return {  // Prince
            "   ___  ",
            "  [P P] ",
            "  (o.o) ",
            "   )|(  ",
            "  /]|[\\ "
        };
        case 2: return {  // Priest
            "    +   ",
            "  (^.^) ",
            "   )H(  ",
            "  /\\|/\\ ",
            "         "
        };
        case 3: return {  // Berserker
            " [B!!!] ",
            " (>O<)  ",
            "  )B(   ",
            "/=]|[=\\ ",
            "         "
        };
        case 4: return {  // Mage
            "  * * * ",
            " (~.~)  ",
            "  )M(   ",
            " /~~~\\  ",
            "         "
        };
        default: return {
            "  ???  ",
            " (?.?) ",
            "  )?(  ",
            "  /|\\  "
        };
    }
}

// --- MONSTER SPRITES ---

inline vector<string> getMonsterSprite(const string& name) {

    if (name.find("Goblin") != string::npos) return {
        "  ,--,  ",
        " (v  v) ",
        "  )--( ",
        "   /|\\  ",
        "         "
    };

    if (name.find("Skeleton") != string::npos) return {
        "  _-_   ",
        " (X.X)  ",
        "  )-(   ",
        "  /|\\   ",
        "         "
    };

    if (name.find("Snake") != string::npos) return {
        "  ~/\\~  ",
        " /o  o\\ ",
        " \\__/~  ",
        "  ~~~~  ",
        "         "
    };

    if (name.find("Golem") != string::npos) return {
        " /###\\  ",
        "|#o o#| ",
        "|#####| ",
        " \\###/  ",
        "         "
    };

    if (name.find("Mage") != string::npos) return {
        "  ***   ",
        " (=.=)  ",
        "  )D(   ",
        " /~~~\\  ",
        "         "
    };

    if (name.find("Warden") != string::npos) return {
        " _/\\_   ",
        "(##W##) ",
        "|#####| ",
        "/|###|\\ ",
        "         "
    };

    if (name.find("Abyss") != string::npos) return {
        "\\/\\/\\/ ",
        "(@  @)  ",
        "|ABYSS| ",
        "\\/\\/\\/ ",
        "         "
    };

    // Generic monster fallback
    return {
        "  ???   ",
        " (o_o)  ",
        "  )-(   ",
        "  /|\\   ",
        "         "
    };
}
