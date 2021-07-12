#ifndef ARGUMENT_H
#define ARGUMENT_H

#include "stdafx.h"

typedef unsigned int uint;

class Argument
{
private:
	std::vector<std::string> keyVariations;				// A vector of possible key variations, such as {"-r", "--resolution"} 
	std::string definition;								// A string that describes this key, e.g. "This command allows us to set the size of the screen in pixels".
	bool isRequiredKey;									// Boolean value indicating whether the key must be present or not
	uint nargs;											// nargs == number of arguments 
														// If this key requires a parameter, we specify how many parameters this key requires
	std::vector<std::string> arguments;					// A vector where the arguments will be written, because one key can have a bunch of parameters, for example: '--rgb 0 255 0'

	std::vector<std::string> getKeyVariations() const;	// This function returns a vector with keys, it is used only by the ArgumentParser class 
	bool isThisKeyRequired() const;						// The function returns a boolean value - whether the given key is mandatory or not
	uint getNargs() const;								// The function returns the number of arguments needed for the given key (if any)

	// Declare the ArgumentParser class as a friend so that it has access to private functions
	friend class ArgumentParser;

	// Make the constructor private, so that you can't create a class without filling in the required fields
	Argument(); 
public:
	Argument(const std::vector<std::string> & keys, const std::string & definition, const bool & isRequiredKey, const unsigned int & nargs, const std::vector<std::string> & arguments); // If you know in advance what the arguments of a given key will be, you can specify them here
	
	Argument(const std::vector<std::string> & keys, const std::string & definition, const bool & isRequiredKey, const uint & nargs = 0); // If the arguments are not known in advance, they can be absolutely anything, then we can specify neither the arguments nor the number of arguments. 

	std::vector<std::string> getArguments() const; // The function returns a vector with all the arguments of the given key, if they are available.
	std::string getDefinition() const; // The function returns a description of what the parameter does
};

#endif // ARGUMENT_H
