#include "Game.h"
#include <algorithm>
#include <iostream>
#include <memory>

Game::Game()
    : running(false), gold(200.0f), window(nullptr), 
      spawnTimer(0.0f), spawnDelay(1.0f), enemiesToSpawn(10),
      enemyTex(nullptr), bgTex(nullptr), menuTex(nullptr),
      fireIconTex(nullptr), iceIconTex(nullptr), windIconTex(nullptr),
      lightIconTex(nullptr), lightningIconTex(nullptr), waterIconTex(nullptr),
      fireTowerTex(nullptr), iceTowerTex(nullptr), windTowerTex(nullptr),
      lightTowerTex(nullptr), lightningTowerTex(nullptr), waterTowerTex(nullptr)
{
    // กำหนดค่าเริ่มต้นผ่าน Initializer list ด้านบนแล้ว
}

Game::~Game() {
    clean();
}

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "SDL Init Failed\n";
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cout << "IMG Init Failed\n";
        return false;
    }

    window = new RenderWindow("Tower Defense", 1280, 720);

    // Load Textures
    enemyTex = window->loadTexture("assets/test_enemy.png");
    bgTex = window->loadTexture("assets/Sky_01.png");
    menuTex = window->loadTexture("assets/Tower_Menu.png");

    // Load Icons
    fireIconTex      = window->loadTexture("assets/fire_icon.png");
    iceIconTex       = window->loadTexture("assets/ice_icon.png");
    windIconTex      = window->loadTexture("assets/wind_icon.png");
    lightIconTex     = window->loadTexture("assets/light_icon.png");
    lightningIconTex = window->loadTexture("assets/lightning_icon.png");
    waterIconTex     = window->loadTexture("assets/water_icon.png");

    // Load Towers
    fireTowerTex      = window->loadTexture("assets/fire_tower.png");
    iceTowerTex       = window->loadTexture("assets/ice_tower.png");
    windTowerTex      = window->loadTexture("assets/wind_tower.png");
    lightTowerTex     = window->loadTexture("assets/light_tower.png");
    lightningTowerTex = window->loadTexture("assets/lightning_tower.png");
    waterTowerTex     = window->loadTexture("assets/water_tower.png");

    path = {
        level.gridToWorld(0,0),
        level.gridToWorld(10,0),
        level.gridToWorld(10,10),
        level.gridToWorld(20,10),
        level.gridToWorld(20,20),
    };

    lastTime = SDL_GetPerformanceCounter();
    running = true;
    return true;
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

void Game::handleEvents() {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) running = false;
        
        if (event.type == SDL_MOUSEMOTION) {
            mousePos.x = (float)event.motion.x;
            mousePos.y = (float)event.motion.y;
        }

        if (event.type == SDL_MOUSEBUTTONDOWN) {
            Vector2D mpos((float)event.button.x, (float)event.button.y);
            int winH = window->getheight();
            const int MENU_H = 100;

            if (event.button.button == SDL_BUTTON_LEFT) {
                // UI Selection
                if (mpos.y >= winH - MENU_H) {
                    int buttonIdx = (int)mpos.x / 100;
                    switch(buttonIdx) {
                        case 0: selectedType = TowerType::Fire; break;
                        case 1: selectedType = TowerType::Ice; break;
                        case 2: selectedType = TowerType::Wind; break;
                        case 3: selectedType = TowerType::Light; break;
                        case 4: selectedType = TowerType::Lightning; break;
                        case 5: selectedType = TowerType::Water; break;
                    }
                }
                // Placement
                else if (selectedType != TowerType::None) {
                    int gx, gy;
                    if (level.worldToGrid(mpos, gx, gy) && level.isEmpty(gx, gy)) {
                        Vector2D world = level.gridToWorld(gx, gy);
                        
                        if      (selectedType == TowerType::Fire)      towers.push_back(std::make_unique<FireTower>(world, fireTowerTex));
                        else if (selectedType == TowerType::Ice)       towers.push_back(std::make_unique<IceTower>(world, iceTowerTex));
                        else if (selectedType == TowerType::Wind)      towers.push_back(std::make_unique<WindTower>(world, windTowerTex));
                        else if (selectedType == TowerType::Light)     towers.push_back(std::make_unique<LightTower>(world, lightTowerTex, gold));
                        else if (selectedType == TowerType::Lightning) towers.push_back(std::make_unique<LightningTower>(world, lightningTowerTex));
                        else if (selectedType == TowerType::Water)     towers.push_back(std::make_unique<WaterTower>(world, waterTowerTex));

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
    spawnTimer += deltaTime;
    if (spawnTimer >= spawnDelay && enemiesToSpawn > 0) {
        enemies.push_back(std::make_unique<Enemy>(path[0].x, path[0].y, enemyTex, path));
        spawnTimer = 0.0f;
        enemiesToSpawn--;
    }

    for (auto& enemy : enemies) enemy->update(deltaTime);
    for (auto& tower : towers) tower->updateTower(deltaTime, enemies);

    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
        [](const std::unique_ptr<Enemy>& e) { return e->hasFinished() || !e->isAlive(); }), 
        enemies.end());
}

void Game::render() {
    window->clear();
    if (bgTex) window->drawTextureFull(bgTex);
    level.render(window->getRenderer());

    for (auto& enemy : enemies) window->render(*enemy);
    for (auto& tower : towers) tower->render(window->getRenderer());

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
    window->display();
}

void Game::clean() {
    // ฟังก์ชันช่วยทำลาย Texture
    auto destroy = [](SDL_Texture*& tex) { if(tex) { SDL_DestroyTexture(tex); tex = nullptr; } };
    
    destroy(enemyTex); destroy(bgTex); destroy(menuTex);
    destroy(fireIconTex); destroy(iceIconTex); destroy(windIconTex);
    destroy(lightIconTex); destroy(lightningIconTex); destroy(waterIconTex);
    destroy(fireTowerTex); destroy(iceTowerTex); destroy(windTowerTex);
    destroy(lightTowerTex); destroy(lightningTowerTex); destroy(waterTowerTex);

    if (window) { window->cleanUp(); delete window; window = nullptr; }
    IMG_Quit();
    SDL_Quit();
}