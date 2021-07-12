### What is this project?
The argument parser is the program, which can help you to don't write a whole bunch of code and can do all the neccessary checks for you. [Usage below](#usage)

#### So what exactly this parser do?  
First of all, the programmer that use this parser should add some possible arguments that can be used in the program.  
After that programmer pass `argc` and `argv` into parser and then parser make all required checks (Were all the required arguments used? Were all the parameters correctly entered? etc)

Then the programmer can do checks (Did the user enter such key?), get all the arguments that were passed to the key, and it is also possible to get the definition of the key (for example, if you make a help function, which explains why you need a particular key)

## How to install this parser?
1. Go to work directory and if you haven't yet initialized a git repo, use the command: `git init`
2. After initialize add this repo as submodule:  
`git submodule add git@github.com:danillucky1234/argumentParser.git`
3. Check `git status` and if you see two new unstaged files: `.gitmodules` and `argumentParser`, it means that everything is okay.
4. Commit the changes:  
`git add .gitmodules argumentPraser`  
`git commit -m "Added argumentParser submodule"`
5. Add these three files to your precompiled header file:
```C++
#include "argumentParser/argument.h"
#include "argumentParser/argumentParser.h"
#include "argumentParser/exception.h"
```

<a name = "usage"></a>
### Example usage
Link to [the example](https://github.com/danillucky1234/argumentParser/blob/main/example.cpp):
```C++
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
		// else - function throws exception
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
```
