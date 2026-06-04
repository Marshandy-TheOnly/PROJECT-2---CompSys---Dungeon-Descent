#ifndef GAME_CORE_HPP
#define GAME_CORE_HPP

#include <string>
#include <vector>
#include <memory>
#include <cstdlib>

using namespace std; // Đã thêm theo yêu cầu của bạn

class Player;
class Character;

// ============================================================================
// VẬT PHẨM TỰ ĐỘNG SCALE ĐỘ MẠNH THEO TẦNG (FLOOR)
// ============================================================================
class Item {
protected:
    string name;
    string description;
    int floorLevel; 
public:
    Item(string n, string d, int lvl) : name(n), description(d), floorLevel(lvl) {}
    virtual ~Item() = default;

    virtual bool isInstantBoost() const = 0; 
    virtual void use(Player& user, Character& target) = 0; 

    string getName() const { return name; }
    string getDescription() const { return description; }
};

// --- Các bình Potion và Thẻ bùa chú giữ nguyên cơ chế nhưng TẦNG CÀNG CAO CHỈ SỐ CÀNG MẠNH ---
class HealthPotion : public Item {
public:
    // Tầng 1 hồi 45 HP, Tầng 5 hồi 105 HP...
    HealthPotion(int lvl) : Item("Health Potion Mk-" + to_string(lvl), "Restores " + to_string(30 + lvl * 15) + " HP.", lvl) {}
    bool isInstantBoost() const override { return false; }
    void use(Player& user, Character& target) override; 
};

class ManaPotion : public Item {
public:
    // Tầng 1 hồi 30 Mana, Tầng 5 hồi 70 Mana...
    ManaPotion(int lvl) : Item("Mana Potion Mk-" + to_string(lvl), "Restores " + to_string(20 + lvl * 10) + " Mana.", lvl) {}
    bool isInstantBoost() const override { return false; }
    void use(Player& user, Character& target) override;
};

class CursedDoll : public Item { 
public:
    // Tầng 1 giảm 6 ATK địch, Tầng 5 giảm 14 ATK địch...
    CursedDoll(int lvl) : Item("Cursed Doll", "Curses the enemy, reducing their ATK by " + to_string(4 + lvl * 2) + " for 2 turns.", lvl) {}
    bool isInstantBoost() const override { return false; }
    void use(Player& user, Character& target) override;
};

class AcidFlask : public Item { 
public:
    // Tầng 1 giảm 5 DEF địch, Tầng 5 giảm 13 DEF địch...
    AcidFlask(int lvl) : Item("Acid Flask", "Corrodes enemy armor, reducing their DEF by " + to_string(3 + lvl * 2) + " for 2 turns.", lvl) {}
    bool isInstantBoost() const override { return false; }
    void use(Player& user, Character& target) override;
};

class StatScroll : public Item {
private:
    int statType; 
public:
    StatScroll(int lvl, int type) : Item("", "", lvl), statType(type) {
        if (type == 1) {
            name = "Life Elixir (Lvl " + to_string(lvl) + ")";
            description = "Permanently increases Max HP by +" + to_string(lvl * 15) + ".";
        } else if (type == 2) {
            name = "Steel Sharpener (Lvl " + to_string(lvl) + ")";
            description = "Permanently increases ATK by +" + to_string(lvl * 3) + ".";
        } else {
            name = "Titanium Core (Lvl " + to_string(lvl) + ")";
            description = "Permanently increases DEF by +" + to_string(lvl * 2) + ".";
        }
    }
    bool isInstantBoost() const override { return true; }
    void use(Player& user, Character& target) override;
};

// ============================================================================
// LỚP CƠ SỞ NHÂN VẬT (CHARACTER BASE)
// ============================================================================
class Character {
protected:
    string name;
    int hp, maxHp, mana, maxMana;
    int attackPower, defense, intelligence, agility;
    
