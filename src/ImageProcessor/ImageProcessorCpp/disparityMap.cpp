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

	cvtColor(leftImg, leftImg, CV_BGR2GRAY);
	cvtColor(rightImg, rightImg, CV_BGR2GRAY);

	Ptr<StereoSGBM> stereo = StereoSGBM::create(
		0,		//minDisparity
		144,	//numDisparities
		3,		//blockSize
		3 * 3 * 4, 
		3 * 3 * 32, 
		1,		//disp12MaxDiff
		10,		//preFilterCap
		10,		//uniquenessRatio
		100,	//speckleWindowSize
		32,		//speckleRange
		true);	//mode

	Mat disparity;
	stereo->compute(rightImg, leftImg, disparity);

	double max, min;
	minMaxIdx(disparity, &min, &max);
	convertScaleAbs(disparity, disparity, 255 / max);

	cvtColor(disparity, disparity, CV_GRAY2RGB);

	imwrite("F:\\1\\raw-stereo.disparity.opencv.png", disparity);
	
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
