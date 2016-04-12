#include "openCvGrayHistogramEqualization.h"

RawImage* OpenCvGrayHistogramEqualization::ProcessInput(CommandLineArgModel* arg, RawImage* image)
{
	OpenCvGrayHistogramEqualizationModel* model = (OpenCvGrayHistogramEqualizationModel*)arg->ParsedModel;
	Rectangle roi = model->Roi;

	Mat img = image->CloneToMat(roi);

	cvtColor(img, img, CV_BGR2GRAY);
	
	equalizeHist(img, img);

	image->Import(img, roi.X, roi.Y);

	return image;
}
bool OpenCvGrayHistogramEqualization::IsThisArgumentForThisFilter(string arg)
{
	return isMatch("OpenCvGrayHistogramEqualization", arg);
}
void OpenCvGrayHistogramEqualization::ParseArgument(CommandLineArgModel* arg)
{
	OpenCvGrayHistogramEqualizationModel* model = new OpenCvGrayHistogramEqualizationModel();
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
