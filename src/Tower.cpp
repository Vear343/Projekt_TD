// #include <iostream>
// #include <vector>
// #include <cmath>
// #include <cstdlib>
// #include <ctime>
// #include <algorithm>
// #include <string>
// #include <sstream>
// #include <fstream>
// #include <map>
// #include <set>
// #include <queue>
// #include "main.cpp" 
// #include "tower.cpp"

// #define MAP_WIDTH 10
// #define MAP_HEIGHT 10

// const int tileWidth = 64;
// const int tileHeight = 32;

// struct Vector2 {
//     float x, y;
// };

// enum TileType {
//     EMPTY,
//     PATH,
//     OCCUPIED
// };

// struct Tile {
//     TileType type = EMPTY;
//     bool buildable = true;
// };

// Tile mapGrid[MAP_WIDTH][MAP_HEIGHT];

// //ป้อมพื้นฐาน การวางป้อมและหมุนหาเมาส์
// class Tower {
// private:
//     int gridX, gridY;
//     Vector2 worldPos;
//     float rotation; // degree

// public:
//     Tower(int gx, int gy) {
//         setGridPosition(gx, gy);
//         rotation = 0.0f;
//     }

//     void setGridPosition(int gx, int gy) {
//         gridX = gx;
//         gridY = gy;

//         // Grid → Isometric world
//         worldPos.x = (gx - gy) * tileWidth / 2.0f;
//         worldPos.y = (gx + gy) * tileHeight / 2.0f;
//     }

//     void rotateTo(Vector2 target) {
//         float dx = target.x - worldPos.x;
//         float dy = target.y - worldPos.y;

//         rotation = atan2(dy, dx) * 180.0f / 3.14159f;
//     }

//     void render() {
//         std::cout << "Tower at (" << gridX << "," << gridY 
//                   << ") Rot: " << rotation << "\n";
//     }
// };

// ///////////////////////////////////////////////////////////พื้นที่เก็บป้อมทั้งหมดในเกม (จริงๆ ควรใช้ smart pointer แต่ขอให้เข้าใจง่ายๆ)

// std::vector<Tower*> towers;


// // Isometric Screen → Grid
// Vector2 screenToGrid(float screenX, float screenY) {

//     float gx = (screenY / tileHeight + screenX / tileWidth);
//     float gy = (screenY / tileHeight - screenX / tileWidth);

//     return { floor(gx), floor(gy) };
// }

// ///////////////////////////////////////////////////////////การตรวจสอบว่าพื้นที่นั้นวางป้อมได้หรือไม่

// bool canPlace(int x, int y) {
//     if (x < 0 || y < 0 || x >= MAP_WIDTH || y >= MAP_HEIGHT)
//         return false;

//     if (mapGrid[x][y].type != EMPTY)
//         return false;

//     if (!mapGrid[x][y].buildable)
//         return false;

//     return true;
// }

// ///////////////////////////////////////////////////////////การวางป้อม

// void placeTower(int x, int y) {
//     if (!canPlace(x, y)) {
//         std::cout << "Cannot place here\n";
//         return;
//     }

//     Tower* tower = new Tower(x, y);
//     towers.push_back(tower);

//     mapGrid[x][y].type = OCCUPIED;

//     std::cout << "Tower placed at " << x << "," << y << "\n";
// }

// ///////////////////////////////////////////////////////////
// // ตัวอย่าง Main Loop จำลอง

// int main() {

//     // สมมุติ mouse click
//     float mouseX = 200;
//     float mouseY = 150;

//     Vector2 grid = screenToGrid(mouseX, mouseY);

//     int gx = (int)grid.x;
//     int gy = (int)grid.y;

//     // Preview
//     if (canPlace(gx, gy))
//         std::cout << "Preview: GREEN\n";
//     else
//         std::cout << "Preview: RED\n";

//     // คลิกวาง
//     placeTower(gx, gy);

//     // หมุนป้อมหาเมาส์
//     for (auto tower : towers) {
//         tower->rotateTo({ mouseX, mouseY });
//         tower->render();
//     }

