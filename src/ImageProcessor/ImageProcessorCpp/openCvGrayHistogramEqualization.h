#ifndef OPENCVGRAYHISTOGRAMEQUALIZATION_H
#define OPENCVGRAYHISTOGRAMEQUALIZATION_H

#include "masterHeaders.h"
#include "rawImage.h"

class OpenCvGrayHistogramEqualization : public FilterHelper
{
	public:
		virtual RawImage* ProcessInput(CommandLineArgModel* arg, RawImage* image);
		virtual bool IsThisArgumentForThisFilter(string arg);
		virtual void ParseArgument(CommandLineArgModel* arg);
};

class OpenCvGrayHistogramEqualizationModel : public CommandLineArgParsedModel
{
};

#endif