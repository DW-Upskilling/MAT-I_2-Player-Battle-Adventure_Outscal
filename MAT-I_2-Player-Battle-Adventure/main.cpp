#include <iostream>
#include <memory>
#include <time.h>
using namespace std;

enum PlayerType { GoldorPlayerType, NecronPlayerType, StormPlayerType };

class Player {
private:
  int health, max_health, damage;
  int defence, strength, critical_chance, ferocity, attack_speed, mana,
      health_regeneration;
  PlayerType playerType;
  string playerName;

protected:
  void setPlayerType(PlayerType playerType) { this->playerType = playerType; }

  void setHealth(int health, bool isMaxHealth) {
    this->health = health;
    if (isMaxHealth)
      this->max_health = health;
  }
  void setDefence(int defence) { this->defence = defence; }
  void setDamage(int damage) { this->damage = damage; }
  void setStrength(int strength) { this->strength = strength; }
  void setCriticalChance(int critical_chance) {
    // Critical Chance cannot be more than 100
    this->critical_chance = critical_chance > 100 ? 100 : critical_chance;
  }
  void setFerocity(int ferocity) { this->ferocity = ferocity; }
  void setAttackSpeed(int attack_speed) {
    // Attack Speed cannot be more than 100
    this->attack_speed = attack_speed > 100 ? 100 : attack_speed;
  }
  void setMana(int mana) { this->mana = mana; }
  void setHealthRegeneration(int health_regeneration) {
    // Health Regeneration cannot be more than 100
    this->health_regeneration =
        health_regeneration > 100 ? 100 : health_regeneration;
  }

  int calculateEffectiveHealth() {
    return this->getHealth() * ((this->getDefence() / 100) + 1);
  }
  int calculateDamagePerTick() {
    int base_damage = this->getDamage() * (1 + (this->getStrength() / 100));
    double crit_damage = (base_damage / (101 - this->getCriticalChance())) *
                         (1 + rand() % this->getCriticalChance());

    int bonus_attack_speed = this->getFerocity() * this->getAttackSpeed() / 100;
    double total_hits = 1 + rand() % bonus_attack_speed;

    return (int)(crit_damage * total_hits);
  }

public:
  Player(string playerName) {
    this->playerName = playerName;
    // Constant Default Values cannot be modified by external entity
    this->health = 100;
    this->max_health = 100;
    this->defence = 100;
    this->damage = 100;
    this->strength = 30;
    this->critical_chance = 10;
    this->ferocity = 10;
    this->attack_speed = 10;
    this->mana = 10;
    this->health_regeneration = 10;
    cout << "[Watcher]:: Hear ye, hear ye, brave adventurer! A great danger "
            "looms in our midst. The infamous "
         << this->getPlayerName()
         << " hath spawned in this realm, and his wickedness knows no bounds. "
         << endl;
  }
  virtual ~Player() {
    cout << "\n[Watcher]:: Rather, we must honor and celebrate our triumphs, "
            "and "
            "reflect on the lessons we have learned along the way. Though "
         << this->getPlayerName()
         << " may have left the realm, his deeds shall not be forgotten, "
            "and his contributions to our cause shall be remembered always."
         << endl;
  }

  PlayerType getPlayerType() { return playerType; }
  string getPlayerTypeName() {
    switch (this->getPlayerType()) {
    case PlayerType::GoldorPlayerType:
      return "Goldor";
    case PlayerType::StormPlayerType:
      return "Storm";
    case PlayerType::NecronPlayerType:
      return "Necron";
    };

    return "";
  }
  string getPlayerName() { return playerName; }

  int getHealth() { return health; }
  int getMaxHealth() { return max_health; }
  int getDefence() { return defence; }
  int getDamage() { return damage; }
  int getStrength() { return strength; }
  int getCriticalChance() { return critical_chance; }
  int getFerocity() { return ferocity; }
  int getAttackSpeed() { return attack_speed; }
  int getMana() { return mana; }
  int getHealthRegeneration() { return health_regeneration; }

  void show() {
    cout << "######################################" << endl;
    cout << "       ADVENTURER'S STATUS" << endl;
    cout << "######################################" << endl;
    cout << "Name: " << this->getPlayerName() << endl;
    cout << "Avatar: " << this->getPlayerTypeName() << endl;
    cout << "Health: " << this->getHealth() << endl;
    cout << "Defence: " << this->getDefence() << endl;
    cout << "Damage: " << this->getDamage() << endl;
    cout << "Strength: " << this->getStrength() << endl;
    cout << "Crititcal Chance: " << this->getCriticalChance() << endl;
    cout << "Ferocity: " << this->getFerocity() << endl;
    cout << "Attack Speed: " << this->getAttackSpeed() << endl;
    cout << "Mana: " << this->getMana() << endl;
    cout << "Health Regeneration: " << this->getHealthRegeneration() << endl;
    cout << "Effective Health: " << this->calculateEffectiveHealth() << endl;
    cout << "Damage per Tick: " << this->calculateDamagePerTick() << endl;
    cout << endl;
  }

