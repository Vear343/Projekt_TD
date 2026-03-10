#include "Game.h"
#include <algorithm>
#include <iostream>

Game::Game()
    : running(false), gold(500.0f), playerHealth(20), window(nullptr),
      currentWave(0), enemiesToSpawnInWave(0), spawnTimer(0.0f),
      spawnDelay(1.5f), waveActive(false),
      enemyTex(nullptr), bgTex(nullptr), menuTex(nullptr),
      currentState(GameState::Menu)
{
    // Initializer list จัดการค่าเริ่มต้นแล้ว
}

Game::~Game() {
    clean();
}

int Game::findTowerAt(Vector2D worldPos) const {
    for (int i = static_cast<int>(towers.size()) - 1; i >= 0; --i) {
        const auto& tower = towers[i];
        SDL_Rect rect;
        rect.w = static_cast<int>(tower->getWidth());
        rect.h = static_cast<int>(tower->getHeight());
        rect.x = static_cast<int>(tower->getX() - tower->getWidth() / 2.0f);
        rect.y = static_cast<int>(tower->getY() - tower->getHeight() / 2.0f);

        if (worldPos.x >= rect.x && worldPos.x <= rect.x + rect.w &&
            worldPos.y >= rect.y && worldPos.y <= rect.y + rect.h) {
            return i;
        }
    }
    return -1;
}

void Game::sellTowerAtIndex(int index) {
    if (index < 0 || index >= static_cast<int>(towers.size())) return;

    int gx, gy;
    if (level.worldToGrid(Vector2D(towers[index]->getX(), towers[index]->getY()), gx, gy)) {
        level.setTile(gx, gy, Level::EMPTY);
    }

    int sellPrice = towers[index]->getCost() * 0.6f;
    gold += sellPrice;

    std::cout << "Tower sold! Refund: " << sellPrice 
              << " | Gold: " << gold << std::endl;

    towers.erase(towers.begin() + index);
}

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) return false;
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) return false;

    window = new RenderWindow("Tower Defense | Press 'N' for Next Wave", 1280, 720);

    // โหลด Texture ทั้งหมด
    enemyTex = window->loadTexture("assets/Enemy.png");
    bossTex = window->loadTexture("assets/Boss.png");
    bgTex = window->loadTexture("assets/Sky_01.png");

    // Load Towers
    fireTowerTexture      = window->loadTexture("assets/Fire_Tower.png");
    iceTowerTexture       = window->loadTexture("assets/Ice_Tower.png");
    windTowerTexture      = window->loadTexture("assets/Wind_Tower.png");
    lightTowerTexture     = window->loadTexture("assets/Light_Tower.png");
    lightningTowerTexture = window->loadTexture("assets/Lightning_Tower.png");
    waterTowerTexture     = window->loadTexture("assets/Water_Tower.png");
    
    // Load projectile textures
    fireProjectileTexture = window->loadTexture("assets/Fire_Ball.png");
    iceProjectileTexture = window->loadTexture("assets/Ice_Ball.png");
    windProjectileTexture = window->loadTexture("assets/Wind_Ball.png");
    lightProjectileTexture = window->loadTexture("assets/Light_Ball.png");
    lightningProjectileTexture = window->loadTexture("assets/Lightning_Ball.png");
    waterProjectileTexture = window->loadTexture("assets/Water_Ball.png");
    
    // Icons
    fireIconTex = window->loadTexture("assets/Fire_Tower.png");
    iceIconTex = window->loadTexture("assets/Ice_Tower.png");
    windIconTex = window->loadTexture("assets/Wind_Tower.png");
    lightIconTex = window->loadTexture("assets/Light_Tower.png");
    lightningIconTex = window->loadTexture("assets/Lightning_Tower.png");
    waterIconTex = window->loadTexture("assets/Water_Tower.png");

    path = {
        level.gridToWorld(0, 9),
        level.gridToWorld(5, 9),
        level.gridToWorld(5, 3),
        level.gridToWorld(10, 3),
        level.gridToWorld(10, 12),
        level.gridToWorld(18, 12),
        level.gridToWorld(18, 4),
        level.gridToWorld(27, 4)
    };

    lastTime = SDL_GetPerformanceCounter();
    running = true;
    return true;
}

