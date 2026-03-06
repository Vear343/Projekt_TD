#include <algorithm>
#include <iostream>
#include <memory>
#include "Game.h"

// Constructor
// Initialize default values for the Game object
Game::Game()
{
    running = false;
    window = nullptr;
    enemyTex = nullptr;
    bgTex = nullptr;
    menuTex = nullptr;
    fireIconTex = nullptr;
    iceIconTex = nullptr;
    fireTowerTex = nullptr;
    iceTowerTex = nullptr;
}

// Destructor
// Automatically called when Game object is destroyed
Game::~Game()
{
    clean();
}

// Initialize everything needed for the game
bool Game::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << "SDL Init Failed\n";
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        std::cout << "IMG Init Failed\n";
        return false;
    }

    window = new RenderWindow("Tower Defense", 1280, 720);

    enemyTex = window->loadTexture("assets/test_enemy.png");
    // try loading a sky background (optional)
    bgTex = window->loadTexture("assets/Sky_01.png");

    // load UI/menu assets
    menuTex = window->loadTexture("assets/Tower_Menu.png");
    fireIconTex = window->loadTexture("assets/fire_icon.png");
    iceIconTex = window->loadTexture("assets/ice_icon.png");
    // tower graphics (can be same as icon or different images)
    fireTowerTex = window->loadTexture("assets/fire_tower.png");
    iceTowerTex  = window->loadTexture("assets/ice_tower.png");
    // create a default path
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

# pragma region main loop
// Main game loop
void Game::run()
{
    while (running)
    {
        Uint64 currentTime = SDL_GetPerformanceCounter();
        float deltaTime = (float)(currentTime - lastTime) / SDL_GetPerformanceFrequency();
        lastTime = currentTime;

        handleEvents();
        update(deltaTime);
        render();
    }
}

// Handle user input
void Game::handleEvents()
{
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            running = false;
        
        if (event.type == SDL_KEYDOWN &&
            event.key.keysym.sym == SDLK_x)
            running = false;


        if (event.type == SDL_MOUSEMOTION)
        {
            mousePos.x = (float)event.motion.x;
            mousePos.y = (float)event.motion.y;
        }

        if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            Vector2D mpos((float)event.button.x,
                          (float)event.button.y);
            int winH = window->getheight();
            const int MENU_H = 100;
            SDL_Rect fireBtn = { 0, winH - MENU_H, 100, MENU_H };
            SDL_Rect iceBtn  = { 100, winH - MENU_H, 100, MENU_H };

            if (event.button.button == SDL_BUTTON_LEFT)
            {
                // check UI buttons first
                if (mpos.y >= winH - MENU_H)
                {
                    if (mpos.x >= fireBtn.x && mpos.x < fireBtn.x + fireBtn.w)
                        selectedType = TowerType::Fire;
                    else if (mpos.x >= iceBtn.x && mpos.x < iceBtn.x + iceBtn.w)
                        selectedType = TowerType::Ice;
                    continue;
                }

                // placing tower on grid
                if (selectedType != TowerType::None)
                {
                    int gx, gy;
                    if (level.worldToGrid(mpos, gx, gy) && level.isEmpty(gx, gy))
                    {
                        Vector2D world = level.gridToWorld(gx, gy);
                        if (selectedType == TowerType::Fire)
                            towers.push_back(
                                std::make_unique<FireTower>(world, fireTowerTex));   // ← pass texture
                        else if (selectedType == TowerType::Ice)
                            towers.push_back(
                                std::make_unique<IceTower>(world, iceTowerTex));    // ← pass texture
                        level.setTile(gx, gy, Level::TOWER);
                        selectedType = TowerType::None;
                    }
                }
            }
            else if (event.button.button == SDL_BUTTON_RIGHT)
            {
                // cancel selection
                selectedType = TowerType::None;
            }
        }
    }
}
    
