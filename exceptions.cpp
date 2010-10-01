#include <SDL/SDL.h>
#include "exceptions.h"

GameException::~GameException() throw(){}
const char* GameException::what() const throw(){
	return SDL_GetError();
}

ExitException::~ExitException() throw(){}
const char* ExitException::what() const throw(){
	return "";
}
