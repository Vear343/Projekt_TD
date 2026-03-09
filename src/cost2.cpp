#include <iostream>
#include <vector>
#include <cmath>
#include <SDL2/SDL.h>

#define MAP_WIDTH 10
#define MAP_HEIGHT 10

const int tileWidth = 64;
const int tileHeight = 32;

// ราคาป้อม
const int TOWER_COST = 50;
const int TOWER_SELL = 25;

//////////////////////////////////////////////////////////////

struct Vector2 {
    float x, y;
};

//////////////////////////////////////////////////////////////

enum TileType {
    EMPTY,
    PATH,
    OCCUPIED
};

struct Tile {
    TileType type = EMPTY;
    bool buildable = true;
};

Tile mapGrid[MAP_WIDTH][MAP_HEIGHT];

//////////////////////////////////////////////////////////////

class Tower {
private:
    int gridX, gridY;
    Vector2 worldPos;
    float rotation;

public:

    Tower(int gx, int gy) {
        setGridPosition(gx, gy);
        rotation = 0;
    }

    void setGridPosition(int gx, int gy) {

        gridX = gx;
        gridY = gy;

        worldPos.x = (gx - gy) * tileWidth / 2.0f;
        worldPos.y = (gx + gy) * tileHeight / 2.0f;
    }

    void rotateTo(Vector2 target) {

        float dx = target.x - worldPos.x;
        float dy = target.y - worldPos.y;

        rotation = atan2(dy, dx) * 180.0f / 3.14159f;
    }

    bool isClicked(Vector2 mousePos) {

        float dx = mousePos.x - worldPos.x;
        float dy = mousePos.y - worldPos.y;

        float dist = sqrt(dx * dx + dy * dy);

        return dist < 40;
    }

    int getGridX() { return gridX; }
    int getGridY() { return gridY; }

    void render() {

        std::cout << "Tower at (" << gridX << "," << gridY
                  << ") Rot: " << rotation << "\n";
    }
};

//////////////////////////////////////////////////////////////

std::vector<Tower*> towers;

int playerMoney = 200;

bool buyingTower = false;

Uint32 lastClickTime = 0;

//////////////////////////////////////////////////////////////

Vector2 screenToGrid(float screenX, float screenY) {

    float gx = (screenY / tileHeight + screenX / tileWidth);
    float gy = (screenY / tileHeight - screenX / tileWidth);

    return { floor(gx), floor(gy) };
}

//////////////////////////////////////////////////////////////

bool canPlace(int x, int y) {

    if (x < 0 || y < 0 || x >= MAP_WIDTH || y >= MAP_HEIGHT)
        return false;

    if (mapGrid[x][y].type != EMPTY)
        return false;

    if (!mapGrid[x][y].buildable)
        return false;

    return true;
}

//////////////////////////////////////////////////////////////
// BUY TOWER

void buyTower() {

    if (playerMoney < TOWER_COST) {
        std::cout << "Not enough money\n";
        return;
    }

    buyingTower = true;

    std::cout << "Click map to place tower\n";
}

//////////////////////////////////////////////////////////////
// PLACE TOWER

void placeTower(int x, int y) {

    if (!canPlace(x, y)) {
        std::cout << "Cannot place here\n";
        return;
    }

    if (playerMoney < TOWER_COST) {
        std::cout << "Not enough money\n";
        return;
    }

    Tower* tower = new Tower(x, y);

    towers.push_back(tower);

    mapGrid[x][y].type = OCCUPIED;

    playerMoney -= TOWER_COST;

    buyingTower = false;

    std::cout << "Tower placed at " << x << "," << y
              << " Money: " << playerMoney << "\n";
}

//////////////////////////////////////////////////////////////
// SELL TOWER

void sellTower(int index) {

    Tower* tower = towers[index];

    int x = tower->getGridX();
    int y = tower->getGridY();

    mapGrid[x][y].type = EMPTY;

    playerMoney += TOWER_SELL;

    delete tower;

    towers.erase(towers.begin() + index);

    std::cout << "Tower sold! Money: " << playerMoney << "\n";
}

//////////////////////////////////////////////////////////////

int main(int argc, char* argv[]) {

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "Tower Defense",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600,
        0
    );

    SDL_Event event;

    bool running = true;

    while (running) {

        while (SDL_PollEvent(&event)) {

            if (event.type == SDL_QUIT)
                running = false;

            // กด B เพื่อซื้อป้อม
            if (event.type == SDL_KEYDOWN) {

                if (event.key.keysym.sym == SDLK_b)
                    buyTower();
            }

            // คลิกเมาส์
            if (event.type == SDL_MOUSEBUTTONDOWN) {

                float mouseX = event.button.x;
                float mouseY = event.button.y;

                Vector2 mousePos = { mouseX, mouseY };

                Uint32 currentTime = SDL_GetTicks();

                // ตรวจขายป้อม (double click)
                for (int i = 0; i < towers.size(); i++) {

                    if (towers[i]->isClicked(mousePos)) {

                        if (currentTime - lastClickTime < 300) {

                            sellTower(i);
                            break;
                        }

                        lastClickTime = currentTime;
                    }
                }

                // วางป้อม
                if (buyingTower) {

                    Vector2 grid = screenToGrid(mouseX, mouseY);

                    int gx = (int)grid.x;
                    int gy = (int)grid.y;

                    placeTower(gx, gy);
                }
            }
        }

        for (auto tower : towers)
            tower->render();
    }

    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}