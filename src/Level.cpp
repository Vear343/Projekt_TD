#include<SDL2/SDL.h>
#include <iostream>

#include "LEVEL.h"

Level::Level() {
        // กำหนดค่าเริ่มต้นให้ Grid (0 = ว่าง, 1 = ทางเดิน)
        for (int y = 0; y < GRID_H; y++) {
            for (int x = 0; x < GRID_W; x++) {
                mapData[y][x] = 0; 
            }
        }
    }

// แปลง Grid Index เป็นพิกัดโลก (World Position) สำหรับ Vector2D
Vector2D Level::gridToWorld(int gx, int gy) {
        float worldX = (float)(OFFSET_X + (gx * TILE_SIZE) + (TILE_SIZE / 2));
        float worldY = (float)(OFFSET_Y + (gy * TILE_SIZE) + (TILE_SIZE / 2));
        return Vector2D(worldX, worldY);
    }

// แปลงพิกัดเมาส์/โลก กลับเป็น Grid Index (ใช้เช็คว่าคลิกโดนช่องไหน)
bool Level::worldToGrid(Vector2D worldPos, int& gx, int& gy) {
        gx = (int)((worldPos.x - OFFSET_X) / TILE_SIZE);
        gy = (int)((worldPos.y - OFFSET_Y) / TILE_SIZE);
        
        // เช็คว่าพิกัดที่ส่งมา อยู่ภายในขอบเขตของ Grid หรือไม่
        return (gx >= 0 && gx < GRID_W && gy >= 0 && gy < GRID_H);
    }

// ฟังก์ชันวาด Grid ทั้งหมด (พื้นหลัง)
void Level::render(SDL_Renderer* renderer) {
        for (int y = 0; y < GRID_H; y++) {
            for (int x = 0; x < GRID_W; x++) {
                SDL_Rect dest = {
                    OFFSET_X + (x * TILE_SIZE),
                    OFFSET_Y + (y * TILE_SIZE),
                    TILE_SIZE,
                    TILE_SIZE
            };
            switch ( (mapData[y][x])){
            case 0: // empty
                SDL_SetRenderDrawColor(renderer, 50, 160, 50, 255); 
                break;    
            
            case 1: // path
                SDL_SetRenderDrawColor(renderer, 150, 100, 50, 255);
                break;
                
            case 2: // wall
                SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                break;

            case 3:  // tower placed
                SDL_SetRenderDrawColor(renderer, 0, 0, 200, 255);
                break;

            default:
                SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
                break;
            }

            // วาดภาพลงไปในตำแหน่งที่คำนวณ Offset ไว้แล้ว
            SDL_RenderFillRect(renderer, &dest);
            // สร้าง Grid outline
            SDL_SetRenderDrawColor(renderer, 229, 235, 231, 127);
            SDL_RenderDrawRect(renderer, &dest);
            }
        }
    }

// ฟังก์ชันวาดตัวละครหรือป้อม (Object) ให้ตรงกับ Grid กึ่งกลาง
void Level::renderEntity(SDL_Renderer* renderer, SDL_Texture* tex, Vector2D pos, float angle) {
        SDL_Rect dest;
        dest.w = TILE_SIZE; 
        dest.h = TILE_SIZE;
        dest.x = (int)(pos.x - dest.w / 2);
        dest.y = (int)(pos.y - dest.h / 2);

        double degrees = angle * (180.0 / M_PI);
        SDL_RenderCopyEx(renderer, tex, NULL, &dest, degrees, NULL, SDL_FLIP_NONE);
    }

bool Level::isEmpty(int gx,int gy) const {
    return mapData[gy][gx] == EMPTY;
}

void Level::setTile(int gx,int gy, TileType type) {
    mapData[gy][gx] = type;
}

void Level::reset() {
    for (int y = 0; y < GRID_H; y++) {
        for (int x = 0; x < GRID_W; x++) {
            mapData[y][x] = 0;  // Reset all tiles to EMPTY
        }
    }
}