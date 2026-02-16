#pragma once
#include<SDL2/SDL.h>
#include<SDL2/SDL_test_images.h>

class RenderWindow
{
public:
    RenderWindow(const char* p_title, int p_w, int p_h); 
    // Load a texture from a file path
    SDL_Texture* loadTexture(const char* p_filePath);
    void cleanUp();
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
};