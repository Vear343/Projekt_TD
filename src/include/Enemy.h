#pragma once

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include "Entity.h"

class Enemy : public Entity {
public:
    // สร้าง Enemy โดยรับพิกัดเริ่มต้น, width, height, speed, texture, และเส้นทางที่ต้องการเดิน
    Enemy(float p_x, float p_y, SDL_Texture* p_texture, const std::vector<Vector2D>& path);
    virtual ~Enemy() = default;
 
    bool isAlive() const {return alive;};
    Vector2D getPosition() const {return Vector2D(x,y);};
    void takeDamage(float dmg);
    bool hasFinished() const { return finished;}
    void update(float deltaTime);
private:
    static constexpr float width = 32;
    static constexpr float height = 32;
    float speed = 200;
    float Hp = 100;

    std::vector<Vector2D> path;
    int currentPathIndex = 0;
    bool alive = true;
    bool finished = false;
};