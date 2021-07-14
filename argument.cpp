#include "argument.h"
#include "exception.h"

Argument::Argument(
		const std::vector<std::string> & keys,
		const std::string & definition,
		const bool & isRequiredKey,
		const uint & nargs,
		const std::vector<std::string> & arguments) : Argument::Argument( // First a constructor with parameters without a vector is called, all fields are initialized there, and in this constructor the last vector with possible key arguments is initialized
											keys,
											definition,
											isRequiredKey,
											nargs
											) 
{
	for (auto & x : arguments)
	{
		this->arguments.push_back(x);
	}
}

// If the arguments are not known beforehand. There is no list of possible arguments, e.g. a number can be anything, then the data calls the constructor
Argument::Argument(
		const std::vector<std::string> & keys,
		const std::string & definition,
		const bool & isRequiredKey,
		const uint & nargs
		) 
{
	if (keys.size() == 0)
	{
		throw Exception("Every key should have at least one variation under which it can be handled");
	}
	for (auto & x : keys)
	{
		this->keyVariations.push_back(x);
	}
	this->definition = definition;
	this->isRequiredKey = isRequiredKey;
	this->nargs = nargs;
}

// This function returns a vector with keys, it is used only by the ArgumentParser class 
std::vector<std::string> Argument::getKeyVariations() const
{
	return this->keyVariations;
}

// The function returns a boolean value - whether the given key is mandatory or not
bool Argument::isThisKeyRequired() const
{
	return this->isRequiredKey;
}

// The function returns the number of arguments needed for the given key (if any)
uint Argument::getNargs() const
{
	return this->nargs;
}

// The function returns a vector with all the arguments of the given key, if they are available
std::vector<std::string> Argument::getArguments() const
{
	// If this key doesn't require arguments, throw exception
	if (this->nargs > 0)
	{
		// If This argument require arguments, but actually haven't them, throw exception
		if (this->arguments.size() > 0)
		{
			return this->arguments;
		}
		else
		{
			throw (Exception("This key doesn't have any arguments"));
		}
	}
	else
	{
		throw (Exception("This key doesn't require any arguments!"));
	}
}

// The function returns a description of what the parameter does
std::string Argument::getDefinition() const
{
	return this->definition;
}