    // Hệ thống quản lý giảm chỉ số (Đã bổ sung bộ đếm cho Agility)
    int tempAtkDebuff, atkDebuffTurns;
    int tempDefDebuff, defDefDebuffTurns;
    int tempAgiDebuff, agiDebuffTurns; // Dùng để khóa né tránh của Rogue
    int burnDamage, burnTurns;
    int stunTurns;
    bool isCounterStance; 

public:
    Character(string n, int h, int m, int atk, int def, int intel, int agi) 
        : name(n), hp(h), maxHp(h), mana(m), maxMana(m), attackPower(atk), defense(def), intelligence(intel), agility(agi) {
            resetCombatModifiers();
        }
    virtual ~Character() = default;

    virtual string attack(Character& target) = 0;

    string getName() const { return name; }
    int getHP() const { return hp; }
    int getMaxHP() const { return maxHp; }
    int getMana() const { return mana; }
    int getMaxMana() const { return maxMana; }
    int getIntelligence() const { return intelligence; }
    bool isAlive() const { return hp > 0; }
    int getStunTurns() const { return stunTurns; }

    int getAttackPower() const { 
        int currentAtk = attackPower - tempAtkDebuff;
        return (currentAtk < 0) ? 0 : currentAtk;
    }
    int getDefense() const { 
        int currentDef = defense - tempDefDebuff;
        return (currentDef < 0) ? 0 : currentDef;
    }
    // Trả về Agility thực tế (Nếu dính Debuff tự thân của Rogue sẽ bằng 0)
    int getAgility() const {
        int currentAgi = agility - tempAgiDebuff;
        return (currentAgi < 0) ? 0 : currentAgi;
    }

    void setHP(int newHp) { hp = (newHp > maxHp) ? maxHp : (newHp < 0 ? 0 : newHp); }
    void setMana(int newMana) { mana = (newMana > maxMana) ? maxMana : (newMana < 0 ? 0 : newMana); }

    void applyAtkDebuff(int amount, int turns) { tempAtkDebuff = amount; atkDebuffTurns = turns; }
    void applyDefDebuff(int amount, int turns) { tempDefDebuff = amount; defDefDebuffTurns = turns; }
    void applyAgiDebuff(int amount, int turns) { tempAgiDebuff = amount; agiDebuffTurns = turns; }
    void applyBurn(int dmgPerTurn, int turns) { burnDamage = dmgPerTurn; burnTurns = turns; }
    void applyStun(int turns) { stunTurns = turns; }
    void enableCounterStance() { isCounterStance = true; }

    // HÀM QUAN TRỌNG: Trừ lượt hiệu ứng cuối hiệp (Nơi hoàn trả Agility cho Rogue)
    string updateDurationTicks() {
        string effectLog = "";
        if (burnTurns > 0) {
            hp -= burnDamage; if (hp < 0) hp = 0;
            effectLog += "\n> " + name + " suffered " + to_string(burnDamage) + " BURN damage!";
            burnTurns--;
        }
        if (stunTurns > 0) stunTurns--;
        if (atkDebuffTurns > 0) { if (--atkDebuffTurns == 0) tempAtkDebuff = 0; }
        if (defDefDebuffTurns > 0) { if (--defDefDebuffTurns == 0) tempDefDebuff = 0; }
        
        // GIỮ LẠI ĐẾN HẾT LƯỢT ĐỊCH: Hết hiệp đấu mới hồi phục Agility cho Rogue
        if (agiDebuffTurns > 0) { 
            if (--agiDebuffTurns == 0) tempAgiDebuff = 0; 
        }
        
        isCounterStance = false; 
        return effectLog;
    }
    
    void resetCombatModifiers() {
        tempAtkDebuff = 0; atkDebuffTurns = 0;
        tempDefDebuff = 0; defDefDebuffTurns = 0;
        tempAgiDebuff = 0; agiDebuffTurns = 0;
        burnDamage = 0; burnTurns = 0; stunTurns = 0;
        isCounterStance = false;
    }

