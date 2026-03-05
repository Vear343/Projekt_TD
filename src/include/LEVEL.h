#pragma once
#include <vector>
#include <SDL2/SDL.h>
#include "Vector2D.h"

class Level {
public:
    static constexpr int TILE_SIZE = 16;
    static constexpr int GRID_W = 56;
    static constexpr int GRID_H = 36;
    
    // คำนวณ Offset สำหรับจอ 800x600 ให้ด่านอยู่ตรงกลาง
    static constexpr int OFFSET_X = (1280 - (GRID_W * TILE_SIZE)) / 2; // 200
    static constexpr int OFFSET_Y = (720 - (GRID_H * TILE_SIZE)) / 2; // 148

    Level();
    
    // --- การแปลงพิกัดที่รวมค่า Offset ---
    Vector2D gridToWorld(int gx, int gy);
    bool worldToGrid(Vector2D worldPos, int& gx, int& gy);
    void render(SDL_Renderer* renderer);
    void renderEntity(SDL_Renderer* renderer, SDL_Texture* tex, Vector2D pos, float angle);
    
    private:
    int mapData[GRID_H][GRID_W];
    enum TileType{
        EMPTY = 0,
        PATH = 1,
        WALL = 2,
    };
};