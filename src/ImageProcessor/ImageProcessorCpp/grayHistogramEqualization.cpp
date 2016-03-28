#include "grayHistogramEqualization.h"

RawImage* GrayHistogramEqualization::ProcessInput(CommandLineArgModel* arg, RawImage* image)
{
	GrayHistogramEqualizationModel* model = (GrayHistogramEqualizationModel*)arg->ParsedModel;
	Rectangle roi = model->Roi;
	vector<byte> histogramPixelMap;
	vector<int> histogram = image->GetHistogram(roi).I;
	double cumulativeProbabilityValue = 0.0, 
		totalNumberOfPixels = roi.Height * roi.Width;
	
	for (unsigned int i = 0; i < histogram.size(); i++)
	{
		double probability = (double)histogram.at(i) / totalNumberOfPixels;
		cumulativeProbabilityValue += probability;

		histogramPixelMap.push_back(roundToByte(floor(cumulativeProbabilityValue * 255.0)));
	}

	for (int y = roi.Y; y < roi.Bottom; y++)
		for (int x = roi.X; x < roi.Right; x++)
		{
			byte value = image->GetBrightness(x, y);

			image->SetPixel(x, y, histogramPixelMap.at(value));
		}

	return image;
}
bool GrayHistogramEqualization::IsThisArgumentForThisFilter(string arg)
{
	return isMatch("GrayHistogramEqualization", arg);
}
void GrayHistogramEqualization::ParseArgument(CommandLineArgModel* arg)
{
	GrayHistogramEqualizationModel* model = new GrayHistogramEqualizationModel();
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
