#pragma once
#include <vector>
#include <SDL2/SDL.h>
#include "Vector2D.h"

class Level {
public:
    static constexpr int TILE_SIZE = 32;
    static constexpr int GRID_W = 26;
    static constexpr int GRID_H = 14;
    
    // คำนวณ Offset สำหรับจอ 800x600 ให้ด่านอยู่ตรงกลาง
    static constexpr int OFFSET_X = (1280 - (GRID_W * TILE_SIZE)) / 2; // 200
    static constexpr int OFFSET_Y = (720 - (GRID_H * TILE_SIZE)) / 2; // 148

    Level();
    
    // --- การแปลงพิกัดที่รวมค่า Offset ---
    Vector2D gridToWorld(int gx, int gy);
    bool worldToGrid(Vector2D worldPos, int& gx, int& gy);
    void render(SDL_Renderer* renderer);
    void renderEntity(SDL_Renderer* renderer, SDL_Texture* tex, Vector2D pos, float angle);

    // tile types that the level stores; public so game logic can inspect/modify
    enum TileType{
        EMPTY = 0,
        PATH = 1,
        WALL = 2,
        TOWER = 3,      // occupied by a placed tower
        CORE = 4       // the end point enemies are trying to reach
    };

    // helpers for game logic
    bool isEmpty(int gx,int gy) const;
    void setTile(int gx,int gy, TileType type);

private:
    int mapData[GRID_H][GRID_W];
};