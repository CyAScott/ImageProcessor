#include "disparityMap.h"

RawImage* DisparityMap::ProcessInput(CommandLineArgModel* arg, RawImage* image)
{
	DisparityMapModel* model = (DisparityMapModel*)arg->ParsedModel;
	Rectangle left, roi = model->Roi, right;

	left.Width = roi.Width / 2;
	right.Width = roi.Width / 2;

	left.X = roi.X;
	right.X = roi.X + left.Width;

	left.Right = roi.Right - right.Width;
	right.Right = roi.Right;

	left.Height = right.Height = roi.Height;
	left.Bottom = right.Bottom = roi.Bottom;
	left.Y = right.Y = roi.Y;

	Mat leftImg = image->CloneToMat(left);
	Mat rightImg = image->CloneToMat(right);

	Ptr<StereoBM> stereo = StereoBM::create(16, 15);

	Mat disparity;
	stereo->compute(leftImg, rightImg, disparity);
	
	RawImage* newImage = new RawImage(left.Width, left.Height);

	newImage->Import(disparity, 0, 0);

	return newImage;
}
bool DisparityMap::IsThisArgumentForThisFilter(string arg)
{
	return isMatch("DisparityMap", arg);
}
void DisparityMap::ParseArgument(CommandLineArgModel* arg)
{
	DisparityMapModel* model = new DisparityMapModel();
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
