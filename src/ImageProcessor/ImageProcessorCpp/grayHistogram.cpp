#include "grayHistogram.h"

RawImage* GrayHistogram::ProcessInput(CommandLineArgModel* arg, RawImage* image)
{
	GrayHistogramModel* model = (GrayHistogramModel*)arg->ParsedModel;
	Rectangle roi = model->Roi;
	vector<int> histogram = image->GetHistogram(roi).I;
	double maxCount = 255.0;

	for (unsigned int i = 0; i < histogram.size(); i++)
		if (histogram.at(i) > maxCount)
			maxCount = histogram.at(i);

	RawImage* returnValue = new RawImage(256, 256);
	double scale = 255.0 / maxCount;

	for (int y = 0; y < 256; y++)
		for (int x = 0; x < 256; x++)
			returnValue->SetPixel(x, y, 255 - round(scale * histogram.at(x)) > y ? 255 : 0);

	return returnValue;
}
bool GrayHistogram::IsThisArgumentForThisFilter(string arg)
{
	return isMatch("GrayHistogram", arg);
}
void GrayHistogram::ParseArgument(CommandLineArgModel* arg)
{
	GrayHistogramModel* model = new GrayHistogramModel();
	model->hasRoi = false;

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
	}

	arg->ParsedModel = model;
}
