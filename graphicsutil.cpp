#include <SDL/SDL.h>
#include <string>
#include "graphicsutil.h"
#include "SDL/SDL_image.h"
using namespace std;

SDL_Surface* load_surface(string filename) {
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

void draw_surface(Coord position, SDL_Surface* image, SDL_Surface* screen) {
    //Rectangle to hold the position as SDL_BlitSurface only takes SDL_Rect
    SDL_Rect pos;
    pos.x = position.x - 16; //TODO: make this independent of image size
    pos.y = position.y - 16;

    //Draw the surface
    SDL_BlitSurface(image, 0, screen, &pos);
}
