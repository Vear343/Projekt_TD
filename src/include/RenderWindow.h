#pragma once
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>

#include "Entity.h"

class RenderWindow
{
public:
    RenderWindow(const char* p_title, int p_w, int p_h); 
    // Load a texture from a file path
    SDL_Texture* loadTexture(const char* p_filePath);
    void cleanUp();
    void clear();
    void render(Entity& p_entity);
    void display();
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
};