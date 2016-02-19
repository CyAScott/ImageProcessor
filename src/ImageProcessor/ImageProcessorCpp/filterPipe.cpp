#include "filterPipe.h"
#include "grayscaleFilter.h"
#include "medianFilter.h"
#include "scaleFilter.h"
#include "thresholdFilter.h"

vector<FilterHelper*> filters =
{
	new GrayscaleFilter(),
	new MedianFilter(),
	new ScaleFilter(),
	new ThresholdFilter()
};

RawImage* process(vector<CommandLineArgModel*> args, RawImage* image)
{
	unsigned int filterIndex, index, paramIndex;
	
	for (index = 0; index < args.size(); index++)
	{
		if (args.at(index)->Argument.IsFilter)
		{
			cout << "Processing: -" << args.at(index)->Argument.Arg << " ";
			for (paramIndex = 0; paramIndex < args.at(index)->Parameters->size(); paramIndex++)
			{
				if (paramIndex)
				{
					cout << " ";
				}
				cout << "\"" << args.at(index)->Parameters->at(paramIndex) << "\"";
			}
			cout << "\n";

			for (filterIndex = 0; filterIndex < filters.size(); filterIndex++)
			{
				if (filters.at(filterIndex)->IsThisArgumentForThisFilter(args.at(index)->Argument.Arg))
				{
					if (args.at(index)->Argument.AllowRoi &&
						!args.at(index)->ParsedModel->hasRoi)
					{
						Rectangle roi = args.at(index)->ParsedModel->Roi = fromSize(image->GetSize());

						cout << "Roi: {x: " << roi.X << ", y: " << roi.Y << ", width: " << roi.Width << ", height: " << roi.Height << "}\n";
					}
					image = filters.at(filterIndex)->ProcessInput(args.at(index), image);
					break;
				}
			}
		}
	}

	return image;
}

void parseArguments(vector<CommandLineArgModel*> args)
{
	unsigned int filterIndex, index;

	for (index = 0; index < args.size(); index++)
	{
		for (filterIndex = 0; filterIndex < filters.size(); filterIndex++)
		{
			if (filters.at(filterIndex)->IsThisArgumentForThisFilter(args.at(index)->Argument.Arg))
			{
				filters.at(filterIndex)->ParseArgument(args.at(index));
				break;
			}
		}
	}
}
