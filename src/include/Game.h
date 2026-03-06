#pragma once
#include <vector>
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "RenderWindow.h"
#include "Level.h"
#include "Enemy.h"
#include "Tower.h"

class Game {
public:
    Game();
    ~Game();

    bool init();
    void run();
    void handleEvents();
    void update(float deltaTime);
    void render();
    void clean();
    
private:
    bool running;
    float gold;
    RenderWindow* window;
    Level level;

    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Tower>> towers;
    std::vector<Vector2D> path;

    // ตัวแปรสำหรับควบคุมการ spawn ศัตรู
    float spawnTimer;
    float spawnDelay;
    int enemiesToSpawn;

    // Textures
    SDL_Texture* enemyTex;
    SDL_Texture* bgTex;
    SDL_Texture* menuTex;

    // UI Icons
    SDL_Texture* fireIconTex;
    SDL_Texture* iceIconTex;
    SDL_Texture* windIconTex;
    SDL_Texture* lightIconTex;
    SDL_Texture* lightningIconTex;
    SDL_Texture* waterIconTex;

    // Tower Textures
    SDL_Texture* fireTowerTex;
    SDL_Texture* iceTowerTex;
    SDL_Texture* windTowerTex;
    SDL_Texture* lightTowerTex;
    SDL_Texture* lightningTowerTex;
    SDL_Texture* waterTowerTex;

    SDL_Event event;
    Uint64 lastTime;

    enum class TowerType { None, Fire, Ice, Wind, Light, Lightning, Water };
    TowerType selectedType = TowerType::None;
    Vector2D mousePos;
};