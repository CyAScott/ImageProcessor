#ifndef HISTOGRAMSTRETCH_H
#define HISTOGRAMSTRETCH_H

#include "masterHeaders.h"
#include "rawImage.h"

class HistogramStretch : public FilterHelper
{
	public:
		virtual RawImage* ProcessInput(CommandLineArgModel* arg, RawImage* image);
		virtual bool IsThisArgumentForThisFilter(string arg);
		virtual void ParseArgument(CommandLineArgModel* arg);
};

class HistogramStretchModel : public CommandLineArgParsedModel
{
	public:
		double Percentile;
};

#endif


