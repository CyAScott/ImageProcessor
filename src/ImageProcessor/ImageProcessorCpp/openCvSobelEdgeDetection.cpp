#include "openCvSobelEdgeDetection.h"

Mat sobel(Mat img, int window, int angle)
{
	Mat direction, g, gx, gy, returnValue;
	
	Sobel(img, gx, CV_32F, 1, 0, window);
	Sobel(img, gy, CV_32F, 0, 1, window);

	cartToPolar(gx, gy, g, direction, true);

	if (angle != -1)
	{
		//direction[x, y] = direction[x, y] > angle ? direction[x, y] : 0
		threshold(direction, direction, (double)angle, 0.0, THRESH_TOZERO);

		//direction[x, y] = direction[x, y] > angle + 1 ? 0 : direction[x, y]
		threshold(direction, direction, (double)angle + 1.0, 0.0, THRESH_BINARY_INV);

		//direction[x, y] = direction[x, y] > 0 ? 1 : 0
		threshold(direction, direction, 0.0, 1.0, THRESH_BINARY);
		
		//returnValue[x, y] = returnValue[x, y] * direction[x, y]
		returnValue *= direction;
	}

	double min, max;
	minMaxLoc(g, &min, &max);
	if (img.type() == CV_8U && max > 0.0)
	{
		g *= 255.0 / max;
	}

	g.convertTo(returnValue, img.type());

	return returnValue;
}

RawImage* OpenCvSobelEdgeDetection::ProcessInput(CommandLineArgModel* arg, RawImage* image)
{
	OpenCvSobelEdgeDetectionModel* model = (OpenCvSobelEdgeDetectionModel*)arg->ParsedModel;

	Rectangle roi = model->Roi;
	Mat g, gx, gy, img = image->CloneToMat(roi);
	vector<Mat> channels;

	//blur(img, img, Size(model->Window, model->Window));

	switch (model->ColorSpace)
	{
		case GRAY:
			cvtColor(img, img, CV_BGR2GRAY);

			img = sobel(img, model->Window, model->Direction);

			cvtColor(img, img, CV_GRAY2RGB);
			break;
		case RGB:
			split(img, channels);

			if (model->ColorChannel1)
			{
				channels[0] = sobel(channels[0], model->Window, model->Direction);
			}
			else
			{
				channels[0] = Scalar(0);
			}
			if (model->ColorChannel2)
			{
				channels[1] = sobel(channels[1], model->Window, model->Direction);
			}
			else
			{
				channels[1] = Scalar(0);
			}
			if (model->ColorChannel3)
			{
				channels[2] = sobel(channels[2], model->Window, model->Direction);
			}
			else
			{
				channels[2] = Scalar(0);
			}

			merge(channels, img);
			break;
		case HSI:
			throw runtime_error("HSI is not support with OpenCvSobelEdgeDetection.");
		case YCRCB:
			cvtColor(img, img, CV_BGR2YCrCb);

			split(img, channels);

			if (model->ColorChannel1)
			{
				channels[0] = sobel(channels[0], model->Window, model->Direction);
			}
			if (model->ColorChannel2)
			{
				channels[1] = sobel(channels[1], model->Window, model->Direction);
			}
			if (model->ColorChannel3)
			{
				channels[2] = sobel(channels[2], model->Window, model->Direction);
			}

			merge(channels, img);
			cvtColor(img, img, CV_YCrCb2BGR);
			break;
	}

	image->Import(img, roi.X, roi.Y);

	return image;
}
bool OpenCvSobelEdgeDetection::IsThisArgumentForThisFilter(string arg)
{
	return isMatch("OpenCvSobelEdgeDetection", arg);
}
void OpenCvSobelEdgeDetection::ParseArgument(CommandLineArgModel* arg)
{
	OpenCvSobelEdgeDetectionModel* model = new OpenCvSobelEdgeDetectionModel();
	bool
		hasColorChannel1 = false,
		hasColorChannel2 = false,
		hasColorChannel3 = false,
		hasColorSpace = false;
	model->ColorChannel1 = true;
	model->ColorChannel2 = true;
	model->ColorChannel3 = true;
	model->ColorSpace = GRAY;
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
