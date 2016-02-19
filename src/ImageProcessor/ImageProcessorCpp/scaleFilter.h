#ifndef SCALEFILTER_H
#define SCALEFILTER_H

#include "masterHeaders.h"
#include "rawImage.h"
#include "medianFilter.h"

class ScaleFilter : public FilterHelper
{
	private:
		MedianFilter* median = new MedianFilter();
		RawImage* scaleDown(double scale, RawImage* image);
		RawImage* scaleUp(double scale, RawImage* image);

	public:
		virtual RawImage* ProcessInput(CommandLineArgModel* arg, RawImage* image);
		virtual bool IsThisArgumentForThisFilter(string arg);
		virtual void ParseArgument(CommandLineArgModel* arg);
};

class ScaleFilterModel : public CommandLineArgParsedModel
{
	public:
		double Scale;
};

#endif
