#include "thresholdFilter.h"

RawImage* ThresholdFilter::ProcessInput(CommandLineArgModel* arg, RawImage* image)
{
	ThresholdFilterModel* model = (ThresholdFilterModel*)arg->ParsedModel;
	Rectangle roi = model->Roi;
	double threshold = model->Threshold;

	for (int y = roi.Y; y < roi.Bottom; y++)
		for (int x = roi.X; x < roi.Right; x++)
			image->SetPixel(x, y, image->ThresholdFilter(x, y, roundToByte(threshold)));

	return image;
}
bool ThresholdFilter::IsThisArgumentForThisFilter(string arg)
{
	return isMatch("ThresholdFilter", arg);
}
void ThresholdFilter::ParseArgument(CommandLineArgModel* arg)
{
	ThresholdFilterModel* model = new ThresholdFilterModel();
	model->hasRoi = false;
	model->Threshold = 128;

	bool hasThreshold = false;

	for (unsigned int index = 0; index < arg->Parameters->size(); index++)
	{
		if (!model->hasRoi)
		{
			Rectangle roi = parseRoi(arg->Parameters->at(index));
			if (roi.Bottom != -1)
			{
				model->hasRoi = true;
				model->Roi = roi;
			}
		}

		if (!hasThreshold)
		{
			string value = getParamValue("threshold", arg->Parameters->at(index));

			if (value.length())
			{
				hasThreshold = true;
				model->Threshold = roundToByte(stod(trim(value)) * 255);
			}
		}
	}
	
	arg->ParsedModel = model;
}
