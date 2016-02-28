#ifndef HSIHISTOGRAMEQUALIZATION_H
#define HSIHISTOGRAMEQUALIZATION_H

#include "masterHeaders.h"
#include "rawImage.h"

class HsiHistogramEqualization : public FilterHelper
{
	public:
		virtual RawImage* ProcessInput(CommandLineArgModel* arg, RawImage* image);
		virtual bool IsThisArgumentForThisFilter(string arg);
		virtual void ParseArgument(CommandLineArgModel* arg);
};

class HsiHistogramEqualizationModel : public CommandLineArgParsedModel
{
	public:
		bool H;
		bool S;
		bool I;
};

#endif