// Update all game objects
void Game::update(float deltaTime)
{
    spawnTimer += deltaTime;

    // Update enemies
    for (auto& enemy : enemies)
        enemy->update(deltaTime);   // <-- pointer now

    // Spawn enemies
    if (spawnTimer >= spawnDelay && enemiesToSpawn > 0)
    {
        enemies.push_back(std::make_unique<Enemy>(
            path[0].x,
            path[0].y,
            enemyTex,
            path
        ));

        spawnTimer = 0.0f;
        enemiesToSpawn--;
    }

    // Update towers
    for (auto& tower : towers) tower->updateTower(deltaTime, enemies);

    // Remove finished enemies
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const std::unique_ptr<Enemy>& e)
            {
                return e->hasFinished();
            }),
        enemies.end()
    );
}
// Render everything to the screen
void Game::render()
{
    window->clear();
    
    // draw sky image if we have one
    if (bgTex) {
        window->drawTextureFull(bgTex);
    }

    level.render(window->getRenderer());
    
    // Render all enemies
    for (auto& enemy : enemies)
        window->render(*enemy);

    // draw towers
    for (auto& tower : towers)
        tower->render(window->getRenderer());

    // draw tower menu bar
    int winW = window->getwidth();
    int winH = window->getheight();
    const int MENU_H = 100;
    SDL_Rect menuRect = { 0, winH - MENU_H, winW, MENU_H };
    if (menuTex)
        SDL_RenderCopy(window->getRenderer(), menuTex, NULL, &menuRect);
    else {
        SDL_SetRenderDrawColor(window->getRenderer(), 50, 50, 50, 200);
        SDL_RenderFillRect(window->getRenderer(), &menuRect);
    }

    // icons
    SDL_Rect fireBtn = { 0, winH - MENU_H, 100, MENU_H };
    SDL_Rect iceBtn  = { 100, winH - MENU_H, 100, MENU_H };
    if (fireIconTex)
        SDL_RenderCopy(window->getRenderer(), fireIconTex, NULL, &fireBtn);
    else {
        SDL_SetRenderDrawColor(window->getRenderer(), 200, 0, 0, 255);
        SDL_RenderFillRect(window->getRenderer(), &fireBtn);
    }
    if (iceIconTex)
        SDL_RenderCopy(window->getRenderer(), iceIconTex, NULL, &iceBtn);
    else {
        SDL_SetRenderDrawColor(window->getRenderer(), 0, 150, 255, 255);
        SDL_RenderFillRect(window->getRenderer(), &iceBtn);
    }

    // ghost preview
    if (selectedType != TowerType::None)
    {
        int gx, gy;
        if (level.worldToGrid(mousePos, gx, gy))
        {
            Vector2D world = level.gridToWorld(gx, gy);
            SDL_Rect rect = { (int)world.x - 10, (int)world.y - 10, 20, 20 };
            SDL_SetRenderDrawBlendMode(window->getRenderer(), SDL_BLENDMODE_BLEND);
            if (selectedType == TowerType::Fire)
                SDL_SetRenderDrawColor(window->getRenderer(), 255, 0, 0, 128);
            else
                SDL_SetRenderDrawColor(window->getRenderer(), 0, 150, 255, 128);
            SDL_RenderFillRect(window->getRenderer(), &rect);
            SDL_SetRenderDrawBlendMode(window->getRenderer(), SDL_BLENDMODE_NONE);
        }
    }

    window->display();
}
# pragma endregion

void Game::clean()
{
    if (window)
    {
        window->cleanUp();
        delete window;
        window = nullptr;
    }

    if (enemyTex) {
        SDL_DestroyTexture(enemyTex);
        enemyTex = nullptr;
    }
    if (bgTex) {
        SDL_DestroyTexture(bgTex);
        bgTex = nullptr;
    }
    if (menuTex) {
        SDL_DestroyTexture(menuTex);
        menuTex = nullptr;
    }
    if (fireIconTex) {
        SDL_DestroyTexture(fireIconTex);
        fireIconTex = nullptr;
    }
    if (iceIconTex) {
        SDL_DestroyTexture(iceIconTex);
        iceIconTex = nullptr;
    }
    if (fireTowerTex) {               // destroy the two new textures too
    SDL_DestroyTexture(fireTowerTex);
    fireTowerTex = nullptr;
    }
    if (iceTowerTex) {
    SDL_DestroyTexture(iceTowerTex);
    iceTowerTex = nullptr;
    }

    IMG_Quit();
    SDL_Quit();
}

