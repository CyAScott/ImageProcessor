#ifndef OPENCVYCRCBHISTOGRAMEQUALIZATION_H
#define OPENCVYCRCBHISTOGRAMEQUALIZATION_H

#include "masterHeaders.h"
#include "rawImage.h"

class OpenCvYCrCbHistogramEqualization : public FilterHelper
{
	public:
		virtual RawImage* ProcessInput(CommandLineArgModel* arg, RawImage* image);
		virtual bool IsThisArgumentForThisFilter(string arg);
		virtual void ParseArgument(CommandLineArgModel* arg);
};

class OpenCvYCrCbHistogramEqualizationModel : public CommandLineArgParsedModel
{
	public:
		bool Y;
		bool Cr;
		bool Cb;
};

#endif