  bool isAlive() { return this->calculateEffectiveHealth() > 0; }

  virtual void takeDamage(int damage) {
    int ehp = calculateEffectiveHealth();
    int health = (ehp - damage) / ((this->getDefence() / 100) + 1);

    cout << "[Watcher]:: " << this->getPlayerTypeName() << "("
         << this->getPlayerName() << ") health reduced by "
         << (this->getHealth() - health) << endl;

    this->setHealth(health, false);
  };
  virtual int heal() {
    int vitality =
        (this->getHealth() / 100) * (this->getHealthRegeneration() / 10);

    int health = this->getHealth() + vitality;
    if (health > this->getMaxHealth())
      health = this->getMaxHealth();

    this->setHealth(health, false);

    cout << "[Watcher]:: " << this->getPlayerTypeName() << "("
         << this->getPlayerName() << ") healed for " << vitality << endl;

    return vitality;
  }
  virtual int attack(Player *player) {
    int damage = this->calculateDamagePerTick();
    player->takeDamage(damage);

    cout << "[Watcher]:: " << this->getPlayerTypeName() << "("
         << this->getPlayerName() << ") dealt damage of " << damage << endl;

    return damage;
  }
};

// Type-A → High Health, Low Damage, Low Healing.
class GoldorPlayer : public Player {
public:
  GoldorPlayer(string playerName) : Player(playerName) {
    this->setPlayerType(PlayerType::GoldorPlayerType);

    // High Health
    this->setHealth(this->getHealth() * 200, true);
    this->setDefence(this->getDefence() * 100);

    cout << "[Wither]:: With the powers of Goldor at his command, "
         << this->getPlayerName()
         << " shall be an even greater asset in our battles against the forces "
            "of darkness that seek to destroy this realm. Together, we shall "
            "stand firm and resolute, and bring an end to the evil that "
            "plagues this land. May thy courage be unyielding and thy spirit "
            "be undaunted, for the battles ahead shall test us in ways we "
            "cannot yet imagine."
         << endl;
  }
  ~GoldorPlayer() {
    cout << "\n[Wither]:: Though it is regrettable that we must part ways with "
            "such a powerful ally, we respect "
         << this->getPlayerName()
         << " decision and wish him well on his journey. May his path be "
            "filled with adventure and triumph, and may his deeds be "
            "remembered in the annals of history."
         << endl;
  }
};
// Type B → Avg Health, Avg Damage, Avg Healing.
class StormPlayer : public Player {
public:
  StormPlayer(string playerName) : Player(playerName) {
    this->setPlayerType(PlayerType::StormPlayerType);

    // Avg Health
    this->setHealth(this->getHealth() * 20, true);
    this->setDefence(this->getDefence() * 10);
    // Avg Damage
    this->setDamage(this->getDamage() * 2);
    this->setMana(this->getMana() * 100);
    // Avg Healing
    this->setHealthRegeneration(this->getHealthRegeneration() * 2);

    cout << "[Wither]:: With these powers at his command, "
         << this->getPlayerName()
         << " shall be a formidable force to be reckoned with, and shall "
            "strike fear into "
            "the hearts of our enemies. May the lightning bolts of his wrath "
            "rain down upon our foes, and may their armies be shattered before "
            "his might. But remember, adventurer, with great power comes great "
            "responsibility. Use these powers wisely, for they can just as "
            "easily bring about our downfall as they can ensure our victory."
         << endl;
  }
  ~StormPlayer() {
    cout << "\n[Wither]:: Though it is regrettable that we must part ways with "
            "such a powerful ally, we respect "
         << this->getPlayerName()
         << " decision and wish him well on his journey. May his path be "
            "filled with adventure and triumph, and may his deeds be "
            "remembered in the annals of history."
         << endl;
  }
  int attack(Player *player) {
    int damage = Player::attack(player);

    int magical_damage = (this->getMana() / 10) * damage;
    // magical damage is random
    bool isMagicalDamage = (rand() % 100) > 50;

    if (isMagicalDamage) {
      player->takeDamage(magical_damage);
      cout << "[Watcher]:: " << this->getPlayerTypeName() << "("
           << this->getPlayerName() << ") dealt magical damage of "
           << magical_damage << endl;

      damage += magical_damage;
    }

    return damage;
  }
};
// Type C → Low Health, High Damage, Avg Healing.
class NecronPlayer : public Player {
public:
  NecronPlayer(string playerName) : Player(playerName) {
    this->setPlayerType(PlayerType::NecronPlayerType);

    // High Damage
    this->setDamage(this->getDamage() * 3);
    this->setStrength(this->getStrength() * 5);
    this->setCriticalChance(50);
    this->setFerocity(this->getFerocity() * 3);
    this->setAttackSpeed(75);
    // Avg Healing
    this->setHealthRegeneration(this->getHealthRegeneration() * 2);

    cout << "[Wither]:: Great news, adventurer! It appears that "
         << this->getPlayerName()
         << " hath been imbued with the powers of Necron, and now possesses "
            "the strength to vanquish our enemies."
         << endl;
  }
  ~NecronPlayer() {
    cout << "[Wither]:: Though it is regrettable that we must part ways with "
            "such a powerful ally, we respect "
         << this->getPlayerName()
         << " decision and wish him well on his journey. May his path be "
            "filled with adventure and triumph, and may his deeds be "
            "remembered in the annals of history."
         << endl;
  }
  int attack(Player *player) {
    int damage = Player::attack(player);

    // rage damage is random, happens less often. its upto 10x recent damage
    // dealt
    int rageDamage = 1 + rand() % 10;
    bool isRageDamage = (rand() % 1000) < 100;

    if (isRageDamage) {
      player->takeDamage(damage * rageDamage);
      cout << "[Watcher]:: " << this->getPlayerTypeName() << "("
           << this->getPlayerName() << ") dealt rage damage of "
           << (damage * rageDamage) << endl;

      damage += (damage * rageDamage);
    }

    return damage;
  }

