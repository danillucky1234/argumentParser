#include "stdafx.h"
#include "exception.h"
#include "argument.h"
#include "argumentParser.h"

int main(int argc, char ** argv)
{
	ArgumentParser argParser;

	// Add first argument
	argParser.addArgument(Argument(
			{ "-r", "--resolution" },												// key variations
			"This key can help you to set resolution according to your monitor",	// definition
			true,																	// is required key
			2																		// number of arguments this key can hold
			)
	);

	// Second arg
	argParser.addArgument(Argument(
			{ "-h", "--help" },					// key variations
			"Help message and exit",			// definition
			false								// is required key
			)
	);
	
	// Third arg
	argParser.addArgument(Argument (
			{ "-c", "--cipher" },					// key variations
			"Choose what cipher to use",			// definition
			false,									// is required key
			1,										// nArgs
			{ "ctc", "caesar", "hill", "rsa" }		// possible arguments
			)
	);

	try
	{
		// Try to parse arguments
		// If everything is okay - fine,
		// else - throw exception
		argParser.parseArgs(argc, argv);
	}
	catch(const std::exception & ex)
	{
		std::cout << ex.what() << '\n';	
	}
	
	if (argParser.isUsedTheKey("-h"))
	{
		std::cout << "The key '-h' was used!\n";
	}
	else
	{
		std::cout << "The key '-h' was NOT used!\n";
	}

	if (argParser.isUsedTheKey("-r"))
	{
		std::cout << "The key '-r' was used!\n";
		auto rArgs = argParser.getArgumentsByKey("-r");
		std::cout << "-r arguments:\n";
		for (auto & x : rArgs)
		{
			std::cout << x << '\n';
		}
	}
	else
	{
		std::cout << "The key '-r' was NOT used!\n";
	}

	if (argParser.isUsedTheKey("--cipher"))
	{
		std::cout << "The key '--cipher' was used!\n";
		std::vector<std::string> rArgs = argParser.getArgumentsByKey("--cipher");
		std::cout << "-c arguments:\n";
		for (auto & x : rArgs)
		{
			std::cout << x << '\n';
		}
	}
	else
	{
		std::cout << "The key '-c' was NOT used!\n";
	}
	
	return 0;
}
