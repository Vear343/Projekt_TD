#include <algorithm>
#include <iostream>

#include "Game.h"

// Constructor
// Initialize default values for the Game object
Game::Game()
{
    running = false;
    window = nullptr;
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

    // Create path
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
            event.key.keysym.sym == SDLK_ESCAPE)
            running = false;
        }
    }
    
// Update all game objects
void Game::update(float deltatime)
{
    spawnTimer += deltatime;

    for (auto& enemy : enemies)
    enemy.update(deltatime);

    if (spawnTimer >= spawnDelay && enemiesToSpawn > 0){
        enemies.emplace_back(
            path[0].x,
            path[0].y,
            enemyTex,
            path
        );

        spawnTimer = 0.0f;
        enemiesToSpawn--;
    }

    // Remove enemies
    enemies.erase(
        // ใช้ std::remove_if เพื่อกรองเอาเฉพาะ Enemy ที่ยังไม่จบเส้นทางออกมา
        std::remove_if(enemies.begin(), enemies.end(),
            [](Enemy& e){
                return e.hasFinished();
            }),
        enemies.end()
    );
}

// Render everything to the screen
void Game::render()
{
    window->clear();
    
    level.render(window->getRenderer());
    
    // Render all enemies
    for (auto& enemy : enemies)
    window->render(enemy);
    
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

    IMG_Quit();
    SDL_Quit();
}