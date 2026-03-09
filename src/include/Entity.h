#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Vector2D.h"

class Entity {
public:
    // Constructor รับค่าพื้นฐานในการแสดงผล
    Entity(float p_x, float p_y, float p_width, float p_height, SDL_Texture* p_texture);
    virtual ~Entity() = default;
    
    SDL_Rect getCurrentFrame() const { return currentFrame; }
    SDL_Texture* getTexture() const { return texture; }
    
    // Getter พื้นฐาน
    float getX() const { return x; }
    float getY() const { return y; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }
    SDL_Rect getCollider();
    
    // ฟังก์ชันหาจุดกึ่งกลาง (Center) สำคัญมากสำหรับป้อมในการคำนวณระยะยิง
    Vector2D getCenter() const {
        return Vector2D(x, y);
    }
    
    // Virtual functions ที่ลูกหลานต้องเอาไปเขียนต่อ
    virtual void update(float deltaTime) = 0; 
    // virtual void render(SDL_Renderer* renderer);
    
protected:
    float x, y;
    float width, height;
    SDL_Rect collider;
    SDL_Rect currentFrame; // สำหรับอนิเมชันในอนาคต
    SDL_Texture* texture;
};