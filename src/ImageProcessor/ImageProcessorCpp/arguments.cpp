#include "arguments.h"

vector<CommandLineArgModel*> parse(unsigned int argc, char* argv[])
{
	struct CommandLineArgModel *currentArgument = NULL;
	vector<CommandLineArgModel*> arguments;
	vector<string> parameters;
	string arg;
	unsigned int argAttIndex, argAttLength = sizeof(ArgAttributes) / sizeof(ArgAttributes[0]), index;

	for (index = 1; index < argc; index++)
	{
		arg = argv[index];
		
		if (arg.length())
		{
			if (arg[0] == '-')
			{
				if (currentArgument != NULL)
				{
					currentArgument->Parameters = new vector<string>(parameters.begin(), parameters.end());

					arguments.push_back(currentArgument);
				}

				arg = arg.substr(1);

				for (argAttIndex = 0; argAttIndex < argAttLength; argAttIndex++)
				{
					if (isMatch(arg, ArgAttributes[argAttIndex].Arg))
					{
						break;
					}
				}

				if (argAttIndex == argAttLength)
				{
					cerr << "The argument \"";
					cerr << arg;
					cerr << "\" is not recognized.";
					throw exception();
				}

				parameters.clear();

				currentArgument = new CommandLineArgModel();
				currentArgument->Argument = ArgAttributes[argAttIndex];
			}
			else if (currentArgument == NULL)
			{
				cerr << "the first command line argument must be an argument (i.e. -Input)";
				throw exception();
			}
			else
			{
				parameters.push_back(arg);
			}
		}
	}

	if (currentArgument != NULL)
	{
		currentArgument->Parameters = new vector<string>(parameters.begin(), parameters.end());

		arguments.push_back(currentArgument);
	}
	
	return arguments;
}

