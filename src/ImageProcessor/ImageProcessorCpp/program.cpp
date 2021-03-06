#include "arguments.h"
#include "masterHeaders.h"
#include "grayscaleFilter.h"
#include "medianFilter.h"
#include "scaleFilter.h"
#include "thresholdFilter.h"
#include "filterPipe.h"
#include "rawImage.h"

int main(int argc, char* argv[])
{
	try
	{
		cout << "Starting image processor.\n\n";

		testRegExPatterns();

		vector<CommandLineArgModel*> args = parse(argc, argv);
		
		if (!validate(args))
		{
			return EXIT_SUCCESS;
		}

		parseArguments(args);

		string input = "", inputTemp = "", output = "", outputTemp = "";
		for (unsigned int index = 0; index < args.size(); index++)
		{
			if (isMatch("input", args.at(index)->Argument.Arg))
			{
				input = args.at(index)->Parameters->at(0);
				inputTemp = input + ".temp.ppm";
			}
			if (isMatch("output", args.at(index)->Argument.Arg))
			{
				output = args.at(index)->Parameters->at(0);
				outputTemp = output + ".temp.ppm";
			}
		}

		string convertForReadCommand = "convert \"";
		convertForReadCommand.append(input);
		convertForReadCommand.append("\" -compress none \"");
		convertForReadCommand.append(inputTemp);
		convertForReadCommand.append("\"");
		cout << convertForReadCommand << "\n\n";
		if (system(convertForReadCommand.c_str()))
		{
			cerr << "Failed to read the file.";
			throw exception();
		}
		
		process(args, new RawImage(inputTemp))->Save(outputTemp);
		
		string convertForWriteCommand = "convert \"";
		convertForWriteCommand.append(outputTemp);
		convertForWriteCommand.append("\" \"");
		convertForWriteCommand.append(output);
		convertForWriteCommand.append("\"");
		cout << "\n" << convertForWriteCommand << "\n\n";
		if (system(convertForWriteCommand.c_str()))
		{
			cerr << "Failed to write the file.";
			throw exception();
		}
		remove(outputTemp.c_str());

		return EXIT_SUCCESS;
	}
	catch (...)
	{
		cout << "\n";
		return EXIT_FAILURE;
	}
}