void Game::startNextWave() {
    // ระบบ Skip Bonus: ถ้า Wave เก่ายังไม่หมด ได้เงินเพิ่ม 100
    if (waveActive && !enemies.empty()) {
        gold += 100.0f;
        if (spawnDelay > 0.5f) {
            spawnDelay -= 0.1f;
        }
    }

    currentWave++;
    waveActive = true;
    enemiesToSpawnInWave = 10 + currentWave * 5; // เพิ่มจำนวนมอนสเตอร์ในแต่ละ Wave
    spawnTimer = 0.0f;
    std::cout << "Wave " << currentWave << " Started!" << std::endl;
}

void Game::resetGame() {
    gold = 500.0f;
    playerHealth = 20;
    currentWave = 0;
    enemiesToSpawnInWave = 0;
    spawnTimer = 0.0f;
    waveActive = false;
    enemies.clear();
    towers.clear();
    level.reset();  // Reset the level grid
    selectedType = TowerType::None;
    // Note: Path is set in init() and doesn't need resetting unless modified dynamically.
}

void Game::handleEvents() {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) running = false;

        if (currentState == GameState::Menu) {
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                Vector2D mpos((float)event.button.x, (float)event.button.y);
                int winW = window->getwidth();
                int winH = window->getheight();

                SDL_Rect playButton = { (winW - 200) / 2, (winH - 100) / 2 + 100, 200, 100 }; 
                if (mpos.x >= playButton.x && mpos.x <= playButton.x + playButton.w &&
                    mpos.y >= playButton.y && mpos.y <= playButton.y + playButton.h) {
                    resetGame();  // Reset all game state instead of just clearing enemies/towers
                    currentState = GameState::Playing;
                }

                SDL_Rect exitButton = { (winW - 200) / 2, (winH - 100) / 2 + 250, 200, 100 };
                if (mpos.x >= exitButton.x && mpos.x <= exitButton.x + exitButton.w &&
                    mpos.y >= exitButton.y && mpos.y <= exitButton.y + exitButton.h) {
                    running = false;
                }
            }
            continue;
        }

        if (currentState == GameState::Pause) {

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                currentState = GameState::Playing;
                continue;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                Vector2D mpos((float)event.button.x, (float)event.button.y);
                int winW = window->getwidth();
                int winH = window->getheight();
                
                SDL_Rect resumeButton = { (winW - 200) / 2, (winH - 100) / 2 + 100, 200, 100 };
                if (mpos.x >= resumeButton.x && mpos.x <= resumeButton.x + resumeButton.w &&
                    mpos.y >= resumeButton.y && mpos.y <= resumeButton.y + resumeButton.h) {
                    currentState = GameState::Playing;
                }

                SDL_Rect exittomainButton = { (winW - 200) / 2, (winH - 100) / 2 + 250, 200, 100 };
                if (mpos.x >= exittomainButton.x && mpos.x <= exittomainButton.x + exittomainButton.w &&
                    mpos.y >= exittomainButton.y && mpos.y <= exittomainButton.y + exittomainButton.h) {
                    currentState = GameState::Menu;
                }
            }
            continue;
        }

        if (currentState == GameState::Playing) {
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    if (currentState == GameState::Playing) {
                        currentState = GameState::Pause;
                        continue;
                    }
                }
                if (currentState != GameState::Playing) continue;
    
                if (event.key.keysym.sym == SDLK_n && isWaveFinished()) { startNextWave(); }
                else if (event.key.keysym.sym == SDLK_c) {
                    bool current = window->isDrawColliders();
                    window->setDrawColliders(!current);
                }
            }
    
            if (currentState != GameState::Playing) continue;  
    
            if (event.type == SDL_MOUSEMOTION) {
                mousePos.x = (float)event.motion.x;
                mousePos.y = (float)event.motion.y;
            }
    
            if (event.type != SDL_MOUSEBUTTONDOWN) continue;
    
            Vector2D mpos((float)event.button.x, (float)event.button.y);
            int winH = window->getheight();
    
            if (event.button.button == SDL_BUTTON_RIGHT) {
                selectedType = TowerType::None;
                continue;
            }
    
            if (event.button.button != SDL_BUTTON_LEFT) continue;
    
            if (event.button.clicks >= 2 && mpos.y < winH - 100) {
                int towerIndex = findTowerAt(mpos);
                if (towerIndex != -1) {
                    sellTowerAtIndex(towerIndex);
                    selectedType = TowerType::None;
                    continue;
                }
            }
    
            if (mpos.y >= winH - 100) {
                int idx = (int)mpos.x / 100;
                if (idx == 0) selectedType = TowerType::Fire;
                else if (idx == 1) selectedType = TowerType::Ice;
                else if (idx == 2) selectedType = TowerType::Wind;
                else if (idx == 3) selectedType = TowerType::Light;
                else if (idx == 4) selectedType = TowerType::Lightning;
                else if (idx == 5) selectedType = TowerType::Water;
                continue;
            }
    
            if (selectedType == TowerType::None) continue;
    
            int gx, gy;
            if (!level.worldToGrid(mpos, gx, gy) || !level.isEmpty(gx, gy)) continue;
    
            Vector2D world = level.gridToWorld(gx, gy);
            bool placed = false;
            if      (selectedType == TowerType::Fire && gold >= FireTower::cost) {
                towers.push_back(std::make_unique<FireTower>(world, fireTowerTexture, fireProjectileTexture));
                gold -= FireTower::cost;
                placed = true;
            }
            else if (selectedType == TowerType::Ice && gold >= IceTower::cost) {
                towers.push_back(std::make_unique<IceTower>(world, iceTowerTexture, iceProjectileTexture));
                gold -= IceTower::cost;
                placed = true;
            }
            else if (selectedType == TowerType::Wind && gold >= WindTower::cost) {
                towers.push_back(std::make_unique<WindTower>(world, windTowerTexture, windProjectileTexture));
                gold -= WindTower::cost;
                placed = true;
            }
            else if (selectedType == TowerType::Light && gold >= LightTower::cost) {
                towers.push_back(std::make_unique<LightTower>(world, lightTowerTexture, gold));
                gold -= LightTower::cost;
                placed = true;
            }
            else if (selectedType == TowerType::Lightning && gold >= LightningTower::cost) {
                towers.push_back(std::make_unique<LightningTower>(world, lightningTowerTexture, lightningProjectileTexture));
                gold -= LightningTower::cost;
                placed = true;
            }
            else if (selectedType == TowerType::Water && gold >= WaterTower::cost) {
                towers.push_back(std::make_unique<WaterTower>(world, waterTowerTexture, waterProjectileTexture));
                gold -= WaterTower::cost;
                placed = true;
            }
            else {
                std::cout << "Not enough gold to place " << (int)selectedType << " Tower. Current Gold: " << gold << std::endl;
            }
    
            if (placed) {
                std::cout << "Placed " << (int)selectedType << " Tower, Remaining Gold: " << gold << std::endl;
                level.setTile(gx, gy, Level::TOWER);
                selectedType = TowerType::None;
            }
        }
    }
}
void Game::update(float deltaTime) {
    if (currentState != GameState::Playing) return;

    // 1. ระบบ Spawn มอนสเตอร์
    if (waveActive && enemiesToSpawnInWave > 0) {
        spawnTimer += deltaTime;
        if (spawnTimer >= spawnDelay) {
            // ถ้าเป็นตัวสุดท้ายของ Wave (ตัวที่ 1) ให้เป็นบอส
            if (enemiesToSpawnInWave == 1) {
                enemies.push_back(
                    std::make_unique<BossEnemy>(path[0].x, path[0].y, bossTex, path)
                );
            }
            else {
                enemies.push_back(
                    std::make_unique<Enemy>(path[0].x, path[0].y, enemyTex, path)
                );
            }
            
            enemiesToSpawnInWave--;
            spawnTimer = 0.0f;
        }
    }

    // 2. อัปเดตมอนสเตอร์
    for (auto& enemy : enemies) {
        enemy->update(deltaTime);
    }

    // Update towers (projectiles may kill enemies)
    for (auto& tower : towers) tower->updateTower(deltaTime, enemies, waveActive);

    // 3. เช็คการตายและให้รางวัล (AFTER towers attack, so projectile kills are detected)
    for (auto& enemy : enemies) {
        // ถ้ามอนตาย (isAlive เป็น false) และยังไม่ได้ให้รางวัล
        if (!enemy->isAlive() && !enemy->isRewardGiven()) {
            float reward = enemy->getReward();
            gold += enemy->getReward(); // สังหารได้เงินตามรางวัลของมอนสเตอร์
            std::cout << "Enemy killed! " << reward <<" gold, Current: " << gold << std::endl;
            enemy->setRewardGiven(true);
        }
    }

    // เช็คมอนสเตอร์ที่เข้าเส้นชัยแล้วลด HP ของผู้เล่น
    for (auto& enemy : enemies) {
        if (enemy->hasFinished() && !enemy->isRewardGiven()) {
            // Check if it's a boss to apply different damage
            if (dynamic_cast<BossEnemy*>(enemy.get()) != nullptr) {
                playerHealth -= 3;  // Boss deals 3 damage
                std::cout << "Boss reached core! Player HP: " << playerHealth << std::endl;
            } else {
                playerHealth -= 1;  // Regular enemy deals 1 damage
                std::cout << "Enemy reached core! Player HP: " << playerHealth << std::endl;
            }

            if (playerHealth <= 0) {
                std::cout << "Game Over! Final Wave: " << currentWave << std::endl;
                currentState = GameState::GameOver;
            }

            // ตั้ง setRewardGiven เป็น true เพื่อไม่ให้ลด HP ซ้ำ
            enemy->setRewardGiven(true);
        }
    }

    if (currentState != GameState::Playing) return;

    if (waveActive && enemiesToSpawnInWave == 0 && enemies.empty()) {
    waveActive = false;
    std::cout << "Wave Cleared!\n";
    }

    // 4. แสดงผล Gold บน Window Title แบบ Real-time
    std::string title = "Tower Defense | Gold: " + std::to_string((int)gold) + 
                        " | Wave: " + std::to_string(currentWave) + 
                        " | Enemies: " + std::to_string(enemies.size()) + 
                        " | Health: " + std::to_string(playerHealth);
    SDL_SetWindowTitle(window->getSDLWindow(), title.c_str());

    // ลบมอนสเตอร์ที่ตายหรือเข้าเส้นชัย
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
        [](const std::unique_ptr<Enemy>& e) { return !e->isAlive() || e->hasFinished(); }), 
        enemies.end());
}

