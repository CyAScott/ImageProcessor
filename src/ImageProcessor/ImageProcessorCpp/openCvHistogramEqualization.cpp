#include "openCvHistogramEqualization.h"

RawImage* OpenCvHistogramEqualization::ProcessInput(CommandLineArgModel* arg, RawImage* image)
{
	OpenCvHistogramEqualizationModel* model = (OpenCvHistogramEqualizationModel*)arg->ParsedModel;
	Rectangle roi = model->Roi;

	Mat img = image->CloneToMat(roi);

	vector<Mat> channels;
	split(img, channels);

	equalizeHist(channels[0], channels[0]);
	equalizeHist(channels[1], channels[1]);
	equalizeHist(channels[2], channels[2]);

	merge(channels, img);

	image->Import(img, roi.X, roi.Y);

	return image;
}
bool OpenCvHistogramEqualization::IsThisArgumentForThisFilter(string arg)
{
	return isMatch("OpenCvHistogramEqualization", arg);
}
void OpenCvHistogramEqualization::ParseArgument(CommandLineArgModel* arg)
{
	OpenCvHistogramEqualizationModel* model = new OpenCvHistogramEqualizationModel();
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