#pragma once
#include <string>
using namespace std;

// ============================================================================
// LỚP CƠ SỞ NHÂN VẬT (ENTITY BASE)
// ============================================================================
class Entity {
protected:
    string name;
    int hp, maxHp;
    int attackPower, defense;

public:
    Entity(string n, int h, int atk, int def)
        : name(n), hp(h), maxHp(h), attackPower(atk), defense(def) {}
    virtual ~Entity() = default;

    virtual string attack(Entity& target) = 0;

    string getName()     const { return name; }
    int getHP()          const { return hp; }
    int getMaxHP()       const { return maxHp; }
    int getAttackPower() const { return attackPower; }
    int getDefense()     const { return defense; }
    bool isAlive()       const { return hp > 0; }

    void setHP(int newHp) {
        hp = (newHp > maxHp) ? maxHp : (newHp < 0 ? 0 : newHp);
    }
    void setMaxHP(int v)       { maxHp = v; if (hp > maxHp) hp = maxHp; }
    void setAttackPower(int v) { attackPower = v; }
    void setDefense(int v)     { defense = v; }

    virtual string takeDamage(int incomingDmg) {
        int finalDmg = incomingDmg - defense;
        if (finalDmg < 1) finalDmg = 1;
        hp -= finalDmg;
        if (hp < 0) hp = 0;
        return " -> " + name + " took " + to_string(finalDmg) + " dmg.";
    }

    void healFull() { hp = maxHp; }
    void permanentlyBuff(int h, int a, int d) {
        maxHp += h; hp += h;
        attackPower += a;
        defense += d;
    }
};
