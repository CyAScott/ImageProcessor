#ifndef OPENCVHISTOGRAMEQUALIZATION_H
#define OPENCVHISTOGRAMEQUALIZATION_H

#include "masterHeaders.h"
#include "rawImage.h"

class OpenCvHistogramEqualization : public FilterHelper
{
	public:
		virtual RawImage* ProcessInput(CommandLineArgModel* arg, RawImage* image);
		virtual bool IsThisArgumentForThisFilter(string arg);
		virtual void ParseArgument(CommandLineArgModel* arg);
};

class OpenCvHistogramEqualizationModel : public CommandLineArgParsedModel
{
};

#endif
