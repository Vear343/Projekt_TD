#pragma once
#include <vector>
#include <SDL2/SDL.h>
#include "Vector2D.h"
#include "Entity.h"

class Level {
public:
    static constexpr int TILE_SIZE = 32;
    static constexpr int GRID_W = 25;
    static constexpr int GRID_H = 19;
    
    // คำนวณ Offset สำหรับจอ 800x600 ให้ด่านอยู่ตรงกลาง
    static constexpr int OFFSET_X = (1280 - (GRID_W * TILE_SIZE)) / 2; // 200
    static constexpr int OFFSET_Y = (720 - (GRID_H * TILE_SIZE)) / 2; // 148

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

    // ฟังก์ชันวาด Grid ทั้งหมด (พื้นหลัง) โดยใช้ Texture ที่ส่งเข้ามา
    void render(SDL_Renderer* renderer, Entity& p_entity) {
        for (int y = 0; y < GRID_H; y++) {
            for (int x = 0; x < GRID_W; x++) {
                SDL_Rect dest = {
                    OFFSET_X + (x * TILE_SIZE),
                    OFFSET_Y + (y * TILE_SIZE),
                    TILE_SIZE,
                    TILE_SIZE
            };

            // วาดภาพลงไปในตำแหน่งที่คำนวณ Offset ไว้แล้ว
            SDL_RenderCopy(renderer, p_entity.getTexture(), NULL, &dest);
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