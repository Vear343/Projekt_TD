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
    // SDL initialization FIRST
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

    // Create window
    window = new RenderWindow("Tower Defense", 1280, 720);
    
    if (window == nullptr) {
        std::cout << "Window creation failed\n";
        return false;
    }

    // Load textures AFTER window is created
    enemyTex = window->loadTexture("assets/test_enemy.png");
    bgTex = window->loadTexture("assets/Sky_01.png");
    fireTowerTexture = window->loadTexture("assets/fire_tower.png");
    iceTowerTexture = window->loadTexture("assets/ice_tower.png");

    // Create path
    path = {
        level.gridToWorld(0, 0),
        level.gridToWorld(10, 0),
        level.gridToWorld(10, 10),
        level.gridToWorld(20, 10),
        level.gridToWorld(20, 20),
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
            event.key.keysym.sym == SDLK_ESCAPE)
            running = false;

        // left mouse button places a tower
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
        {
            Vector2D mpos((float)event.button.x,
                        (float)event.button.y);
            int gx, gy;
            if (level.worldToGrid(mpos, gx, gy))
            {
                // only on empty ground
                if (level.isEmpty(gx, gy))
                {
                    Vector2D world = level.gridToWorld(gx, gy);
                    towers.push_back(std::make_unique<IceTower>(world, iceTowerTexture));
                    level.setTile(gx, gy, Level::TOWER);
                }
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
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
            [](const std::unique_ptr<Enemy>& e)
            {
                return !e->isAlive() || e->hasFinished();
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

    IMG_Quit();
    SDL_Quit();
}

