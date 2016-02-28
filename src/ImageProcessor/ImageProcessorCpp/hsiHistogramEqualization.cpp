#include "hsiHistogramEqualization.h"
# define PI           3.14159265358979323846  /* pi */

RawImage* HsiHistogramEqualization::ProcessInput(CommandLineArgModel* arg, RawImage* image)
{
	HsiHistogramEqualizationModel* model = (HsiHistogramEqualizationModel*)arg->ParsedModel;
	Rectangle roi = model->Roi;
	HistogramResult histogram = image->GetHistogram(roi);
	double
		cumulativeProbabilityValue,
		probability,
		totalNumberOfPixels = roi.Height * roi.Width;

	vector<vector<double>> H;
	vector<vector<double>> S;
	vector<vector<int>> I;

	map<double, int> hHistogram;
	map<double, int> sHistogram;
	map<int, int> iHistogram;

	map<double, double> hHistogramMap;
	map<double, double> sHistogramMap;
	map<int, int> iHistogramMap;

	for (int y = roi.Y; y < roi.Bottom; y++)
	{
		vector<double> hRow;
		vector<double> sRow;
		vector<int> iRow;

		for (int x = roi.X; x < roi.Right; x++)
		{
			Color color = image->GetPixel(x, y);
			double r = color.R;
			double b = color.B;
			double g = color.G;

			int i = (r + b + g) / 3;
			
			double m = min(min(r, b), g);
			double s = i == 0 ? 0.0 : 1.0 - m / (double)i;

			double h = 0.0;
			if (r - g == 0.0 && r - b == 0.0)
			{
				h = 0.0;
			}
			else
			{
				if (g - b >= 0.0)
				{
					h = acos((r - 0.5 * g - 0.5 * b) / sqrt(r * r + g * g + b * b - r * g - r * b - g * b)) * 180.0 / PI;
				}
				if (g - b < 0.0)
				{
					h = 360.0 - acos((r - 0.5 * g - 0.5 * b) / sqrt(r * r + g * g + b * b - r * g - r * b - g * b)) * 180.0 / PI;
				}
			}

			hRow.push_back(h);
			sRow.push_back(s);
			iRow.push_back(i);

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

		H.push_back(hRow);
		S.push_back(sRow);
		I.push_back(iRow);
	}


	if (model->H)
	{
		cumulativeProbabilityValue = 0.0;
		for (map<double, int>::iterator it = hHistogram.begin(); it != hHistogram.end(); ++it)
		{
			probability = (double)it->second / totalNumberOfPixels;
			cumulativeProbabilityValue += probability;
			hHistogramMap[it->first] = cumulativeProbabilityValue * 0;
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
			iHistogramMap[it->first] = min(255.0, max(0.0, round(cumulativeProbabilityValue * 255.0)));
		}
	}

	for (int y = roi.Y; y < roi.Bottom; y++)
		for (int x = roi.X; x < roi.Right; x++)
		{
			Color color;
			double hh = model->H ? hHistogramMap[H.at(y).at(x)] : H.at(y).at(x);
			double ss = model->S ? sHistogramMap[S.at(y).at(x)] : S.at(y).at(x);
			double ii = model->I ? iHistogramMap[I.at(y).at(x)] : I.at(y).at(x);

			if (hh == 0)
			{
				color.B = min(255.0, max(0.0, round(ii*(1 - ss))));
				color.G = min(255.0, max(0.0, round(ii*(1 - ss))));
				color.R = min(255.0, max(0.0, round(ii*(1 + 2 * ss))));
			}
			if (hh > 0.0 && hh < 120.0)
			{
				color.B = min(255.0, max(0.0, round(ii*(1 - ss))));
				color.R = min(255.0, max(0.0, round(ii*(1 + ss*cos((hh / 180)*PI) / cos(PI*(60 - hh) / 180)))));
				color.G = min(255.0, max(0.0, round(3 * ii - (double)color.R - (double)color.B)));
			}
			if (hh == 120.0)
			{
				color.B = min(255.0, max(0.0, round(ii*(1 - ss))));
				color.G = min(255.0, max(0.0, round(ii*(1 + 2 * ss))));
				color.R = min(255.0, max(0.0, round(ii*(1 - ss))));
			}
			if (hh > 120.0 && hh < 240.0)
			{
				color.R = min(255.0, max(0.0, round(ii*(1 - ss))));
				color.G = min(255.0, max(0.0, round(ii*(1 + ss*cos(PI*(hh - 120) / 180) / cos(PI*(180 - hh) / 180)))));
				color.B = min(255.0, max(0.0, round(3 * ii - (double)color.R - (double)color.G)));
			}
			if (hh == 240.0)
			{
				color.B = min(255.0, max(0.0, round(ii*(1 + 2 * ss))));
				color.G = min(255.0, max(0.0, round(ii*(1 - ss))));
				color.R = min(255.0, max(0.0, round(ii*(1 - ss))));
			}
			if (hh > 240.0 && hh <= 360.0)
			{
				color.G = min(255.0, max(0.0, round(ii*(1 - ss))));
				color.B = min(255.0, max(0.0, round(ii*(1 + ss*cos(PI*(hh - 240) / 180) / cos(PI*(300 - hh) / 180)))));
				color.R = min(255.0, max(0.0, round(3 * ii - (double)color.G - (double)color.B)));
			}

			image->SetPixel(x, y, color);
		}
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
			string value = getParamValue("\\s*h\\s*", arg->Parameters->at(index));

			if (value.length())
			{
				hasH = true;
				model->H = regex_match(value, regex("\\s*true\\s*"));
			}
		}

		if (!hasS)
		{
			string value = getParamValue("\\s*s\\s*", arg->Parameters->at(index));

			if (value.length())
			{
				hasS = true;
				model->S = regex_match(value, regex("\\s*true\\s*"));
			}
		}

		if (!hasI)
		{
			string value = getParamValue("\\s*i\\s*", arg->Parameters->at(index));

			if (value.length())
			{
				hasI = true;
				model->I = regex_match(value, regex("\\s*true\\s*"));
			}
		}
	}

	arg->ParsedModel = model;
}