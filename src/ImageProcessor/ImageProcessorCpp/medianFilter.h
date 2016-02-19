#ifndef MEDIANFILTER_H
#define MEDIANFILTER_H

#include "masterHeaders.h"
#include "rawImage.h"

class MedianFilter : public FilterHelper
{
	public:
		virtual RawImage* ProcessInput(CommandLineArgModel* arg, RawImage* image);
		virtual bool IsThisArgumentForThisFilter(string arg);
		virtual void ParseArgument(CommandLineArgModel* arg);
};

class MedianFilterModel : public CommandLineArgParsedModel
{
	public:
		bool X;
		bool Y;
		int Window;
};

#endif
