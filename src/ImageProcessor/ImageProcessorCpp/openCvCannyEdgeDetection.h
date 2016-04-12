#ifndef OPENCVCANNYEDGEDETECTION_H
#define OPENCVCANNYEDGEDETECTION_H

#include "masterHeaders.h"
#include "rawImage.h"

class OpenCvCannyEdgeDetection : public FilterHelper
{
	public:
		virtual RawImage* ProcessInput(CommandLineArgModel* arg, RawImage* image);
		virtual bool IsThisArgumentForThisFilter(string arg);
		virtual void ParseArgument(CommandLineArgModel* arg);
};

class OpenCvCannyEdgeDetectionModel : public CommandLineArgParsedModel
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
