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
    enemyTex = window->loadTexture("assets/Enemy_Beta.png");
    bgTex = window->loadTexture("assets/Sky_01.png");

    // Load Towers
    fireTowerTex      = window->loadTexture("assets/Fire_Tower.png");
    iceTowerTex       = window->loadTexture("assets/Ice_Tower.png");
    windTowerTex      = window->loadTexture("assets/Wind_Tower.png");
    lightTowerTex     = window->loadTexture("assets/Light_Tower.png");
    lightningTowerTex = window->loadTexture("assets/Lightning_Tower.png");
    waterTowerTex     = window->loadTexture("assets/Water_Tower.png");
    
    // Load projectile textures
    fireProjectileTexture = window->loadTexture("assets/Fire_Ball.png");
    iceProjectileTexture = window->loadTexture("assets/Ice_Ball.png");
    windProjectileTexture = window->loadTexture("assets/Wind_Ball.png");
    lightProjectileTexture = window->loadTexture("assets/Light_Ball.png");
    lightningProjectileTexture = window->loadTexture("assets/Lightning_Ball.png");
    waterProjectileTexture = window->loadTexture("assets/Water_Ball.png");
    menuTex = window->loadTexture("assets/Tower_Menu.png");
    
    // Icons
    fireIconTex = window->loadTexture("assets/Fire_Tower.png");
    iceIconTex = window->loadTexture("assets/Ice_Tower.png");
    windIconTex = window->loadTexture("assets/Wind_Tower.png");
    lightIconTex = window->loadTexture("assets/Light_Tower.png");
    lightningIconTex = window->loadTexture("assets/Lightning_Tower.png");
    waterIconTex = window->loadTexture("assets/Water_Tower.png");

    path = {
        level.gridToWorld(0,0),
        level.gridToWorld(10,0),
        level.gridToWorld(10,10),
        level.gridToWorld(20,10),
        level.gridToWorld(20,20),
        level.gridToWorld(30,40),
    };

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
                        Vector2D world = level.gridToWorld(gx, gy);
                        if      (selectedType == TowerType::Fire)      towers.push_back(std::make_unique<FireTower>(world, fireTowerTex, fireProjectileTexture));
                        else if (selectedType == TowerType::Ice)       towers.push_back(std::make_unique<IceTower>(world, iceTowerTex, iceProjectileTexture));
                        else if (selectedType == TowerType::Wind)      towers.push_back(std::make_unique<WindTower>(world, windTowerTex, windProjectileTexture));
                        else if (selectedType == TowerType::Light)     towers.push_back(std::make_unique<LightTower>(world, lightTowerTex, gold));
                        else if (selectedType == TowerType::Lightning) towers.push_back(std::make_unique<LightningTower>(world, lightningTowerTex, lightningProjectileTexture));
                        else if (selectedType == TowerType::Water)     towers.push_back(std::make_unique<WaterTower>(world, waterTowerTex, waterProjectileTexture));

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

    // Render background
    if (bgTex) window->drawTextureFull(bgTex);

    // Render level
    level.render(window->getRenderer());

    // Render enemies and towers
    for (auto& enemy : enemies) window->render(*enemy);
    for (auto& tower : towers) tower->render(window->getRenderer());

    
    // Render projectiles from all towers
    for (auto& tower : towers) {
        for (const auto& p : tower->projectiles)
        window->render(*p);
    }

    // Ghost Preview
    if (selectedType != TowerType::None) {
        int gx, gy;
        if (level.worldToGrid(mousePos, gx, gy)) {
            Vector2D world = level.gridToWorld(gx, gy);
            SDL_Rect ghostRect = { (int)world.x - 16, (int)world.y - 16, 32, 32 };
            
            switch (selectedType) {
                case TowerType::Fire:      SDL_SetRenderDrawColor(window->getRenderer(), 255, 0, 0, 150); break;
                case TowerType::Ice:       SDL_SetRenderDrawColor(window->getRenderer(), 0, 150, 255, 150); break;
                case TowerType::Wind:      SDL_SetRenderDrawColor(window->getRenderer(), 200, 255, 200, 150); break;
                case TowerType::Light:     SDL_SetRenderDrawColor(window->getRenderer(), 255, 255, 150, 150); break;
                case TowerType::Lightning: SDL_SetRenderDrawColor(window->getRenderer(), 200, 0, 255, 150); break;
                case TowerType::Water:     SDL_SetRenderDrawColor(window->getRenderer(), 0, 0, 255, 150); break;
                default: break;
            }
            if (!level.isEmpty(gx, gy)) SDL_SetRenderDrawColor(window->getRenderer(), 255, 0, 0, 100);
            SDL_RenderFillRect(window->getRenderer(), &ghostRect);
        }
    }
    // UI Rendering
    int winW = window->getwidth();
    int winH = window->getheight();
    const int MENU_H = 100;
    
    SDL_Rect menuRect = { 0, winH - MENU_H, winW, MENU_H };
    SDL_SetRenderDrawBlendMode(window->getRenderer(), SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(window->getRenderer(), 40, 40, 40, 200);
    SDL_RenderFillRect(window->getRenderer(), &menuRect);

    SDL_Texture* icons[] = { fireIconTex, iceIconTex, windIconTex, lightIconTex, lightningIconTex, waterIconTex };
    for (int i = 0; i < 6; i++) {
        SDL_Rect btnRect = { i * 100, winH - MENU_H, 100, MENU_H };
        if (icons[i]) SDL_RenderCopy(window->getRenderer(), icons[i], NULL, &btnRect);
        
        if ((int)selectedType - 1 == i) {
            SDL_SetRenderDrawColor(window->getRenderer(), 255, 255, 0, 255);
            SDL_RenderDrawRect(window->getRenderer(), &btnRect);
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