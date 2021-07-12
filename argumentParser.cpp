#include "argumentParser.h"
#include "exception.h"

// A default constructor where we just initialize the numberOfRequiredKeys variable to zero to increment it later and use it in the parseArgs function to check if all required parameters were passed
ArgumentParser::ArgumentParser()
{
	this->numberOfRequiredKeys = 0;
}

// This function checks if the key exists in the parser, and if it does, throws an exception; if the key has not yet been added, it adds it to the vector of possible keys
void ArgumentParser::addArgument(const Argument & arg)
{
	// Later, in the parseArgs function, when we finish parsing all the items from the command line, we will compare the number of mandatory keys. If it does not match, we throw an exception. That's why we check if the given key is mandatory or not.
	if (arg.isThisKeyRequired())
	{
		++(this->numberOfRequiredKeys);
	}
	
	// Let's check if we have added such a key. If so, throw an exception that such a key already exists
	std::vector<std::string> possibleAddedKeys = arg.getKeyVariations();
	for (auto & x : this->possibleArguments)
	{
		std::vector<std::string> vectorOfKeys = x.getKeyVariations();
		// We don't need to check all the keys from the possibleAddedKeys vector, we only need to check the first argument from this vector.
		// If we didn't find such a key among the already added objects in the possibleArguments vector, then such a key has not been added yet.
		if (std::find(vectorOfKeys.begin(), vectorOfKeys.end(), possibleAddedKeys[0]) != vectorOfKeys.end())
		{
			// And if we found one, we throw an exception with an explanation that such a key has already been added to the parser.
			throw Exception("You have already added the " + possibleAddedKeys[0] + " key!");
		}
	}
	this->possibleArguments.push_back(arg);
}

// This function searches a given vector for the key we need. If it exists, it returns the index of the vector where the key is located, but if there is no such key in the vector, it returns -1.
uint ArgumentParser::getIndexFromVectorInWhichSearchKey(const std::vector<Argument> & args, const std::string & key)
{
	std::vector<std::string> keyNameVariationsVec;
	// Passing through the vector with arguments
	for (int i = 0; i < args.size(); ++i)
	{
		keyNameVariationsVec = args[i].getKeyVariations();
		// Look in the key vector for the key we need
		if (std::find(keyNameVariationsVec.begin(), keyNameVariationsVec.end(), key) != keyNameVariationsVec.end())
		{
			// If we find the right key, we return the vector index
			return i;
		}
	}

	// If we didn't find that key in the vector we return -1
	return -1;
}

