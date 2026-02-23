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

    enum TileType{
        EMPTY = 0,
        PATH = 1,
        WALL = 2,
    };

    Level() {
        // กำหนดค่าเริ่มต้นให้ Grid (0 = ว่าง, 1 = ทางเดิน)
        for (int y = 0; y < GRID_H; y++) {
            for (int x = 0; x < GRID_W; x++) {
                mapData[y][x] = 0; 
            }
        }
    }

    // --- การแปลงพิกัดที่รวมค่า Offset ---

    // แปลง Grid Index เป็นพิกัดโลก (World Position) สำหรับ Vector2D
    Vector2D gridToWorld(int gx, int gy) {
        float worldX = (float)(OFFSET_X + (gx * TILE_SIZE) + (TILE_SIZE / 2));
        float worldY = (float)(OFFSET_Y + (gy * TILE_SIZE) + (TILE_SIZE / 2));
        return Vector2D(worldX, worldY);
    }

    // แปลงพิกัดเมาส์/โลก กลับเป็น Grid Index (ใช้เช็คว่าคลิกโดนช่องไหน)
    bool worldToGrid(Vector2D worldPos, int& gx, int& gy) {
        gx = (int)((worldPos.x - OFFSET_X) / TILE_SIZE);
        gy = (int)((worldPos.y - OFFSET_Y) / TILE_SIZE);
        
        // เช็คว่าพิกัดที่ส่งมา อยู่ภายในขอบเขตของ Grid หรือไม่
        return (gx >= 0 && gx < GRID_W && gy >= 0 && gy < GRID_H);
    }

    // ฟังก์ชันวาด Grid ทั้งหมด (พื้นหลัง)
    void render(SDL_Renderer* renderer) {
        for (int y = 0; y < GRID_H; y++) {
            for (int x = 0; x < GRID_W; x++) {
                SDL_Rect dest = {
                    OFFSET_X + (x * TILE_SIZE),
                    OFFSET_Y + (y * TILE_SIZE),
                    TILE_SIZE,
                    TILE_SIZE
            };
            switch ( (mapData[y][x])){
            case 0:
                SDL_SetRenderDrawColor(renderer, 50, 160, 50, 255);
                break;    
            
            case 1:
                SDL_SetRenderDrawColor(renderer, 150, 100, 50, 255);
                break;
                
            case 2:
                SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                break;

            default:
                SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
                break;
            }

            // วาดภาพลงไปในตำแหน่งที่คำนวณ Offset ไว้แล้ว
            SDL_RenderFillRect(renderer, &dest);
            // สร้าง Grid outline
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &dest);
            }
        }
    }

    // ฟังก์ชันวาดตัวละครหรือป้อม (Object) ให้ตรงกับ Grid กึ่งกลาง
    void renderEntity(SDL_Renderer* renderer, SDL_Texture* tex, Vector2D pos, float angle) {
        SDL_Rect dest;
        dest.w = TILE_SIZE; 
        dest.h = TILE_SIZE;
        dest.x = (int)(pos.x - dest.w / 2);
        dest.y = (int)(pos.y - dest.h / 2);

        double degrees = angle * (180.0 / M_PI);
        SDL_RenderCopyEx(renderer, tex, NULL, &dest, degrees, NULL, SDL_FLIP_NONE);
    }

private:
    int mapData[GRID_H][GRID_W];
};