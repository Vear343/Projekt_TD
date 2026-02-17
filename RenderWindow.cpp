#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<iostream>
#include "RenderWindow.h"

// Constructor to create a window and renderer
RenderWindow::RenderWindow(const char* p_title, int p_w, int p_h)
    :window(NULL), renderer(NULL)
{
    window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN);

    if (window == NULL)
    {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
    }
    else
    {
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    }
}

// Load a texture from a file path
SDL_Texture* RenderWindow::loadTexture(const char* p_filepPath)
{
    SDL_Texture* texture = NULL;
    texture = IMG_LoadTexture(renderer, p_filepPath);
    // Check if the texture was loaded successfully
    if (texture == NULL)
    {
        std::cout << "Unable to load texture! SDL_Error: " << SDL_GetError() << std::endl;
    }
    return texture;
}

void RenderWindow::cleanUp()
{
    SDL_DestroyWindow(window);
}

void RenderWindow::clear()
{
    SDL_RenderClear(renderer);
}

void RenderWindow::render(SDL_Texture* p_texture)
{
    SDL_RenderCopy(renderer, p_texture, NULL, NULL);
}

void RenderWindow::display()
{
    SDL_RenderPresent(renderer);
}