#pragma once
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<vector>

#include"Vector2D.h"

class Entity
{
public:
    Entity(float p_x = 0, float p_y = 0, float p_width = 32, float p_height = 32, SDL_Texture* p_texture = nullptr);
    float getX() {return y;};
    float getY() {return x;};
    float getWidth() {return width;};
    float getHeight() {return height;};
    void update(float deltatime);
    SDL_Texture* getTexture() {return texture;};
    SDL_Rect getCollider() {return collider;};
    SDL_Rect getCurrentFrame() {return currentFrame;};
protected:
    float x, y, width, height;
    SDL_Rect collider;
    SDL_Rect currentFrame;
    SDL_Texture* texture;
};