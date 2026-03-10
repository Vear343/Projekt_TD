#pragma once
#include <vector>
#include <memory>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "RenderWindow.h"
#include "Level.h"
#include "Enemy.h"
#include "Tower.h"
#include "Projectile.h"

class Game {
public:
    Game();
    ~Game();

    bool init();
    void run();
    void handleEvents();
    void update(float deltaTime);
    void render();
    void renderPlayingState();
    void clean();
    int findTowerAt(Vector2D worldPos) const;
    void sellTowerAtIndex(int index);
    
    // ฟังก์ชันจัดการ Wave
    void startNextWave();
    bool isWaveFinished() const { return enemiesToSpawnInWave == 0 && enemies.empty(); }
    void resetGame();  // New method to reset all game state

private:
    bool running;
    float gold;
    int playerHealth;
    RenderWindow* window;
    Level level;
    
    // ระบบ Wave
    int currentWave = 0;
    int enemiesToSpawnInWave;
    float spawnTimer;
    float spawnDelay;
    bool waveActive;

    // Game Objects
    std::vector<Vector2D> path;
    std::vector<std::unique_ptr<Tower>> towers;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Projectile>> projectiles;

    // Textures
    SDL_Texture* menuTex;
    SDL_Texture* playButtonTexture;
    SDL_Texture* exitButtonTexture;
    SDL_Texture* resumeButtonTexture;
    SDL_Texture* exittomainButtonTexture;
    SDL_Texture* enemyTex;
    SDL_Texture* bossTex;
    SDL_Texture* mainmenuTex;
    SDL_Texture* bgTex;
    SDL_Texture* fireTowerTexture = nullptr;
    SDL_Texture* iceTowerTexture = nullptr;
    SDL_Texture* windTowerTexture = nullptr;
    SDL_Texture* lightTowerTexture = nullptr;
    SDL_Texture* lightningTowerTexture = nullptr;
    SDL_Texture* waterTowerTexture = nullptr;
    SDL_Texture* fireProjectileTexture = nullptr;
    SDL_Texture* iceProjectileTexture = nullptr;
    SDL_Texture* windProjectileTexture = nullptr;
    SDL_Texture* lightProjectileTexture = nullptr;
    SDL_Texture* lightningProjectileTexture = nullptr;
    SDL_Texture* waterProjectileTexture = nullptr;

    // UI Icons
    SDL_Texture* fireIconTex;
    SDL_Texture* iceIconTex;
    SDL_Texture* windIconTex;
    SDL_Texture* lightIconTex;
    SDL_Texture* lightningIconTex;
    SDL_Texture* waterIconTex;

    bool setDrawColliders = false; // สำหรับเปิด/ปิดการวาด collider ใน RenderWindow
    

    SDL_Event event;
    Uint64 lastTime;

    enum class TowerType { None, Fire, Ice, Wind, Light, Lightning, Water };
    TowerType selectedType = TowerType::None;
    Vector2D mousePos;

    enum class GameState { Menu, Playing, Pause, GameOver };
    GameState currentState;
};
