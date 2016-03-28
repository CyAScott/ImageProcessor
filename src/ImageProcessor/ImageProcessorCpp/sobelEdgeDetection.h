#ifndef SOBELEDGEDETECTION_H
#define SOBELEDGEDETECTION_H

#include "masterHeaders.h"
#include "rawImage.h"

class SobelEdgeDetection : public FilterHelper
{
	public:
		virtual RawImage* ProcessInput(CommandLineArgModel* arg, RawImage* image);
		virtual bool IsThisArgumentForThisFilter(string arg);
		virtual void ParseArgument(CommandLineArgModel* arg);
};

class SobelEdgeDetectionModel : public CommandLineArgParsedModel
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
