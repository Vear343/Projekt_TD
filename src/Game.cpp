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

    SDL_Texture* enemyTex = window->loadTexture("assets/test_enemy.png");

    // Create path
    std::vector<Vector2D> path = {
        level.gridToWorld(0,0),
        level.gridToWorld(10,0),
        level.gridToWorld(10,10),
        level.gridToWorld(20,10),
        level.gridToWorld(20,20),
    };
    // Create an enemy and add it to the enemies vector
    enemies.emplace_back(200, 200, 32, 32, 300.0f, enemyTex, path);

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
    for (auto& enemy : enemies)
    enemy.update(deltatime);

    for (auto& tower : towers)      
        tower->update(deltatime, enemies);

    // Remove enemies
    enemies.erase(
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
    
    for (auto& enemy : enemies)
    window->render(enemy);

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

    IMG_Quit();
    SDL_Quit();
}