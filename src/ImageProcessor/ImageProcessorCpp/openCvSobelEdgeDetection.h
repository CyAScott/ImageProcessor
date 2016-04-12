#ifndef OPENCVSOBELEDGEDETECTION_H
#define OPENCVSOBELEDGEDETECTION_H

#include "masterHeaders.h"
#include "rawImage.h"

class OpenCvSobelEdgeDetection : public FilterHelper
{
	public:
		virtual RawImage* ProcessInput(CommandLineArgModel* arg, RawImage* image);
		virtual bool IsThisArgumentForThisFilter(string arg);
		virtual void ParseArgument(CommandLineArgModel* arg);
};

class OpenCvSobelEdgeDetectionModel : public CommandLineArgParsedModel
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