    string takeDamage(int incomingDmg, int attackerInt, Character* attacker = nullptr, bool forceCrit = false) {
        // ĐÃ SỬA: Dùng getAgility() thay vì dùng biến raw agility để tính toán chuẩn xác tỷ lệ né dính debuff
        if (!forceCrit && (rand() % 100) < getAgility()) {
            return " -> " + name + " nimbly DODGED the attack!";
        }

        bool isCrit = forceCrit || ((rand() % 100) < attackerInt);
        if (isCrit) incomingDmg *= 2;

        int finalDmg = incomingDmg - getDefense();
        if (finalDmg < 1) finalDmg = 1; 

        // PHẢN ĐÒN CHỈ CÓ HOÀNG TỬ (PRINCE) CÓ
        if (isCounterStance) {
            int originalIncomingDmg = finalDmg;
            
            finalDmg = static_cast<int>(originalIncomingDmg * 0.5); // NHẬN 50% DAMAGE
            if (finalDmg < 1) finalDmg = 1;
            hp -= finalDmg; if (hp < 0) hp = 0;

            string log = " -> " + name + " parries! Takes only 50% damage (" + to_string(finalDmg) + " dmg).";
            
            if (attacker != nullptr) {
                int reflectedDmg = originalIncomingDmg; // PHẢN LẠI ĐỦ 100% SÁT THƯƠNG GỐC
                attacker->setHP(attacker->getHP() - reflectedDmg);
                log += " [ROYAL REFLECT!] Flung " + to_string(reflectedDmg) + " dmg back to " + attacker->getName() + ".";
            }
            return log;
        }

        hp -= finalDmg; if (hp < 0) hp = 0;
        return " -> " + name + " took " + to_string(finalDmg) + " dmg" + (isCrit ? " (CRITICAL!)" : ".");
    }

    void permanentlyBuff(int h, int a, int d) { maxHp += h; hp += h; attackPower += a; defense += d; }
    void healFull() { hp = maxHp; mana = maxMana; }
};

// ============================================================================
// LỚP NGƯỜI CHƠI (PLAYER BASE)
// ============================================================================
class Player : public Character {
protected:
    vector<unique_ptr<Item>> inventory; 
public:
    Player(string n, int h, int m, int atk, int def, int intel, int agi) 
        : Character(n, h, m, atk, def, intel, agi) {}

    virtual string useSkill1(Character& target) = 0;
    virtual string useSkill2(Character& target) = 0;
    virtual int getClassType() const = 0; 
    
    void addItemToBag(unique_ptr<Item> item) { inventory.push_back(move(item)); }
    const vector<unique_ptr<Item>>& getInventory() const { return inventory; }
    
    string useBagItem(size_t index, Character& target) {
        if (index < inventory.size()) {
            string msg = "Used " + inventory[index]->getName();
            inventory[index]->use(*this, target);
            inventory.erase(inventory.begin() + index); 
            return msg;
        }
        return "Invalid item!";
    }
};

// Triển khai bọc inline cho Item sử dụng
inline void HealthPotion::use(Player& user, Character& target) { user.setHP(user.getHP() + (30 + floorLevel * 15)); }
inline void ManaPotion::use(Player& user, Character& target) { user.setMana(user.getMana() + (20 + floorLevel * 10)); }
inline void CursedDoll::use(Player& user, Character& target) { target.applyAtkDebuff(4 + floorLevel * 2, 2); }
inline void AcidFlask::use(Player& user, Character& target) { target.applyDefDebuff(3 + floorLevel * 2, 2); }
inline void StatScroll::use(Player& user, Character& target) {
    if (statType == 1) user.permanentlyBuff(floorLevel * 15, 0, 0);
    else if (statType == 2) user.permanentlyBuff(0, floorLevel * 3, 0);
    else user.permanentlyBuff(0, 0, floorLevel * 2);
}

