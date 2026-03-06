#include "Game.h"
#include <algorithm>
#include <iostream>

Game::Game()
    : running(false), gold(500.0f), window(nullptr), 
      currentWave(0), enemiesToSpawnInWave(0), spawnTimer(0.0f), 
      spawnDelay(1.0f), waveActive(false),
      enemyTex(nullptr), bgTex(nullptr), menuTex(nullptr)
{
    // Initializer list จัดการค่าเริ่มต้นแล้ว
}

Game::~Game() {
    clean();
}

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) return false;
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) return false;

    window = new RenderWindow("Tower Defense | Press 'N' for Next Wave", 1280, 720);

    // โหลด Texture ทั้งหมด
    enemyTex = window->loadTexture("assets/test_enemy.png");
    bgTex = window->loadTexture("assets/Sky_01.png");
    menuTex = window->loadTexture("assets/Tower_Menu.png");
    
    // Icons
    fireIconTex = window->loadTexture("assets/fire_icon.png");
    iceIconTex = window->loadTexture("assets/ice_icon.png");
    windIconTex = window->loadTexture("assets/wind_icon.png");
    lightIconTex = window->loadTexture("assets/light_icon.png");
    lightningIconTex = window->loadTexture("assets/lightning_icon.png");
    waterIconTex = window->loadTexture("assets/water_icon.png");

    // Towers
    fireTowerTex = window->loadTexture("assets/fire_tower.png");
    iceTowerTex = window->loadTexture("assets/ice_tower.png");
    windTowerTex = window->loadTexture("assets/wind_tower.png");
    lightTowerTex = window->loadTexture("assets/light_tower.png");
    lightningTowerTex = window->loadTexture("assets/lightning_tower.png");
    waterTowerTex = window->loadTexture("assets/water_tower.png");

    // เส้นทางเดินศัตรู
    path = { level.gridToWorld(0,5), level.gridToWorld(15,5), 
             level.gridToWorld(15,15), level.gridToWorld(30,15) };

    lastTime = SDL_GetPerformanceCounter();
    running = true;
    return true;
}

void Game::startNextWave() {
    // ระบบ Skip Bonus: ถ้า Wave เก่ายังไม่หมด ได้เงินเพิ่ม 100
    if (waveActive && !enemies.empty()) {
        gold += 100.0f;
    }

    currentWave++;
    waveActive = true;
    enemiesToSpawnInWave = 10; // มาเป็นชุด ชุดละ 10 ตัว
    spawnTimer = 0.0f;
    std::cout << "Wave " << currentWave << " Started!" << std::endl;
}

void Game::handleEvents() {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) running = false;

        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_n) startNextWave(); // กด N เพื่อเริ่ม/ข้าม Wave
        }

        if (event.type == SDL_MOUSEMOTION) {
            mousePos.x = (float)event.motion.x;
            mousePos.y = (float)event.motion.y;
        }

        if (event.type == SDL_MOUSEBUTTONDOWN) {
            Vector2D mpos((float)event.button.x, (float)event.button.y);
            int winH = window->getheight();
            
            if (event.button.button == SDL_BUTTON_LEFT) {
                // คลิกเลือกป้อมจาก UI (แถบล่าง 100px)
                if (mpos.y >= winH - 100) {
                    int idx = (int)mpos.x / 100;
                    if (idx == 0) selectedType = TowerType::Fire;
                    else if (idx == 1) selectedType = TowerType::Ice;
                    else if (idx == 2) selectedType = TowerType::Wind;
                    else if (idx == 3) selectedType = TowerType::Light;
                    else if (idx == 4) selectedType = TowerType::Lightning;
                    else if (idx == 5) selectedType = TowerType::Water;
                }
                // วางป้อม
                else if (selectedType != TowerType::None) {
                    int gx, gy;
                    if (level.worldToGrid(mpos, gx, gy) && level.isEmpty(gx, gy)) {
                        Vector2D wPos = level.gridToWorld(gx, gy);
                        if (selectedType == TowerType::Fire) towers.push_back(std::make_unique<FireTower>(wPos, fireTowerTex));
                        else if (selectedType == TowerType::Ice) towers.push_back(std::make_unique<IceTower>(wPos, iceTowerTex));
                        else if (selectedType == TowerType::Wind) towers.push_back(std::make_unique<WindTower>(wPos, windTowerTex));
                        else if (selectedType == TowerType::Light) towers.push_back(std::make_unique<LightTower>(wPos, lightTowerTex, gold));
                        else if (selectedType == TowerType::Lightning) towers.push_back(std::make_unique<LightningTower>(wPos, lightningTowerTex));
                        else if (selectedType == TowerType::Water) towers.push_back(std::make_unique<WaterTower>(wPos, waterTowerTex));
                        
                        level.setTile(gx, gy, Level::TOWER);
                        selectedType = TowerType::None;
                    }
                }
            } else if (event.button.button == SDL_BUTTON_RIGHT) {
                selectedType = TowerType::None;
            }
        }
    }
}

