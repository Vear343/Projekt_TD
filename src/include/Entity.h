#pragma once
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<vector>

#include"Vector2D.h"

class Entity
{
public:
    Entity(float p_x = 0, float p_y = 0, SDL_Texture* p_texture = nullptr);
    float getX();
    float getY();
    SDL_Texture* getTexture();
    SDL_Rect getCurrentFrame();
protected:
    float x, y;
    SDL_Rect currentFrame;
    SDL_Texture* texture;
};