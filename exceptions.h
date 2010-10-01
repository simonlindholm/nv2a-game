#pragma once
#include <exception>

// This is the standard class for SDL exceptions
class GameException : public std::exception {
	public:
		virtual const char* what() const throw();
		virtual ~GameException() throw();
};

// Exception symbolising quitting the program
class ExitException : public std::exception {
	public:
		virtual ~ExitException() throw();
		virtual const char* what() const throw();
};
