#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>
#include <SDL2/SDL.h>

class Point {
  public:
    Point(double x, double y) : x(x), y(y) {}
    double x;
    double y;
};

int initialiseSDL() {
  if(SDL_Init(SDL_INIT_VIDEO) < 0) {
      std::cout << "Failed to initialize the SDL2 library\n";
      return -1;
  }
  return 0;
}

SDL_Window* createWindow(const int width, const int height) {
  SDL_Window* window = SDL_CreateWindow("SDL2 Window",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        width,
                                        height,
                                        0);

  if(!window)
    {
        std::cout << "Failed to create window\n";
        exit(-1);
    }
  
  return window;
}

SDL_Surface* createSurface(SDL_Window* window) {
  SDL_Surface* window_surface = SDL_GetWindowSurface(window);

  if (!window_surface) {
    std::cout << "Failed to get the surface from the window\n";
    exit(-1);
  }

  return window_surface;
}

SDL_Renderer* createRenderer(SDL_Window* window) {
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (!renderer) {
    std::cout << "Failed to create renderer\n";
    exit(-1);
  }

  return renderer;
}

int main() {
  initialiseSDL();
  SDL_Window* window = createWindow(800, 800);
  SDL_Surface* window_surface = createSurface(window);

  SDL_Renderer* renderer = createRenderer(window);

  // Event loop
  SDL_Event event;
 bool quit = false;
 while (!quit) {
      while (SDL_PollEvent(&event)) {
          if (event.type == SDL_QUIT) {
              quit = true;
          }
      }
      // Rendering code moved outside of the event loop
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      SDL_RenderClear(renderer); // Clear the renderer to a solid color
      SDL_RenderDrawPoint(renderer, 100, 100);
      SDL_RenderPresent(renderer);
 }
  
  

  // Cleanup
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
