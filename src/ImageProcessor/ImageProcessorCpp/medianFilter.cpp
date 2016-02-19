#include "medianFilter.h"

RawImage* MedianFilter::ProcessInput(CommandLineArgModel* arg, RawImage* image)
{
	MedianFilterModel* model = (MedianFilterModel*)arg->ParsedModel;

	if (model->Window == 1) return image;

	Rectangle roi = model->Roi;
	int padding = (model->Window - 1) / 2;
	int window = model->Window;

	RawImage* returnValue = NULL;

	if (padding > 1)
	{
		if (model->Y)
		{
			returnValue = image->Clone();

			for (int x = roi.X; x < roi.Right; x++)
				for (int y = roi.Y; y < roi.Bottom; y++)
					returnValue->SetPixel(x, y, image->GetAverage(x, y - padding, 1, window, roi));
			image = returnValue;
		}

		if (model->X)
		{
			returnValue = image->Clone();
			for (int x = roi.X; x < roi.Right; x++)
				for (int y = roi.Y; y < roi.Bottom; y++)
					returnValue->SetPixel(x, y, image->GetAverage(x - padding, y, window, 1, roi));
		}
	}

	return returnValue == NULL ? image : returnValue;
}
bool MedianFilter::IsThisArgumentForThisFilter(string arg)
{
	return isMatch("Median", arg);
}
void MedianFilter::ParseArgument(CommandLineArgModel* arg)
{
	MedianFilterModel* model = new MedianFilterModel();
	model->hasRoi = false;
	model->Window = -1;

	bool hasX = false, hasY = false;

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

		if (model->Window < 0)
		{
			string value = getParamValue("\\s*window\\s*", arg->Parameters->at(index));

			if (value.length())
			{
				model->Window = stoi(trim(value));
			}
		}

		if (!hasX)
		{
			string value = getParamValue("\\s*x\\s*", arg->Parameters->at(index));

			if (value.length())
			{
				hasX = true;
				model->X = regex_match(value, regex("\\s*true\\s*"));
			}
		}

		if (!hasY)
		{
			string value = getParamValue("\\s*y\\s*", arg->Parameters->at(index));

			if (value.length())
			{
				hasY = true;
				model->Y = regex_match(value, regex("\\s*true\\s*"));
			}
		}
	}

	if (model->Window < 0) model->Window = 5;
	if (!hasX) model->X = true;
	if (!hasY) model->Y = true;

	arg->ParsedModel = model;
}
