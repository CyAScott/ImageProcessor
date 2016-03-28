#include "histogramEqualization.h"

RawImage* HistogramEqualization::ProcessInput(CommandLineArgModel* arg, RawImage* image)
{
	HistogramEqualizationModel* model = (HistogramEqualizationModel*)arg->ParsedModel;
	Rectangle roi = model->Roi;
	HistogramResult histogram = image->GetHistogram(roi);
	vector<byte> histogramPixelMapR, histogramPixelMapG, histogramPixelMapB;
	double 
		cumulativeProbabilityValueR = 0.0,
		cumulativeProbabilityValueG = 0.0,
		cumulativeProbabilityValueB = 0.0,
		probability,
		totalNumberOfPixels = roi.Height * roi.Width;

	for (int i = 0; i < 256; i++)
	{
		probability = (double)histogram.R.at(i) / totalNumberOfPixels;
		cumulativeProbabilityValueR += probability;
		histogramPixelMapR.push_back(roundToByte(floor(cumulativeProbabilityValueR * 255.0)));

		probability = (double)histogram.G.at(i) / totalNumberOfPixels;
		cumulativeProbabilityValueG += probability;
		histogramPixelMapG.push_back(roundToByte(floor(cumulativeProbabilityValueG * 255.0)));

		probability = (double)histogram.B.at(i) / totalNumberOfPixels;
		cumulativeProbabilityValueB += probability;
		histogramPixelMapB.push_back(roundToByte(floor(cumulativeProbabilityValueB * 255.0)));
	}

	for (int y = roi.Y; y < roi.Bottom; y++)
		for (int x = roi.X; x < roi.Right; x++)
		{
			Color color = image->GetPixel(x, y);

			image->SetPixel(x, y, 
				histogramPixelMapR.at(color.R),
				histogramPixelMapG.at(color.G),
				histogramPixelMapB.at(color.B));
		}

	return image;
}
bool HistogramEqualization::IsThisArgumentForThisFilter(string arg)
{
	return isMatch("HistogramEqualization", arg);
}
void HistogramEqualization::ParseArgument(CommandLineArgModel* arg)
{
	HistogramEqualizationModel* model = new HistogramEqualizationModel();
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

