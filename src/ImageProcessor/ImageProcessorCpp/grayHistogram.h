#ifndef GRAYHISTOGRAM_H
#define GRAYHISTOGRAM_H

#include "masterHeaders.h"
#include "rawImage.h"

class GrayHistogram : public FilterHelper
{
	public:
		virtual RawImage* ProcessInput(CommandLineArgModel* arg, RawImage* image);
		virtual bool IsThisArgumentForThisFilter(string arg);
		virtual void ParseArgument(CommandLineArgModel* arg);
};

class GrayHistogramModel : public CommandLineArgParsedModel
{
};

#endif