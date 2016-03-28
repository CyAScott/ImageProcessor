#include "histogramStretch.h"

int indexOf(vector<int> source, int value)
{
	for (unsigned int i = 0; i < source.size(); i++)
	{
		if (source.at(i) == value)
		{
			return i;
		}
	}
	return -1;
}

int lastIndexOf(vector<int> source, int value)
{
	for (unsigned int i = source.size() - 1; i >= 0; i--)
	{
		if (source.at(i) == value)
		{
			return i;
		}
	}
	return -1;
}

RawImage* HistogramStretch::ProcessInput(CommandLineArgModel* arg, RawImage* image)
{
	HistogramStretchModel* model = (HistogramStretchModel*)arg->ParsedModel;
	Rectangle roi = model->Roi;
	double lower = floor(model->Percentile * 255.0);
	double upper = 255.0 - lower;

	double coefficient = 255.0 / (double)(upper - lower);

	for (int y = roi.Y; y < roi.Bottom; y++)
		for (int x = roi.X; x < roi.Right; x++)
		{
			Color color = image->GetPixel(x, y);

			color.R = roundToByte(((double)color.R - lower) * coefficient);
			color.B = roundToByte(((double)color.B - lower) * coefficient);
			color.G = roundToByte(((double)color.G - lower) * coefficient);

			image->SetPixel(x, y, color);
		}

	return image;
}
bool HistogramStretch::IsThisArgumentForThisFilter(string arg)
{
	return isMatch("HistogramStretch", arg);
}
void HistogramStretch::ParseArgument(CommandLineArgModel* arg)
{
	HistogramStretchModel* model = new HistogramStretchModel();
	bool hasPercentile = false;
	model->hasRoi = false;
	model->Percentile = 0.05;

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

		if (!hasPercentile)
		{
			string value = getParamValue("percentile", arg->Parameters->at(index));

			if (value.length())
			{
				hasPercentile = true;
				model->Percentile = stod(trim(value));
				if (model->Percentile > 1.0)
				{
					model->Percentile = 1.0;
				}
				else if (model->Percentile < 0.0)
				{
					model->Percentile = 0.0;
				}
			}
		}
	}

	arg->ParsedModel = model;
}