  int heal() {
    int vitality = Player::heal();

    // rage heal, when rage necron provides 100% health refill
    bool isRageHeal = (rand() % 1000) < 10;

    if (isRageHeal) {
      this->setHealth(this->getMaxHealth(), false);

      cout << "[Watcher]:: " << this->getPlayerTypeName() << "("
           << this->getPlayerName() << ") rage healed for "
           << (isRageHeal ? this->getMaxHealth() : vitality) << endl;
    }
    return vitality;
  }
};

class TwoPlayerBattleAdventure {
private:
  unique_ptr<Player> players[2];

  void menu() {
    cout << "######################################" << endl;
    cout << "Welcome to Two Player Battle Adventure" << endl;
    cout << "######################################" << endl;
    cout << endl;
    int choice = 0;
    while (true) {
      srand(time(0));
      cout << "\n[Watcher]:: Listen closely, adventurers, for the fate of this "
              "quest lies in thy hands. Thou must choose thy path with great "
              "care. Behold, the options before thee:"
           << endl;
      cout << "\t1.Embark upon thy journey and begin the game." << endl;
      cout << "\t2.Turn back now and abandon thy quest." << endl;
      cout << "[Query]:: So, what shall it be? Which path shall thou take? "
              "Enter thy choice now and let destiny guide thee:";
      cin >> choice;

      switch (choice) {
      case 1:
        cout << "\n[Watcher]:: Hark! 'Tis a great honor, for thou art the "
                "first among players to enter this realm!"
             << endl;
        setup(0);
        this->players[0]->show();
        cout << "\n[Watcher]:: Greetings, valiant traveler! Thou art the "
                "second to set foot upon this fabled land, and 'tis an honor "
                "to have thee join our ranks."
             << endl;
        setup(1);
        this->players[1]->show();
        cout << endl;
        game();
        reset();
        break;
      case 2:
        // exits from the loop and return from the function
        return;
      default:
        cout << "[Error]:: Ah, brave adventurer, it seems thou hast not chosen "
                "from the options presented. Pray, heed my words and choose "
                "again, for only then can thy journey commence. The fate of "
                "this realm rests upon thy choices, and only thou canst make "
                "the right one. Take heart, and choose wisely this time, for "
                "the path ahead is fraught with peril."
             << endl;
        break;
      }
    }
  }