//     return 0;
// }

// ////////////////////////////////////////////////////

// ////tower template

// ////////////////////////////////////////////////////// Vector2 และฟังก์ชันคำนวณระยะทาง


// struct Vector2 {
//     float x, y;
// };

// float distance(Vector2 a, Vector2 b) {
//     return std::sqrt(
//         (a.x - b.x) * (a.x - b.x) +
//         (a.y - b.y) * (a.y - b.y)
//     );
// }

// /////////////////////////////////////////////////////
// // Enemy Class (ไว้ทดสอบ)

// class Enemy {
// public:
//     Vector2 position;
//     float hp;
//     float speed;
//     bool alive;

//     Enemy(float x, float y) {
//         position = {x, y};
//         hp = 100;
//         speed = 20;
//         alive = true;
//     }

//     void takeDamage(float dmg) {
//         hp -= dmg;
//         if (hp <= 0) {
//             alive = false;
//             std::cout << "Enemy Dead\n";
//         }
//     }
// };

// //////////////////////////////////////////////////////
// // Tower Base Class

// class Tower {
// protected:
//     Vector2 position;
//     float damage;
//     float range;
//     float attackSpeed;
//     float cooldown;
//     float rotation;

// public:
//     Tower(Vector2 pos) {
//         position = pos;
//         cooldown = 0;
//         rotation = 0;
//     }

//     virtual void update(float dt, std::vector<Enemy*>& enemies) = 0;

//     void rotateTo(Vector2 target) {
//         float dx = target.x - position.x;
//         float dy = target.y - position.y;
//         rotation = atan2(dy, dx) * 180.0f / 3.14159f;
//     }
// };

// //////////////////////////////////////////////////////
// // LightTower (Hit Scan)

// class LightTower : public Tower {
// public:
//     LightTower(Vector2 pos) : Tower(pos) {
//         damage = 25;
//         range = 150;
//         attackSpeed = 0.5f; // ยิงทุก 0.5 วิ
//     }

//     Enemy* findTarget(std::vector<Enemy*>& enemies) {
//         for (auto enemy : enemies) {
//             if (enemy->alive &&
//                 distance(position, enemy->position) <= range)
//                 return enemy;
//         }
//         return nullptr;
//     }

//     void attack(Enemy* target) {
//         std::cout << "LightTower ยิง!\n";
//         target->takeDamage(damage);

//         // Hit Scan Effect (แค่ debug)
//         std::cout << "ยิงจาก (" 
//                   << position.x << "," << position.y 
//                   << ") ไป (" 
//                   << target->position.x << "," 
//                   << target->position.y << ")\n";
//     }

//     void update(float dt, std::vector<Enemy*>& enemies) override {
//         cooldown -= dt;

//         Enemy* target = findTarget(enemies);

//         if (target) {
//             rotateTo(target->position);

//             if (cooldown <= 0) {
//                 attack(target);
//                 cooldown = attackSpeed;
//             }
//         }
//     }
// };

// //////////////////////////////////////////////////////
// // MAIN TEST

// int main() {

//     std::vector<Enemy*> enemies;
//     enemies.push_back(new Enemy(200, 200));

//     LightTower lightTower({100, 100});

//     // จำลอง game loop
//     for (int i = 0; i < 10; i++) {

//         float deltaTime = 0.16f; // ~60 FPS

//         lightTower.update(deltaTime, enemies);

//         if (!enemies[0]->alive)
//             break;
//     }

//     return 0;
// }
// //////////////status effect template

//  enum StatusType {
//     BURN,
//     SLOW,
//     STUN
// };

// struct StatusEffect {
//     StatusType type;
//     float duration;
//     float value;     // ใช้เก็บ %slow หรือ damage per sec
// };
// /////////////Enemy รองรับ Status Effect
// class Enemy {
// public:
//     Vector2 position;
//     float hp;
//     float baseSpeed;
//     float currentSpeed;
//     bool alive;

//     std::vector<StatusEffect> effects;

