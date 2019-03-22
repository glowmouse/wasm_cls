///
/// Game of life programmed with Web Assembly
/// (C) Andrew Brownbill 2019
///  

#include <iostream>
#include <utility>
#include <unordered_map>
#include <vector>
#include <string>

#include <SDL/SDL.h>
#include <emscripten.h>

// X and Y screen resolution
constexpr int X_SCREEN=1024;
constexpr int Y_SCREEN=768;

// Draw the game of life buffer on the screen.
void drawScreen( SDL_Surface *screen, unsigned int count )
{
  const Uint32 c = SDL_MapRGBA( screen->format, count%256, count/256, 128, 255 );

  // Clear
  Uint32 *start = (Uint32*)screen->pixels;
  Uint32 *end= start + X_SCREEN * Y_SCREEN;
  for ( Uint32 *cur = start; cur < end; ++cur ) *cur = c;
}

// Creates the screen and initial board.  Updates the game.
class Optrace
{
  public:

  Optrace( const Optrace& other ) = delete;
  Optrace& operator=( const Optrace& other ) = delete;

  Optrace() 
  {
    SDL_Init(SDL_INIT_VIDEO );
    screen = SDL_SetVideoMode(X_SCREEN, Y_SCREEN, 32, SDL_SWSURFACE);
    count = 0;
  }

  ~Optrace()
  {
    screen = nullptr;
    SDL_Quit();
  }
  
  void update( void )
  {
    count+=1;
    if (SDL_MUSTLOCK(screen)) SDL_LockSurface(screen);
    drawScreen( screen, count  );
    if (SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
    SDL_UpdateRect(screen, 0, 0, 0, 0);
  }

  private:
  SDL_Surface *screen;
  unsigned int count;
};

std::unique_ptr< Optrace > singleton; 

// Advance forward one.  Callback from emscripten
void tick() {
  singleton->update(); 
}

int main(int argc, char** argv) 
{
  srand(time( nullptr ));
  
  singleton = std::unique_ptr< Optrace >( new Optrace());
  emscripten_set_main_loop(tick, 0, 0);

  return 0;
}

