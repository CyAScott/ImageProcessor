#include "sobelEdgeDetection.h"

RawImage* SobelEdgeDetection::ProcessInput(CommandLineArgModel* arg, RawImage* image)
{
	SobelEdgeDetectionModel* model = (SobelEdgeDetectionModel*)arg->ParsedModel;

	Rectangle roi = model->Roi;

	RawImage* returnValue = image->Clone();
	vector<vector<int>> masks;
	const int masks3x3[][3] =
	{
		{ -10, 0, 10 },
		{ -20, 0, 20 },
		{ -10, 0, 10 }
	};
	const int masks5x5[][5] =
	{
		{ -5, -4, 0, 4, 5 },
		{ -8, -10, 0, 10, 8 },
		{ -10, -20, 0, 20, 10 },
		{ -8, -10, 0, 10, 8 },
		{ -5, -4, 0, 4, 5 }
	};

	HsiImage hsiImage;
	if (model->ColorSpace == HSI)
	{
		hsiImage = image->ExportHsi(roi);
	}

	switch (model->Window)
	{
		case 3:
			for (int x = 0; x < 3; x++)
			{
				vector<int> col;
				for (int y = 0; y < 3; y++)
				{
					col.push_back(masks3x3[x][y]);
				}
				masks.push_back(col);
			}
			break;
		case 5:
			for (int x = 0; x < 5; x++)
			{
				vector<int> col;
				for (int y = 0; y < 5; y++)
				{
					col.push_back(masks5x5[x][y]);
				}
				masks.push_back(col);
			}
			break;
	}

	SobelResult sobelResult;
	byte grayColor;
	SobelHsiColorResult hsiResult;
	HsiColor hsiColor;
	SobelRgbColorResult rgbResult;
	Color rgbColor;
	for (int x = roi.X; x < roi.Right; x++)
	{
		for (int y = roi.Y; y < roi.Bottom; y++)
		{
			switch (model->ColorSpace)
			{
				case GRAY:
					sobelResult = image->Sobel(x, y, roi, masks);
					grayColor = sobelResult.G;

					if (model->Direction == -1 ||
						(model->Direction >= sobelResult.Direction - 2 &&
						model->Direction <= sobelResult.Direction + 2))
					{
						grayColor = model->Threshold == -1 ? grayColor : grayColor >= model->Threshold ? 255 : 0;
					}
					else
					{
						grayColor = 0;
					}

					returnValue->SetPixel(x, y, grayColor);
					break;
				case HSI:
					hsiResult = sobelHsi(&hsiImage, x - roi.X, y - roi.Y, roi, masks);
					hsiColor = { hsiResult.H.G, hsiResult.S.G, hsiResult.I.G };

					if (model->ColorChannel1 &&
						(model->Direction == -1 ||
						(model->Direction >= hsiResult.H.Direction - 2 &&
						model->Direction <= hsiResult.H.Direction + 2)))
					{
						hsiColor.H = model->Threshold == -1 ? hsiColor.H : hsiColor.H >= model->Threshold ? 360.0 : 0.0;
					}
					else
					{
						hsiColor.H = hsiImage.H.at(y).at(x);
					}

					if (model->ColorChannel2 &&
						(model->Direction == -1 ||
						(model->Direction >= hsiResult.S.Direction - 2 &&
						model->Direction <= hsiResult.S.Direction + 2)))
					{
						hsiColor.S = model->Threshold == -1 ? hsiColor.S : hsiColor.S >= model->Threshold ? 1.0 : 0.0;
					}
					else
					{
						hsiColor.S = hsiImage.S.at(y).at(x);
					}

					if (model->ColorChannel3 &&
						(model->Direction == -1 ||
						(model->Direction >= hsiResult.I.Direction - 2 &&
						model->Direction <= hsiResult.I.Direction + 2)))
					{
						hsiColor.I = model->Threshold == -1 ? hsiColor.I : hsiColor.I >= model->Threshold ? 255 : 0;
					}
					else
					{
						hsiColor.I = hsiImage.I.at(y).at(x);
					}

					returnValue->SetPixel(x, y, hsiColor.H, hsiColor.S, hsiColor.I);
					break;
				case RGB:
					rgbResult = image->SobelRgb(x, y, roi, masks);
					rgbColor = { roundToByte(rgbResult.R.G), roundToByte(rgbResult.G.G), roundToByte(rgbResult.B.G) };

					if (model->ColorChannel1 &&
						(model->Direction == -1 ||
						(model->Direction >= rgbResult.R.Direction - 2 &&
						model->Direction <= rgbResult.R.Direction + 2)))
					{
						rgbColor.R = model->Threshold == -1 ? rgbColor.R : rgbColor.R >= model->Threshold ? 255 : 0;
					}
					else
					{
						rgbColor.R = 0;
					}

					if (model->ColorChannel2 &&
						(model->Direction == -1 ||
						(model->Direction >= rgbResult.G.Direction - 2 &&
						model->Direction <= rgbResult.G.Direction + 2)))
					{
						rgbColor.G = model->Threshold == -1 ? rgbColor.G : rgbColor.G >= model->Threshold ? 255 : 0;
					}
					else
					{
						rgbColor.G = 0;
					}

					if (model->ColorChannel3 &&
						(model->Direction == -1 ||
						(model->Direction >= rgbResult.B.Direction - 2 &&
						model->Direction <= rgbResult.B.Direction + 2)))
					{
						rgbColor.B = model->Threshold == -1 ? rgbColor.B : rgbColor.B >= model->Threshold ? 255 : 0;
					}
					else
					{
						rgbColor.B = 0;
					}

					returnValue->SetPixel(x, y, rgbColor);
					break;
				case YCRCB:
					throw runtime_error("YCRCB is not support with SobelEdgeDetection.");
			}
		}
	}

	return returnValue;
}
bool SobelEdgeDetection::IsThisArgumentForThisFilter(string arg)
{
	return isMatch("SobelEdgeDetection", arg);
}
void SobelEdgeDetection::ParseArgument(CommandLineArgModel* arg)
{
	SobelEdgeDetectionModel* model = new SobelEdgeDetectionModel();
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
				else if (isMatch(value, "hsi"))
				{
					model->ColorSpace = HSI;
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
