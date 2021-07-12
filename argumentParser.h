#ifndef ARGUMENTPARSER_H
#define ARGUMENTPARSER_H

#include "stdafx.h"
#include "argument.h"

class ArgumentParser
{
private:
	std::vector<Argument> possibleArguments;	// A vector of Argument classes. A vector of all possible keys that the program's author specifies
	uint numberOfRequiredKeys;					// When filling the possibleArguments vector, we will increment the counter with the number of keys that are mandatory for program execution, so that later, in the parseArgs function, we can determine whether all mandatory keys have been used
	std::vector<Argument> realArguments;		// A vector of Argument classes, all useful parameters that will be passed to the program will be written in this vector and later we will pull from this vector all arguments, definition, check whether the key was used, etc.

	uint getIndexFromVectorInWhichSearchKey(const std::vector<Argument> & args, const std::string & key); // This function searches a given vector for the key we need. If it exists, it returns the index of the vector where the key is located, but if there is no such key in the vector, it returns -1.

public:
	ArgumentParser();														// A default constructor where we just initialize the numberOfRequiredKeys variable to zero to increment it later and use it in the parseArgs function to check if all required parameters were passed
	void addArgument(const Argument & arg);									// This function checks if the key exists in the parser, and if it does, throws an exception; if the key has not yet been added, it adds it to the vector of possible keys
	void parseArgs(const int & argc, char ** argv);							// It takes each argv argument and looks for the required vector from possibleArguments, checks if it is a required parameter - increases the counter. If at the end the number of required keys does not match, then throws an exception saying that there are not enough required keys.
																			// Also checks possibleArguments nargs (number of parameters that the given key must accept) and reads the same number of arguments into the vector, if there are not enough arguments (argc has fewer unreadable elements than required for the given key), then throw exception. Writes all keys into another vector - realArguments, so that later we can take definition, arguments and other values from realArguments in other functions

	std::vector<std::string> getArgumentsByKey(const std::string & key); // This function takes a string with a key and looks for it among those entered by the user. If he passed parameters to this key - then we will display all these parameters in a vector
	std::string getDefinitionByKey(const std::string & key);			// If a programmer needs to print the definition of a given key (for example, for a help function), he can pass the key to this function and it will return a description
	bool isUsedTheKey(const std::string & key);							// If there are keys in the program that do not have a parameter and the programmer needs to process them, it is sufficient to use this function, which will tell you whether the key was entered by the user
};

#endif // ARGUMENTPARSER_H
