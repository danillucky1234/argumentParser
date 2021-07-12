#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include "stdafx.h"

class Exception : virtual public std::exception
{
private:
	std::string message;	// Message with an error
	Exception() {};			//Make the constructor private, so that you can't create a class without filling in the required fields

public:
	Exception(const std::string & msg) : message(msg) {}	// 	A constructor that initializes a private field
	virtual ~Exception() {}											// Destructor
	virtual const char* what() const noexcept
	{
		return this->message.c_str();								// The function the programmer will call to see what the error is
	}
};

#endif // EXCEPTIONS_H