bool validate(vector<CommandLineArgModel*> args)
{
	string descriptionText, documentation, temp;
	vector<CommandLineArgModel*> foundArgs;
	const int documentationWidth = 80;
	bool displayHelp = false, isOnlyOneAllowed, isRequired, matchFound;
	unsigned int argAttLength = sizeof(ArgAttributes) / sizeof(ArgAttributes[0]), 
		argIndex, foundArgIndex, foundParamIndex, index, paramIndex;
	
	for (argIndex = 0; argIndex < args.size(); argIndex++)
	{
		if (isMatch("help", args.at(argIndex)->Argument.Arg))
		{
			displayHelp = true;
			break;
		}
	}
	displayHelp = displayHelp && args.size() == 1;

	//Help Header
	if (displayHelp)
	{
		appendAndWrap(&documentation, "Displaying help info:", documentationWidth);
		documentation.append("\n");

		appendAndWrap(&documentation, "The general format for arguments is:", documentationWidth, "\t");
		documentation.append("\n");
		appendAndWrap(&documentation, "-Input <input file path> ... {pipe line arguments} ... -Output <output file path>", documentationWidth, "\t\t");
		documentation.append("\n");

		appendAndWrap(&documentation, 
			"Pipe line arguments are arguments where the order does matter. "
			"Each pipe line argument applies an filter on the image and applies it in the order in which is was sent to the program. "
			"The format for an argument and paramters is:", documentationWidth, "\t");
		documentation.append("\n");
		appendAndWrap(&documentation, "-{Argument} {Paramter 1} {Paramter 2} ... {Paramter N}", documentationWidth, "\t\t");
		documentation.append("\n");

		appendAndWrap(&documentation, "The following arguments can be used:", documentationWidth, "\t");
	}

	for (index = 0; index < argAttLength; index++)
	{
		for (argIndex = 0; argIndex < args.size(); argIndex++)
		{
			if (args.at(argIndex)->Argument.Arg == ArgAttributes[index].Arg)
			{
				foundArgs.push_back(args.at(argIndex));
			}
		}

		isRequired = ArgAttributes[index].Required;
		if (!displayHelp && isRequired && foundArgs.size() == 0)
		{
			cerr << "The -";
			cerr << ArgAttributes[index].Arg;
			cerr << " argument is required.";
			throw exception();
		}
		temp = "-";
		temp.append(ArgAttributes[index].Arg);
		temp.append(": ");
		temp.append(isRequired ? "Required" : "Optional");
		appendAndWrap(&documentation, temp, documentationWidth, "\t\t");
		documentation.append("\n");

		//Argument Attributes
		descriptionText = "";
		if (displayHelp && ArgAttributes[index].Description != "")
		{
			descriptionText.append(ArgAttributes[index].Description);
		}

		isOnlyOneAllowed = ArgAttributes[index].OnlyOne;
		if (displayHelp && isOnlyOneAllowed)
		{
			descriptionText.append("Multiple arguments of this type are not allowed. ");
		}
		if (!displayHelp && isOnlyOneAllowed && foundArgs.size() > 1)
		{
			cerr << "Only one -";
			cerr << ArgAttributes[index].Arg;
			cerr << " argument is allowed.";
			throw exception();
		}

		if (displayHelp && isOnlyOneAllowed)
		{
			if (ArgAttributes[index].ParamMin == ArgAttributes[index].ParamMax)
			{
				descriptionText.append("The argument requires exactly ");
				descriptionText.append(to_string(ArgAttributes[index].ParamMin));
				descriptionText.append(" paramter(s). ");
			}
			else
			{
				descriptionText.append("This argument requires a range of paramters of ");
				descriptionText.append(to_string(ArgAttributes[index].ParamMin));
				descriptionText.append(" to ");
				descriptionText.append(to_string(ArgAttributes[index].ParamMax));
				descriptionText.append(". ");
			}
		}

		for (foundArgIndex = 0; foundArgIndex < foundArgs.size(); foundArgIndex++)
		{
			unsigned int paramLength = foundArgs.at(foundArgIndex)->Parameters->size();

			if (!displayHelp &&
				(paramLength < (unsigned int)ArgAttributes[index].ParamMin || paramLength > (unsigned int)ArgAttributes[index].ParamMax))
			{
				cerr << ArgAttributes[index].ParamRangeError;
				throw exception();
			}

			for (foundParamIndex = 0; foundParamIndex < paramLength; foundParamIndex++)
			{
				matchFound = false;
				for (paramIndex = 0; paramIndex < ArgAttributes[index].Params.size() && !matchFound; paramIndex++)
				{
					if (!ArgAttributes[index].Params.at(paramIndex).IsCaseSensitive)
					{
						foundArgs.at(foundArgIndex)->Parameters->at(foundParamIndex) = toLowerString(foundArgs.at(foundArgIndex)->Parameters->at(foundParamIndex));
					}

					if (ArgAttributes[index].Params.at(paramIndex).Pattern == ".*" ||
						regex_match(
							foundArgs.at(foundArgIndex)->Parameters->at(foundParamIndex),
							regex(ArgAttributes[index].Params.at(paramIndex).Pattern)))
					{
						matchFound = true;
					}
				}
				if (!matchFound)
				{
					cerr << "The -";
					cerr << ArgAttributes[index].Arg;
					cerr << " argument paramters are invalid.";
					throw exception();
				}
			}
		}

		if (!displayHelp &&
			isMatch("input", ArgAttributes[index].Arg) &&
			!fileExists(foundArgs.at(0)->Parameters->at(0)))
		{
			cerr << "The input file was not found.";
			throw exception();
		}

		if (descriptionText.length() > 0)
		{
			appendAndWrap(&documentation, descriptionText, documentationWidth, "\t\t\t");
			documentation.append("\n");
		}

		//Argument Attributes
		if (displayHelp && ArgAttributes[index].Params.size() > 0)
		{
			appendAndWrap(&documentation, "Argument Parameter(s):", documentationWidth, "\t\t\t");

			for (paramIndex = 0; paramIndex < ArgAttributes[index].Params.size(); paramIndex++)
			{
				temp = "Format: ";
				temp.append(ArgAttributes[index].Params.at(paramIndex).Format);
				appendAndWrap(&documentation, temp, documentationWidth, "\t\t\t\t");
				
				temp = "Optional: ";
				temp.append(ArgAttributes[index].Params.at(paramIndex).Optional ? "true" : "false");
				appendAndWrap(&documentation, "Optional: " + ArgAttributes[index].Params.at(paramIndex).Optional, documentationWidth, "\t\t\t\t");
				
				appendAndWrap(&documentation, "Description:", documentationWidth, "\t\t\t\t");
				appendAndWrap(&documentation, ArgAttributes[index].Params.at(paramIndex).Description, documentationWidth, "\t\t\t\t\t");
				documentation.append("\n");
			}
		}
		documentation.append("\n");

		foundArgs.clear();
	}

	if (args.size() == 0)
	{
		cerr << "No arguments were provided.";
		throw exception();
	}
	
	if (displayHelp)
	{
		cout << documentation;
		return false;
	}

	return true;
}