void Game::update(float deltaTime) {
    // 1. ระบบ Spawn มอนสเตอร์
    if (waveActive && enemiesToSpawnInWave > 0) {
        spawnTimer += deltaTime;
        if (spawnTimer >= spawnDelay) {
            auto newEnemy = std::make_unique<Enemy>(path[0].x, path[0].y, enemyTex, path);
            
            // ถ้าเป็นตัวสุดท้ายของ Wave (ตัวที่ 1) ให้เป็นบอส
            if (enemiesToSpawnInWave == 1) {
                newEnemy->setHp(500.0f);   // แกร่งกว่า 5 เท่า
                newEnemy->setSpeed(20.0f); // บอสเดินช้า
            }
            
            enemies.push_back(std::move(newEnemy));
            enemiesToSpawnInWave--;
            spawnTimer = 0.0f;
        }
    }

    // 2. อัปเดตมอนสเตอร์และเช็คการตาย
    for (auto& enemy : enemies) {
        enemy->update(deltaTime);
        // ถ้ามอนตาย (isAlive เป็น false) และยังไม่ได้ให้รางวัล
        if (!enemy->isAlive() && !enemy->isRewardGiven()) {
            gold += 50.0f; // สังหารได้เงิน 50
            enemy->setRewardGiven(true);
        }
    }

    for (auto& tower : towers) tower->updateTower(deltaTime, enemies);

    // 3. แสดงผล Gold บน Window Title แบบ Real-time
    std::string title = "Tower Defense | Gold: " + std::to_string((int)gold) + 
                        " | Wave: " + std::to_string(currentWave) + 
                        " | Enemies: " + std::to_string(enemies.size());
    SDL_SetWindowTitle(window->getSDLWindow(), title.c_str());

    // ลบมอนสเตอร์ที่ตายหรือเข้าเส้นชัย
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
        [](const std::unique_ptr<Enemy>& e) { return !e->isAlive() || e->hasFinished(); }), 
        enemies.end());
}

void Game::render() {
    window->clear();
    if (bgTex) window->drawTextureFull(bgTex);
    level.render(window->getRenderer());

    for (auto& enemy : enemies) window->render(*enemy);
    for (auto& tower : towers) tower->render(window->getRenderer());

    // วาด UI เมนู
    int winW = window->getwidth();
    int winH = window->getheight();
    SDL_Rect menuRect = { 0, winH - 100, winW, 100 };
    SDL_SetRenderDrawColor(window->getRenderer(), 50, 50, 50, 255);
    SDL_RenderFillRect(window->getRenderer(), &menuRect);

    SDL_Texture* icons[] = { fireIconTex, iceIconTex, windIconTex, lightIconTex, lightningIconTex, waterIconTex };
    for (int i = 0; i < 6; i++) {
        SDL_Rect iconRect = { i * 100, winH - 100, 100, 100 };
        if (icons[i]) SDL_RenderCopy(window->getRenderer(), icons[i], NULL, &iconRect);
        if ((int)selectedType - 1 == i) {
            SDL_SetRenderDrawColor(window->getRenderer(), 255, 255, 0, 255);
            SDL_RenderDrawRect(window->getRenderer(), &iconRect);
        }
    }

    window->display();
}

void Game::run() {
    while (running) {
        Uint64 currentTime = SDL_GetPerformanceCounter();
        float deltaTime = (float)(currentTime - lastTime) / SDL_GetPerformanceFrequency();
        lastTime = currentTime;
        handleEvents();
        update(deltaTime);
        render();
    }
}

void Game::clean() {
    if (window) { window->cleanUp(); delete window; }
    IMG_Quit();
    SDL_Quit();
}