//     Enemy(float x, float y) {
//         position = {x, y};
//         hp = 200;
//         baseSpeed = 40;
//         currentSpeed = baseSpeed;
//         alive = true;
//     }

//     void takeDamage(float dmg) {
//         hp -= dmg;
//         if (hp <= 0) {
//             alive = false;
//             std::cout << "Enemy Dead\n";
//         }
//     }

//     void applyEffect(StatusEffect effect) {
//         effects.push_back(effect);
//     }

//     void update(float dt) {

//         currentSpeed = baseSpeed;

//         for (int i = 0; i < effects.size(); i++) {

//             effects[i].duration -= dt;

//             switch (effects[i].type) {

//             case BURN:
//                 takeDamage(effects[i].value * dt);
//                 break;

//             case SLOW:
//                 currentSpeed = baseSpeed * (1.0f - effects[i].value);
//                 break;

//             case STUN:
//                 currentSpeed = 0;
//                 break;
//             }
//         }

//         // ลบ effect หมดเวลา
//         effects.erase(
//             std::remove_if(effects.begin(), effects.end(),
//                 [](StatusEffect& e) { return e.duration <= 0; }),
//             effects.end()
//         );

//         // เดินไปข้างหน้า (สมมุติแกน X)
//         position.x += currentSpeed * dt;
//     }
// };
// ///////////ป้อมไฟ
// class FireTower : public Tower {
// public:
//     FireTower(Vector2 pos) : Tower(pos) {
//         damage = 20;
//         range = 150;
//         attackSpeed = 1.0f;
//     }

//     void attack(Enemy* target) {

//         target->takeDamage(damage);

//         StatusEffect burn;
//         burn.type = BURN;
//         burn.duration = 3.0f;
//         burn.value = 10; // 10 dmg/sec

//         target->applyEffect(burn);

//         std::cout << "Burn applied!\n";
//     }
// };
// //////ป้อมน้ำ
// class FireTower : public Tower {
// public:
//     FireTower(Vector2 pos) : Tower(pos) {
//         damage = 20;
//         range = 150;
//         attackSpeed = 1.0f;
//     }

//     void attack(Enemy* target) {

//         target->takeDamage(damage);

//         StatusEffect burn;
//         burn.type = BURN;
//         burn.duration = 3.0f;
//         burn.value = 10; // 10 dmg/sec

//         target->applyEffect(burn);

//         std::cout << "Burn applied!\n";
//     }
// };
// //////ป้อมสายฟ้า
// class LightningTower : public Tower {
// public:
//     LightningTower(Vector2 pos) : Tower(pos) {
//         damage = 15;
//         range = 150;
//         attackSpeed = 1.0f;
//     }

//     void attackChain(Enemy* first, std::vector<Enemy*>& enemies) {

//         first->takeDamage(damage);

//         int chainCount = 0;

//         for (auto enemy : enemies) {

//             if (enemy != first &&
//                 distance(first->position, enemy->position) < 100 &&
//                 chainCount < 2) {

//                 enemy->takeDamage(damage * 0.7f);
//                 chainCount++;
//             }
//         }

//         std::cout << "Lightning chain!\n";
//     }
// };
// ///////ป้อมพัดลม
// class WindTower : public Tower {
// public:
//     WindTower(Vector2 pos) : Tower(pos) {
//         attackSpeed = 1.5f;
//     }

//     void attack(Enemy* target) {

//         target->position.x -= 50; // ผลักถอยหลัง 50 หน่วย

//         std::cout << "Enemy pushed back!\n";
//     }
// };
// //// ป้อมน้ำแข็ง
// class IceTower : public Tower {
// public:
//     IceTower(Vector2 pos) : Tower(pos) {
//         damage = 10;
//         attackSpeed = 2.0f;
//     }

//     void attack(Enemy* target) {

//         target->takeDamage(damage);

//         StatusEffect stun;
//         stun.type = STUN;
//         stun.duration = 1.5f;
//         stun.value = 0;

//         target->applyEffect(stun);

//         std::cout << "Enemy stunned!\n";
//     }
// };
