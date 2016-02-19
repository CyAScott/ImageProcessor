#ifndef THRESHOLDFILTER_H
#define THRESHOLDFILTER_H

#include "masterHeaders.h"
#include "rawImage.h"

class ThresholdFilter : public FilterHelper
{
	public:
		virtual RawImage* ProcessInput(CommandLineArgModel* arg, RawImage* image);
		virtual bool IsThisArgumentForThisFilter(string arg);
		virtual void ParseArgument(CommandLineArgModel* arg);
};

class ThresholdFilterModel : public CommandLineArgParsedModel
{
	public:
		byte Threshold;
};

#endif
