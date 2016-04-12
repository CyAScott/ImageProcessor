#include "openCvCannyEdgeDetection.h"

RawImage* OpenCvCannyEdgeDetection::ProcessInput(CommandLineArgModel* arg, RawImage* image)
{
	OpenCvCannyEdgeDetectionModel* model = (OpenCvCannyEdgeDetectionModel*)arg->ParsedModel;

	Rectangle roi = model->Roi;
	Mat img = image->CloneToMat(roi);
	vector<Mat> channels;
	
	blur(img, img, Size(model->Window * 3, model->Window * 3));

	switch (model->ColorSpace)
	{
		case GRAY:
			cvtColor(img, img, CV_BGR2GRAY);

			Canny(img, img, 0, model->Threshold, model->Window);

			cvtColor(img, img, CV_GRAY2RGB);
			break;
		case RGB:
			split(img, channels);

			if (model->ColorChannel1)
			{
				Canny(channels[0], channels[0], 0, model->Threshold, model->Window);
			}
			else
			{
				channels[0] = Scalar(0);
			}
			if (model->ColorChannel2)
			{
				Canny(channels[1], channels[1], 0, model->Threshold, model->Window);
			}
			else
			{
				channels[1] = Scalar(0);
			}
			if (model->ColorChannel3)
			{
				Canny(channels[2], channels[2], 0, model->Threshold, model->Window);
			}
			else
			{
				channels[2] = Scalar(0);
			}

			merge(channels, img);
			break;
		case YCRCB:
			cvtColor(img, img, CV_BGR2YCrCb);
			
			split(img, channels);

			if (model->ColorChannel1)
			{
				Canny(channels[0], channels[0], 0, model->Threshold, model->Window);
			}
			if (model->ColorChannel2)
			{
				Canny(channels[1], channels[1], 0, model->Threshold, model->Window);
			}
			if (model->ColorChannel3)
			{
				Canny(channels[2], channels[2], 0, model->Threshold, model->Window);
			}

			merge(channels, img);

			cvtColor(img, img, CV_YCrCb2BGR);
			break;
	}

	image->Import(img, roi.X, roi.Y);

	return image;
}
bool OpenCvCannyEdgeDetection::IsThisArgumentForThisFilter(string arg)
{
	return isMatch("OpenCvCannyEdgeDetection", arg);
}
void OpenCvCannyEdgeDetection::ParseArgument(CommandLineArgModel* arg)
{
	OpenCvCannyEdgeDetectionModel* model = new OpenCvCannyEdgeDetectionModel();
	bool
		hasColorChannel1 = false,
		hasColorChannel2 = false,
		hasColorChannel3 = false,
		hasColorSpace = false;
	model->ColorChannel1 = true;
	model->ColorChannel2 = true;
	model->ColorChannel3 = true;
	model->ColorSpace = YCRCB;// GRAY;
	model->Direction = -1;
	model->hasRoi = false;
	model->Threshold = -1;
	model->Window = -1;

	for (unsigned int index = 0; index < arg->Parameters->size(); index++)
	{

		if (!hasColorChannel1)
		{
			string value = getParamValue("colorChannel1", arg->Parameters->at(index));

			if (value.length())
			{
				model->ColorChannel1 = isMatch(value, "true");
				hasColorChannel1 = true;
			}
		}

		if (!hasColorChannel2)
		{
			string value = getParamValue("colorChannel2", arg->Parameters->at(index));

			if (value.length())
			{
				model->ColorChannel2 = isMatch(value, "true");
				hasColorChannel2 = true;
			}
		}

		if (!hasColorChannel3)
		{
			string value = getParamValue("colorChannel3", arg->Parameters->at(index));

			if (value.length())
			{
				model->ColorChannel3 = isMatch(value, "true");
				hasColorChannel3 = true;
			}
		}

		if (!hasColorSpace)
		{
			string value = getParamValue("color", arg->Parameters->at(index));

			if (value.length())
			{
				if (isMatch(value, "rgb"))
				{
					model->ColorSpace = RGB;
				}
				else if (isMatch(value, "ycrcb"))
				{
					model->ColorSpace = YCRCB;
				}
				hasColorSpace = true;
			}
		}

		if (!model->hasRoi)
		{
			Rectangle roi = parseRoi(arg->Parameters->at(index));
			if (roi.Bottom != -1)
			{
				model->hasRoi = true;
				model->Roi = roi;
			}
		}

		if (model->Direction < 0)
		{
			string value = getParamValue("direction", arg->Parameters->at(index));

			if (value.length())
			{
				model->Direction = stoi(trim(value)) % 360;
			}
		}

		if (model->Threshold < 0)
		{
			string value = getParamValue("threshold", arg->Parameters->at(index));

			if (value.length())
			{
				int threshold = stoi(trim(value));

				if (threshold >= 0 || threshold <= 255)
				{
					model->Threshold = threshold;
				}
			}
		}

		if (model->Window < 0)
		{
			string value = getParamValue("window", arg->Parameters->at(index));

			if (value.length())
			{
				int window = stoi(trim(value));

				if (window == 3 || window == 5)
				{
					model->Window = window;
				}
			}
		}
	}

	if (model->Window < 0) model->Window = 5;

	arg->ParsedModel = model;
}
