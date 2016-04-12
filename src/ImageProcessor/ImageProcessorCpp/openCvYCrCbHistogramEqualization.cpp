#include "openCvYCrCbHistogramEqualization.h"

RawImage* OpenCvYCrCbHistogramEqualization::ProcessInput(CommandLineArgModel* arg, RawImage* image)
{
	OpenCvYCrCbHistogramEqualizationModel* model = (OpenCvYCrCbHistogramEqualizationModel*)arg->ParsedModel;
	Rectangle roi = model->Roi;

	Mat img = image->CloneToMat(roi);

	cvtColor(img, img, CV_BGR2YCrCb);

	vector<Mat> channels;
	split(img, channels);

	if (model->Y)
	{
		equalizeHist(channels[0], channels[0]);
	}
	if (model->Cr)
	{
		equalizeHist(channels[1], channels[1]);
	}
	if (model->Cb)
	{
		equalizeHist(channels[2], channels[2]);
	}

	merge(channels, img);

	cvtColor(img, img, CV_YCrCb2BGR);

	image->Import(img, roi.X, roi.Y);

	return image;
}
bool OpenCvYCrCbHistogramEqualization::IsThisArgumentForThisFilter(string arg)
{
	return isMatch("OpenCvYCrCbHistogramEqualization", arg);
}
void OpenCvYCrCbHistogramEqualization::ParseArgument(CommandLineArgModel* arg)
{
	OpenCvYCrCbHistogramEqualizationModel* model = new OpenCvYCrCbHistogramEqualizationModel();
	model->hasRoi = false;
	model->Y = true;
	model->Cr = false;
	model->Cb = false;
	bool hasY = false, hasCr = false, hasCb = false;

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

		if (!hasY)
		{
			string value = getParamValue("y", arg->Parameters->at(index));

			if (value.length())
			{
				hasY = true;
				model->Y = isMatch(value, "true");
			}
		}

		if (!hasCr)
		{
			string value = getParamValue("cr", arg->Parameters->at(index));

			if (value.length())
			{
				hasCr = true;
				model->Cr = isMatch(value, "true");
			}
		}

		if (!hasCb)
		{
			string value = getParamValue("cb", arg->Parameters->at(index));

			if (value.length())
			{
				hasCb = true;
				model->Cb = isMatch(value, "true");
			}
		}
	}

	arg->ParsedModel = model;
}
