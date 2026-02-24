#pragma once
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "RenderWindow.h"
#include "Level.h"
#include "Enemy.h"
#include "Tower.h"


class Game
{
public:
    Game();
    ~Game();

    bool init();
    void run();      // main loop
    void clean();

private:
    void handleEvents();
    void update(float deltatime);
    void render();
    
    bool running;

    RenderWindow* window;
    Level level;

    std::vector<Enemy> enemies;
    std::vector<std::unique_ptr<Tower>> towers;
    SDL_Event event;

    Uint64 lastTime;
};