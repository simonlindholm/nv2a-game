#pragma once
#include <exception>
#include <string>

// This is the standard class for SDL exceptions. This is needed because SDL
// was originally written in C and doesn't use C++-style error handling.
// Wraps SDL_GetError().
class SDL_Exception : public std::exception {
	public:
		virtual const char* what() const throw();
		virtual ~SDL_Exception() throw();
};

// Exception thrown when not able to load the configuration, for any reason.
class ConfigException : public std::exception {
	private:
		std::string msg;
	public:
		ConfigException(const std::string& msg);
		virtual const char* what() const throw();
		virtual ~ConfigException() throw();
};

// Exception symbolising quitting the program
class ExitException {};