// ============================================================================
// HOÀN THIỆN LOGIC CHI TIẾT 5 CLASS ANH HÙNG
// ============================================================================

// 1. PRINCE (Hoàng Tử)
class Prince : public Player {
public:
    Prince(string n) : Player(n, 150, 60, 18, 10, 10, 10) {}
    string attack(Character& target) override {
        return "[Attack] Prince strikes! " + target.takeDamage(getAttackPower(), intelligence, this);
    }
    string useSkill1(Character& target) override {
        if (mana < 15) return "Not enough Mana!";
        mana -= 15;
        return "[Skill 1: Royal Strike] " + target.takeDamage(getAttackPower() * 2, intelligence, this);
    }
    string useSkill2(Character& target) override {
        if (mana < 20) return "Not enough Mana!";
        mana -= 20;
        enableCounterStance(); 
        return "[Skill 2: Sovereign Guard] Prince enters parry stance! (Takes 50% dmg, Reflects 100% dmg this turn)";
    }
    int getClassType() const override { return 1; }
};

// 2. PRIEST (Mục Sư)
class Priest : public Player {
private:
    int normalAttackBuffCharges; 
public:
    Priest(string n) : Player(n, 120, 80, 12, 6, 18, 8) { normalAttackBuffCharges = 0; }

    string attack(Character& target) override {
        int finalAtk = getAttackPower();
        string label = "[Attack] Priest strikes! ";
        if (normalAttackBuffCharges > 0) {
            finalAtk += 15; 
            normalAttackBuffCharges--;
            label = "[Buffed Attack] Holy empowered blow! ";
        }
        return label + target.takeDamage(finalAtk, intelligence, this);
    }
    string useSkill1(Character& target) override {
        if (mana < 15) return "Not enough Mana!";
        mana -= 15;
        int healAmt = intelligence * 2;
        setHP(hp + healAmt);
        return "[Skill 1: Holy Remedy] Restored " + to_string(healAmt) + " HP to self.";
    }
    string useSkill2(Character& target) override {
        if (mana < 20) return "Not enough Mana!";
        mana -= 20;
        normalAttackBuffCharges = 2; // TĂNG SÁT THƯƠNG ĐÁNH THƯỜNG 2 LẦN TIẾP THEO
        return "[Skill 2: Grace of Light] Your next 2 normal attacks are blessed with bonus damage!";
    }
    int getClassType() const override { return 2; }
};

// 3. BERSERKER (Cuồng Chiến Sĩ)
class Berserker : public Player {
public:
    Berserker(string n) : Player(n, 180, 30, 22, 5, 5, 12) {}
    string attack(Character& target) override {
        return "[Attack] Berserker swings axe! " + target.takeDamage(getAttackPower(), intelligence, this);
    }
    string useSkill1(Character& target) override {
        if (mana < 10) return "Not enough Mana!";
        mana -= 10;
        return "[Skill 1: Heavy Cleave] " + target.takeDamage(getAttackPower() * 1.5, intelligence, this);
    }
    string useSkill2(Character& target) override { 
        if (hp <= 40) return "Too weak to offer Blood Sacrifice!";
        hp -= 40; // TRỪ MÁU BẢN THÂN
        int hyperDmg = getAttackPower() * 4; // GÂY SÁT THƯƠNG RẤT NHIỀU TRONG HIỆP NÀY
        return "[Skill 2: Desperate Outrage] Sacrificed 40 HP! Devastating strike: " + target.takeDamage(hyperDmg, intelligence, this);
    }
    int getClassType() const override { return 3; }
};

