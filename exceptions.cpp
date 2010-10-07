#include <SDL/SDL.h>
#include "exceptions.h"

ConfigException::~ConfigException() throw() {}
ConfigException::ConfigException(const char* msg) {
	this->msg = msg;
}
const char* ConfigException::what() const throw() {
	return this->msg;
}

SDL_Exception::~SDL_Exception() throw() {}
const char* SDL_Exception::what() const throw() {
	// The error is held by the global SDL function SDL_GetError
	return SDL_GetError();
}
