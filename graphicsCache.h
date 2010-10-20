#pragma once
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <vector>
#include "graphicsutil.h"

using namespace std;

/*
    GraphicsCache is class that stores every image and every variation of those images
    so that you can pre-render the images and store them in the graphics memory.
    This way the program won't have to render things such as rotating and loading images
    real-time which would be a major performance set-back.
*/

class GraphicsCache {
    public:
        enum ImageTypes {
            ImgHuman,
            ImgEnd
        };

        static GraphicsCache& get();
        SDL_Surface* getImg(ImageTypes it, int angle) const;
    private:
        rotateableObject cache[ImgEnd];
        //Constructors/Destructors can be private as they are called by the get() function
        GraphicsCache(const GraphicsCache&); //left undefined
        GraphicsCache();
        ~GraphicsCache();
};
