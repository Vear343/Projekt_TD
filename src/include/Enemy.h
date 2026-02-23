#pragma once

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include "Entity.h"

class Enemy : public Entity {
public:
    // สร้าง Enemy โดยรับพิกัดเริ่มต้น, width, height, speed, texture, และเส้นทางที่ต้องการเดิน
    Enemy(float p_x, float p_y, float p_width, float p_height, float p_speed, SDL_Texture* p_texture, const std::vector<Vector2D>& path);

    bool hasFinished() const { return finished;}
    void update(float deltaTime);
private:
    std::vector<Vector2D> path;
    int currentPathIndex = 0;
    bool finished = false;

    float speed;
};