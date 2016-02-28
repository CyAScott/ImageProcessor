#ifndef GRAYHISTOGRAMEQUALIZATION_H
#define GRAYHISTOGRAMEQUALIZATION_H

#include "masterHeaders.h"
#include "rawImage.h"

class GrayHistogramEqualization : public FilterHelper
{
	public:
		virtual RawImage* ProcessInput(CommandLineArgModel* arg, RawImage* image);
		virtual bool IsThisArgumentForThisFilter(string arg);
		virtual void ParseArgument(CommandLineArgModel* arg);
};

class GrayHistogramEqualizationModel : public CommandLineArgParsedModel
{
};

#endif

