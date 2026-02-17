#include<iostream>
#include<SDL2/SDL.h>

#include"RenderWindow.h"

const int WIDTH = 800, HEIGHT = 600;

int main(int argc, char* argv[])
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) > 0)
    {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    // Create a window
    RenderWindow window("SDL Window", WIDTH, HEIGHT);

    SDL_Texture* texture = window.loadTexture("assets/test_map.jpg");

    SDL_Event event;
    // Main loop
    while(true){
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT) return false;
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) return false;
            }
            // Clear the window, render the texture, and display it
            window.clear();
            window.render(texture);
            window.display();
        }
    }
    // Clean up and quit SDL
    window.cleanUp();
    SDL_Quit();

    return 0;
}