void Game::render() {
    window->clear();

    if (currentState == GameState::Menu) {
        // Render menu background (using bgTex or a separate one)
        if (mainmenuTex) window->drawTextureFull(mainmenuTex);

        // Render play button
        int winW = window->getwidth();
        int winH = window->getheight();
        SDL_Rect playButton = { (winW - 200) / 2, (winH - 100) / 2 + 100, 200, 100 }; 
        SDL_RenderCopy(window->getRenderer(), playButtonTexture, NULL, &playButton);
        SDL_Rect exitButton = { (winW - 200) / 2, (winH - 100) / 2 + 250, 200, 100 };
        SDL_RenderCopy(window->getRenderer(), exitButtonTexture, NULL, &exitButton);
        // Optionally render text, but for simplicity, just the button
    } else if (currentState == GameState::Playing) {
        // Render background
        if (bgTex) window->drawTextureFull(bgTex);

        // Render level
        level.render(window->getRenderer());

    // Render enemies and towers
    for (auto& enemy : enemies){
        enemy->renderHpbar(window->getRenderer());
        window->render(*enemy);
    }
    for (auto& tower : towers) window->render(*tower);

        
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
    } else if (currentState == GameState::Pause) {
        if (bgTex) window->drawTextureFull(bgTex);
    }

    window->present();
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

