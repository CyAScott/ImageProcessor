#ifndef DISPARITYMAP_H
#define DISPARITYMAP_H

#include "masterHeaders.h"
#include "rawImage.h"

class DisparityMap : public FilterHelper
{
	public:
		virtual RawImage* ProcessInput(CommandLineArgModel* arg, RawImage* image);
		virtual bool IsThisArgumentForThisFilter(string arg);
		virtual void ParseArgument(CommandLineArgModel* arg);
};

class DisparityMapModel : public CommandLineArgParsedModel
{
	public:
		ColorSpaceEnum ColorSpace;
		bool ColorChannel1;
		bool ColorChannel2;
		bool ColorChannel3;
		int Direction;
		int Threshold;
		int Window;
};

#endif