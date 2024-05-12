#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>
#include <SDL2/SDL.h>

#define GRAVITY 9.81
#define AIR_DENSITY 1.225

enum ObjectState {
  InAir,
  OnFloor
};

enum ObjectShape {
  SquareS,
  CircleS
};

class Square {
  public:
    Square(double x, double y, double size) : x(x), y(y), size(size) {}
    double x;
    double y;
    double size;            //m^2
    double velocity = 0;    //m/s
    double mass = 100;        //kg
    double dt = 0.1;        //s
    ObjectState state;
    ObjectShape shape = ObjectShape::SquareS;
  
    void draw(SDL_Renderer* renderer) {
      SDL_Rect rect;
      rect.x = x;
      rect.y = y;
      std::cout << "Y: " << y << "\n";
      rect.w = size;
      rect.h = size;
      SDL_RenderFillRect(renderer, &rect);
    }

    double dragCoefficient(ObjectShape shape) {
      switch (shape) {
        case ObjectShape::SquareS:
          return 1.05;
        case ObjectShape::CircleS:
          return 0.47;
        default:
          std::cout << "Invalid shape\n";
          return 1.05;
      }
    }
    
    double accelerationY(ObjectShape shape) {
      double drag = 0.5 * AIR_DENSITY * dragCoefficient(shape) * pow(velocity, 2) * size;
      double forceY = mass * GRAVITY;
      double accelerationY = forceY / mass;

      accelerationY = (accelerationY - drag / mass) * dt;      //divide by mass as drag is a force and F = ma

      return accelerationY;
    }

    void update(int floorY) {
      velocity += accelerationY(shape) * dt;  

      if (y + size >= floorY) {
        y = floorY;
        velocity = 0;
        y = floorY - size;
        state = ObjectState::OnFloor;
      } else {
        state = ObjectState::InAir;
        y += velocity;
      }
    }
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

  int window_width, window_height;

  bool checkStep = true;

  Square object = Square(100, 100, 50);
  // Event loop
  SDL_Event event;
 bool quit = false;
 while (!quit) {
      while (SDL_PollEvent(&event)) {
          if (event.type == SDL_QUIT) {
              quit = true;
          }
          if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_SPACE) {
              checkStep = true;
            }
          }
      }

      if (checkStep) {
        std::cout << "checkStep" << checkStep << std::endl;

        SDL_GetRendererOutputSize(renderer, &window_width, &window_height);
        //clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer); // Clear the renderer to a solid color

        //draw floor        
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        int floorY = window_height - 20;
        SDL_RenderFillRect(renderer, new SDL_Rect{0, floorY, window_width, 20}); 

        SDL_SetRenderDrawColor(renderer, 255, 20, 20, 255);
        object.update(floorY);
        object.draw(renderer);
        SDL_RenderPresent(renderer);

        checkStep = false;
      }
 }
  
  

  // Cleanup
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