// It takes each argv argument and looks for the required vector from possibleArguments, checks if it is a required parameter - increases the counter. If at the end the number of required keys does not match, then throws an exception saying that there are not enough required keys.
void ArgumentParser::parseArgs(const int & argc, char ** argv)
{
	uint theNumberOfRequiredKeys = 0;
	uint indexOfVectorWhereIsLocatedFoundedKey;
	// Pass through all arguments passed to the program, skipped only the first parameter, because this parameter is the name of the executable program
	for (int i = 1; i < argc; ++i)
	{
		indexOfVectorWhereIsLocatedFoundedKey = getIndexFromVectorInWhichSearchKey(this->possibleArguments, argv[i]);
		if (indexOfVectorWhereIsLocatedFoundedKey != -1)
		{
			// If we found such a key in the vector of possibleArguments, then we must:
			// 1) See if this key is required
			// 2) If the key has nargs(number of arguments) > 0, then we read the required number of parameters from the command line
			if (this->possibleArguments[indexOfVectorWhereIsLocatedFoundedKey].isThisKeyRequired())
			{
				// If the key is mandatory, then increase the counter of required keys
				++theNumberOfRequiredKeys;
			}
			uint nArgs = this->possibleArguments[indexOfVectorWhereIsLocatedFoundedKey].getNargs();
			if (nArgs != 0)
			{
				// если после ключа не будет всех аргументов, то кидаем ексепшен что не хватает аргументов данному ключу
				// If the key does not have all the arguments after it, then we throw an wxception saying that there are not enough arguments to that key
				if (argc - i - 1 < nArgs)
				{
					std::string ex = "The key ";
					ex.append(argv[i]);
					ex.append(" lacks arguments!");
					throw Exception(ex);
				}
				// If there are enough arguments on the command line, we must write all these parameters for the key in a vector
				std::vector<std::string> usersArgumentsForArgvKey; // In this vector we will store all the arguments of this key
				std::vector<std::string> possibleArgumentsFromProgrammer;
				bool isPresentPossibleArguments = false;
				try
				{
					// If programmer specified arguments for this key, we should check it
					possibleArgumentsFromProgrammer = this->possibleArguments[indexOfVectorWhereIsLocatedFoundedKey].getArguments();
					isPresentPossibleArguments = true;
				}
				catch(const Exception & ex)
				{
					// If we caught an error, it means that this key does not have any possible arguments and there is no need to compare it with anything.
					isPresentPossibleArguments = false;
				}

				if (isPresentPossibleArguments)
				{
					// If the programmer has specified what arguments this key can have, then we must check if the user's argv[i] matches the programmer's requirements
					for (int j = 1; j < nArgs + 1; ++j)
					{
						if (std::find(possibleArgumentsFromProgrammer.begin(), possibleArgumentsFromProgrammer.end(), argv[i + j]) != possibleArgumentsFromProgrammer.end())
						{
							// If we found a match between argv[i] and possibleArgumentsFromProgrammer, then we can write this value
							usersArgumentsForArgvKey.push_back(argv[i + j]);
						}
						else
						{
							// If the programmer gave certain arguments, that means that only they can be used together with this key, we should throw out an error that the user entered the wrong data for this key
							std::string ex = "The ";
							ex.append(argv[i]);
							ex.append(" key requires a certain parameter(s). Double-check the data you entered");
							throw Exception(ex);
						}
					}
				}
				else
				{
					// If the programmer didn't specified any arguments, we accept everything that the user entered
					for (int j = 1; j < nArgs + 1; ++j)
					{
							usersArgumentsForArgvKey.push_back(argv[i + j]);
					}
				}
				i += nArgs; // So as not to keep counting the arguments we have already added to the vector, we skip them
				
				// Add a new object to the vector of passed arguments with the specified parameters
				Argument arg(
							this->possibleArguments[indexOfVectorWhereIsLocatedFoundedKey].getKeyVariations(), 	// keyVariations
							this->possibleArguments[indexOfVectorWhereIsLocatedFoundedKey].getDefinition(),		// definition
							this->possibleArguments[indexOfVectorWhereIsLocatedFoundedKey].isThisKeyRequired(),	// isRequiredKey
							nArgs,																				// nArgs
							usersArgumentsForArgvKey															// arguments
							);
				this->realArguments.push_back(arg);
				
				// To avoid writing one more object, skip to the next iteration
				continue;
			}
			// Push new object to the vector realArgumetns without any arguments (key doesn't have them)
			Argument arg(
					this->possibleArguments[indexOfVectorWhereIsLocatedFoundedKey].getKeyVariations(), 	// keyVariations
					this->possibleArguments[indexOfVectorWhereIsLocatedFoundedKey].getDefinition(),		// definition
					this->possibleArguments[indexOfVectorWhereIsLocatedFoundedKey].isThisKeyRequired(),	// isRequiredKey
					nArgs																				// nArgs
					);
			this->realArguments.push_back(arg);
		}
		else
		{
			// If the user enter the wrong keys, we should throw an exception about it
			std::string ex ="Maybe you entered the wrong key. Double-check this parameter: ";
			ex.append(argv[i]);
			throw Exception(ex);
		}
	}

	// If the number of required parameters did not match, then the user did not enter all the required parameters and we throw an error with an explanation
	if (this->numberOfRequiredKeys != theNumberOfRequiredKeys)
	{
		throw Exception("Not all required parameters are entered");
	}
}

// If there are keys in the program that do not have a parameter and the programmer needs to process them, it is sufficient to use this function, which will tell you whether the key was entered by the user
bool ArgumentParser::isUsedTheKey(const std::string & key)
{
	// If there is no such key in realArguments, it means that this key has not been passed to the program
	uint index = getIndexFromVectorInWhichSearchKey(this->realArguments, key);
	if (index != -1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// This function takes a string with a key and looks for it among those entered by the user. If he passed parameters to this key - then we will display all these parameters in a vector
std::vector<std::string> ArgumentParser::getArgumentsByKey(const std::string & key)
{
	// First, we find the number of the vector with the key and already in it we take the vector with the arguments
	uint index = getIndexFromVectorInWhichSearchKey(this->realArguments, key);
	if (index != -1)
	{
		// Check if the given key has arguments
		if (realArguments[index].getNargs() > 0)
		{
			// If has - return the vector with arguments
			return realArguments[index].getArguments();
		}
		else
		{
			// If the key should have the arguments, but he hasn't - throw an exception
			std::string ex ="The key " + key + " doesn't have any arguments!"; 
			throw Exception(ex);
		}
	}
	else
	{
		// If such key was't founded in the realArguments vector
		std::string ex ="The key" + key + "has not be passed to the program!"; 
		throw Exception(ex);
	}
}

// If a programmer needs to print the definition of a given key (for example, for a help function), he can pass the key to this function and it will return a description
std::string ArgumentParser::getDefinitionByKey(const std::string & key)
{
	// Look for such key among the vector of realArguments, and if there is such a key, we return its definition
	uint index = getIndexFromVectorInWhichSearchKey(this->realArguments, key);
	if (index != -1)
	{
		// If key was founded - return description
		return this->realArguments[index].getDefinition();
	}
	else
	{
		// If not founded - throw an exception, that key has not be passed
		std::string ex ="The key" + key + "has not be passed to the program!"; 
		throw Exception(ex);
	}
}
