#include<cmath>
#include<iostream>

#include "Enemy.h"

Enemy::Enemy(float p_x, float p_y, float p_width, float p_height, float p_speed, SDL_Texture* p_texture,const std::vector<Vector2D>& p_path)
    :Entity(p_x, p_y, p_width, p_height, p_texture), 
    path(p_path), 
    speed(p_speed)
{
    currentFrame.w = width;
    currentFrame.h = height;
}

void Enemy::update(float deltaTime) {
    if (currentPathIndex >= path.size()){
        finished = true;
        return;
    }

    Vector2D target = path[currentPathIndex];
    Vector2D currentPos(x, y);
    Vector2D direction = target - currentPos;

    // Calculate distance to the target
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance < 5.0f){
        currentPathIndex++;
        return;
    }

    // Normalize the direction vector
    direction.x /= distance;
    direction.y /= distance;

    // Move towards the target
    x += direction.x * speed * deltaTime;
    y += direction.y * speed * deltaTime;

    std::cout << "currentPathIndex = " << currentPathIndex << std::endl;
    std::cout << "Position {" << x << ", " << y << " }\n\n";
}