  void game() {
    srand(time(0));

    cout << "[Quest Master]:: Hark, brave Adventurers! The time has come to "
            "test "
            "thy mettle and determine who among thee is the true Champion of "
            "Champions!"
         << endl;

    cout << "[Quest Master]:: Let the battle begin, and may the best warrior "
            "emerge "
            "victorious!\n"
         << endl;

    int choice = 0;
    while (choice != 4) {
      for (int i = 0; i < 2 && choice != 4; i++) {
        choice = 0;
        if (!this->players[i]->isAlive() ||
            !this->players[(i + 1) % 2]->isAlive())
          choice = 4;
        while (choice == 0) {
          choice = this->getPlayerMove(this->players[i].get());
          switch (choice) {
          case 1:
            this->players[i]->heal();
            break;
          case 2:
            this->players[i]->attack(this->players[(i + 1) % 2].get());
            break;
          case 3:
            this->players[i]->show();
            choice = 0;
            break;
          case 4:
            break;
          default:
            choice = 0;
            break;
          }
        }
      }
    }

    cout << "\n[Watcher]:: Suddenly, the air crackles with an eerie energy as "
            "the fearsome Ender Dragon materializes before the adventurers, "
            "its great wings spreading wide as it surveys the scene before it. "
            "The adventurers can feel the weight of its gaze upon them, and "
            "they realize with a start that they have made a grave mistake in "
            "betraying each other."
         << endl;

    cout << "\n[Ender Dragon]:: Foolish mortals, the dragon hisses, its voice "
            "like the sound of grinding stones. You thought you could best me "
            "by turning on each other? You have only succeeded in sealing your "
            "own doom."
         << endl;
  }

  int getPlayerMove(Player *player) {
    int choice;

    cout << "\n[Kudra]:: Hail, noble adventurer " << player->getPlayerName()
         << "! Pray tell, what move shall thou make in this fateful battle? I "
            "have a few options for thee to consider."
         << endl;

    cout << "\t1. Heal thy wounds and fortify thyself" << endl;
    cout << "\t2. Strike with all thy might!" << endl;
    cout << "\t3. Display thy battle stats" << endl;
    cout << "\t4. Withdraw from battle" << endl;
    cout << "Choose thy path wisely, for the fate of this battle hangs in the "
            "balance?";
    cin >> choice;

    return choice;
  }

  void setup(int currentPlayer) {
    string playerName;
    cout << "[NPC]:: Ahoy there, brave wanderer! Pray, do tell, by what "
            "name shall I address thee, mighty adventurer?";
    cin.ignore();
    getline(cin, playerName);

    cout << "[NPC]:: "
         << "Greetings, bold adventurer, " << playerName
         << "! 'Tis a pleasure to make thy acquaintance. But before "
            "thou embark upon thy quest in this realm, thou must "
            "first choose an avatar of the Great Wither. Remember, "
            "thy decision could determine the outcome of this great "
            "adventure. Choose wisely and may fortune favor thee!"
         << endl;
    int choice = 0;
    while (!choice) {
      cout << "\t1. Goldor - High Health, Low Damage, Low Healing" << endl;
      cout << "\t2. Storm - Avg Health, Avg Damage, Avg Healing" << endl;
      cout << "\t3. Necron - Low Health, High Damage, Avg Healing" << endl;
      cout << "[NPC]:: Pray tell, what is thy chosen path? Which avatar of the "
              "Great Wither doth thou desire to accompany thee on this "
              "perilous journey?";
      cin >> choice;
      Player *ptr = NULL;
      switch (choice) {
      case 1:
        ptr = new GoldorPlayer(playerName);
        break;
      case 2:
        ptr = new StormPlayer(playerName);
        break;
      case 3:
        ptr = new NecronPlayer(playerName);
        break;
      default:
        choice = 0;
      }

      players[currentPlayer].reset(ptr);
    }
  }

  void reset() {
    string buffer;
    players[0].reset();
    players[1].reset();

    cout << "\n[Unknown Voice]:: The winds of time have swept across the "
            "land, and the echoes of our adventure have faded into the past... "
            "\n"
         << endl;
    cout << "Press enter to continue" << endl;
    cin.ignore();
    getline(cin, buffer);
  }

public:
  TwoPlayerBattleAdventure() {}
  void start() { menu(); }

  ~TwoPlayerBattleAdventure() {
    cout << "\n\n[Watcher]:: Alas, brave adventurers, 'tis with a heavy heart "
            "that I bid thee farewell. Mayhap this journey was not meant "
            "to be, but fear not, for there are many paths yet to be "
            "discovered. May the winds of fate guide thee on thy next "
            "adventure, and may thy heart remain steadfast and true. "
            "Farewell, and may we meet again on another day."
         << endl;
  }
};

int main() {
  srand(time(0));
  TwoPlayerBattleAdventure twoPlayerBattleAdventure;
  twoPlayerBattleAdventure.start();
}