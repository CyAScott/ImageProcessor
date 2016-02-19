#ifndef GRAYSCALEFILTER_H
#define GRAYSCALEFILTER_H

#include "masterHeaders.h"
#include "rawImage.h"

class GrayscaleFilter : public FilterHelper
{
	public:
		virtual RawImage* ProcessInput(CommandLineArgModel* arg, RawImage* image);
		virtual bool IsThisArgumentForThisFilter(string arg);
		virtual void ParseArgument(CommandLineArgModel* arg);
};

class GrayscaleFilterModel : public CommandLineArgParsedModel
{
};

#endif