// 4. MAGE (Pháp Sư)
class Mage : public Player {
public:
    Mage(string n) : Player(n, 100, 120, 10, 4, 25, 8) {}
    string attack(Character& target) override {
        return "[Attack] Mage fires spark! " + target.takeDamage(getAttackPower(), intelligence, this);
    }
    string useSkill1(Character& target) override { 
        if (mana < 20) return "Not enough Mana!";
        mana -= 20;
        target.applyBurn(intelligence, 3); // TIÊU MANA GÂY HIỆU ỨNG BURN
        return "[Skill 1: Pyroblast] Scorched enemy! (Inflicted BURN damage for 3 turns)";
    }
    string useSkill2(Character& target) override { 
        if (mana < 55) return "Not enough Mana!";
        mana -= 55; // TIÊU LƯỢNG LỚN MANA
        target.applyStun(1); // GÂY SÁT THƯƠNG VÀ LÀM ĐỊCH MẤT LƯỢT
        return "[Skill 2: Absolute Zero] Spent 55 Mana! " + target.takeDamage(getAttackPower() * 2.5, intelligence, this) + " and STUNNED the enemy!";
    }
    int getClassType() const override { return 4; }
};

// 5. ROGUE (Sát Thủ)
class Rogue : public Player {
public:
    Rogue(string n) : Player(n, 110, 45, 16, 4, 12, 22) {}
    string attack(Character& target) override {
        return "[Attack] Rogue stabs! " + target.takeDamage(getAttackPower(), intelligence, this);
    }
    string useSkill1(Character& target) override { 
        if (mana < 15) return "Not enough Mana!";
        mana -= 15;
        
        // ĐÃ SỬA: Đưa toàn bộ Agility vào hệ thống Debuff kéo dài 1 lượt đầy đủ. 
        // Quái vật sẽ đánh trước khi Debuff này tự bay màu ở hàm updateDurationTicks.
        applyAgiDebuff(agility, 1); 
        
        return "[Skill 1: Assassinate] Agility dropped to 0 for this round! Guaranteed CRITICAL: " 
               + target.takeDamage(getAttackPower() * 1.5, intelligence, this, true); // Ép nổ chí mạng 100%
    }
    string useSkill2(Character& target) override { 
        if (mana < 15) return "Not enough Mana!";
        mana -= 15;
        target.applyAtkDebuff(8, 2); // POISON ĐỊCH GIẢM DAMAGE ĐỊCH GÂY RA
        return "[Skill 2: Envenom] Poisoned target! Enemy ATK reduced by -8 for 2 turns.";
    }
    int getClassType() const override { return 5; }
};

// ============================================================================
// LỚP ĐỊCH / BOSS (ĐỂ TRỐNG SKILL SET CHO THÀNH VIÊN KHÁC LÀM)
// ============================================================================
class Monster : public Character {
public:
    Monster(int floor) : Character("Tower Monster F" + to_string(floor), 60 + floor * 20, 40, 15 + floor * 3, 2 + floor, 5, 5) {}
    
    string attack(Character& target) override {
        return "[" + name + " Attacks] " + target.takeDamage(getAttackPower(), intelligence, this);
    }

    // --- CHỪA HOÀN TOÀN PHẦN NÀY CHO NGƯỜI KHÁC LÀM ---
    // Thành viên làm AI quái vật chỉ cần override hoặc code logic tung skill set riêng biệt vào đây.
    virtual string executeSkillSet(Character& target) {
        return attack(target); // Mặc định tự đánh thường để game vận hành thông suốt
    }
};


// ============================================================================
// HỆ THỐNG ĐIỀU PHỐI CHIẾN ĐẤU (COMBAT SYSTEM)
// ============================================================================
class CombatSystem {
private:
    Player& hero;
    Monster& enemy;
    string battleLog;
    bool isPlayerTurn;

public:
    CombatSystem(Player& p, Monster& m);
    void playerAttack();
    void playerSkill1();
    void playerSkill2();
    void playerUseItem(size_t index); 
    void executeEnemyTurn(); 
    bool isCombatOver() const;
    bool isHeroVictorious() const;
    bool getIsPlayerTurn() const { return isPlayerTurn; }
    string getBattleLog() const { return battleLog; }
};

#endif // GAME_CORE_HPP