#include "hsiHistogramEqualization.h"

RawImage* HsiHistogramEqualization::ProcessInput(CommandLineArgModel* arg, RawImage* image)
{
	HsiHistogramEqualizationModel* model = (HsiHistogramEqualizationModel*)arg->ParsedModel;
	Rectangle roi = model->Roi;
	double
		cumulativeProbabilityValue,
		probability,
		totalNumberOfPixels = roi.Height * roi.Width;

	map<double, int> hHistogram;
	map<double, int> sHistogram;
	map<int, int> iHistogram;

	map<double, double> hHistogramMap;
	map<double, double> sHistogramMap;
	map<int, int> iHistogramMap;

	HsiImage hsi = image->ExportHsi(roi);

	for (int y = 0; y < roi.Height; y++)
	{
		for (int x = 0; x < roi.Width; x++)
		{
			int i = hsi.I.at(y).at(x);
			double s = hsi.S.at(y).at(x);
			double h = hsi.H.at(y).at(x);

			if (model->H)
			{
				map<double, int>::iterator hHistogramIt = hHistogram.find(h);
				if (hHistogramIt == hHistogram.end())
				{
					hHistogram.insert(make_pair(h, 1));
				}
				else
				{
					hHistogramIt->second++;
				}
			}
			if (model->S)
			{
				map<double, int>::iterator sHistogramIt = sHistogram.find(s);
				if (sHistogramIt == sHistogram.end())
				{
					sHistogram.insert(make_pair(s, 1));
				}
				else
				{
					sHistogramIt->second++;
				}
			}
			if (model->I)
			{
				map<int, int>::iterator iHistogramIt = iHistogram.find(i);
				if (iHistogramIt == iHistogram.end())
				{
					iHistogram.insert(make_pair(i, 1));
				}
				else
				{
					iHistogramIt->second++;
				}
			}
		}
	}

	if (model->H)
	{
		cumulativeProbabilityValue = 0.0;
		for (map<double, int>::iterator it = hHistogram.begin(); it != hHistogram.end(); ++it)
		{
			probability = (double)it->second / totalNumberOfPixels;
			cumulativeProbabilityValue += probability;
			hHistogramMap[it->first] = cumulativeProbabilityValue * 360.0;
		}
	}

	if (model->S)
	{
		cumulativeProbabilityValue = 0.0;
		for (map<double, int>::iterator it = sHistogram.begin(); it != sHistogram.end(); ++it)
		{
			probability = (double)it->second / totalNumberOfPixels;
			cumulativeProbabilityValue += probability;
			sHistogramMap[it->first] = cumulativeProbabilityValue * 1.0;
		}
	}

	if (model->I)
	{
		cumulativeProbabilityValue = 0.0;
		for (map<int, int>::iterator it = iHistogram.begin(); it != iHistogram.end(); ++it)
		{
			probability = (double)it->second / totalNumberOfPixels;
			cumulativeProbabilityValue += probability;
			iHistogramMap[it->first] = roundToByte(cumulativeProbabilityValue * 255.0);
		}
	}

	for (int y = 0; y < roi.Height; y++)
	{
		for (int x = 0; x < roi.Width; x++)
		{
			if (model->H)
			{
				hsi.H.at(y).at(x) = hHistogramMap[hsi.H.at(y).at(x)];
			}
			if (model->S)
			{
				hsi.S.at(y).at(x) = sHistogramMap[hsi.S.at(y).at(x)];
			}
			if (model->I)
			{
				hsi.I.at(y).at(x) = iHistogramMap[hsi.I.at(y).at(x)];
			}
		}
	}

	image->ApplyHsi(roi, hsi);

	return image;
}
bool HsiHistogramEqualization::IsThisArgumentForThisFilter(string arg)
{
	return isMatch("HsiHistogramEqualization", arg);
}
void HsiHistogramEqualization::ParseArgument(CommandLineArgModel* arg)
{
	HsiHistogramEqualizationModel* model = new HsiHistogramEqualizationModel();
	model->hasRoi = false;
	model->H = false;
	model->S = true;
	model->I = true;
	bool hasH = false, hasS = false, hasI = false;

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

		if (!hasH)
		{
			string value = getParamValue("h", arg->Parameters->at(index));

			if (value.length())
			{
				hasH = true;
				model->H = isMatch(value, "true");
			}
		}

		if (!hasS)
		{
			string value = getParamValue("s", arg->Parameters->at(index));

			if (value.length())
			{
				hasS = true;
				model->S = isMatch(value, "true");
			}
		}

		if (!hasI)
		{
			string value = getParamValue("i", arg->Parameters->at(index));

			if (value.length())
			{
				hasI = true;
				model->I = isMatch(value, "true");
			}
		}
	}

	arg->ParsedModel = model;
}