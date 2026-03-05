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
    void handleEvents();
    void update(float deltatime);
    void render();
    void clean();
    
private:
    bool running;

    RenderWindow* window;
    Level level;

    std::vector<std::unique_ptr<Enemy>> enemies;

    // ตัวแปรสำหรับควบคุมการ spawn ศัตรู
    float spawnTimer = 0.0f;
    float spawnDelay = 1.0f;
    int  enemiesToSpawn = 10; // จำนวนศัตรูที่จะ spawn

    SDL_Texture* enemyTex;
    SDL_Texture* bgTex;             // sky/background image

    // tower menu/UI textures
    SDL_Texture* menuTex;
    SDL_Texture* fireIconTex;
    SDL_Texture* iceIconTex;
    SDL_Texture *fireTowerTex;
    SDL_Texture *iceTowerTex;
    std::vector<Vector2D> path;

    std::vector<std::unique_ptr<Tower>> towers;
    SDL_Event event;

    Uint64 lastTime;

    // interactive placement state
    enum class TowerType { None, Fire, Ice };
    TowerType selectedType = TowerType::None;
    Vector2D mousePos;              // current mouse coordinates
};