#include <SDL/SDL.h>
#include <string>
#include "graphicsutil.h"
#include "SDL/SDL_image.h"
using namespace std;

SDL_Surface* loadSurface(const string& filename) {
    //Declare the optimized image
    SDL_Surface* ret = 0;
    //Load the image
    SDL_Surface* loadedImage = IMG_Load(filename.c_str());

    //If the image got loaded without problems
    if (loadedImage) {
        //Optimize format
        ret = SDL_DisplayFormatAlpha(loadedImage);
        //Free the temporary surface
        SDL_FreeSurface(loadedImage);
    }

    return ret;
}

void drawCenteredSurface(const Coord& position, SDL_Surface* image, SDL_Surface* screen) {
    //Rectangle to hold the position as SDL_BlitSurface only takes SDL_Rect
    SDL_Rect pos;
    pos.x = static_cast<Sint16>(position.x - image->w/2.0); //TODO: make this independent of image size
    pos.y = static_cast<Sint16>(position.y - image->h/2.0);

    //Draw the surface
    SDL_BlitSurface(image, 0, screen, &pos);
}
