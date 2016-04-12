#include "rawImage.h"

RawImage::RawImage(RawImage* rawImage)
{
	if (rawImage == NULL) throw runtime_error("The raw iamge is null.");

	unsigned int height = rawImage->size.Height;
	unsigned int rowLength = rawImage->size.Width * 3;

	for (unsigned int row = 0; row < height; row++)
	{
		vector<byte> newRow;

		for (unsigned int col = 0; col < rowLength; col++)
		{
			newRow.push_back(rawImage->rawBytes.at(row).at(col));
		}

		rawBytes.push_back(newRow);
	}

	size = rawImage->size;
}
RawImage::RawImage(string path)
{
	ifstream file(path);
	NetpbmFormat format;
	string cell, wholeLine;
	vector<byte> buffer;
	vector<string> line;
	bool readFormat = false, readSize = false, readWordSize = false;
	double wordCoefficient = 1.0;
	unsigned int repeatCellBy = 1, rowLength;

	while (getline(file, wholeLine))
	{
		wholeLine = trim(wholeLine);

		if (wholeLine.length() && wholeLine[0] != '#')
		{
			stringstream ss(wholeLine);

			line.clear();

			while (getline(ss, cell, ' '))
			{
				cell = trim(cell);
				if (cell.length())
				{
					line.push_back(cell);
				}
			}

			if (!readFormat)
			{
				if (line.size() != 1)
				{
					throw runtime_error("Unable to read the photo format.");
				}

				string wholeLine = line.at(0);

				if (wholeLine.length() != 2 || toupper(wholeLine[0]) != 'P')
				{
					throw runtime_error("Unknown photo format.");
				}

				if (wholeLine[1] == '1' || wholeLine[1] == '4')
				{
					format = PBM_P1;
					repeatCellBy = 3;
				}
				else if (wholeLine[1] == '2' || wholeLine[1] == '5')
				{
					format = PBM_P2;
					repeatCellBy = 3;
				}
				else if (wholeLine[1] == '3' || wholeLine[1] == '6')
				{
					format = PBM_P3;
					repeatCellBy = 1;
				}
				else
				{
					throw runtime_error("Unknown photo format.");
				}

				readFormat = true;
			}
			else if (!readSize)
			{
				if (line.size() != 2)
				{
					throw runtime_error("Unable to read the photo size.");
				}
				string width = line.at(0);
				string height = line.at(1);

				size.Width = stoi(width);
				size.Height = stoi(height);

				readSize = true;
			}
			else if (!readWordSize)
			{
				wordCoefficient = 255.0 / stod(line.at(0));
				readWordSize = true;
			}
			else
			{
				rowLength = 3 * size.Width;

				for (unsigned int index = 0; index < line.size(); index++)
				{
					byte value = roundToByte(stod(line.at(index)) * wordCoefficient);

					if (format == PBM_P1)
					{
						value = value ? 255 : 0;
					}

					for (unsigned int i = 0; i < repeatCellBy; i++)
					{
						buffer.push_back(value);
					}
					
					if (buffer.size() == rowLength)
					{
						vector<byte> newRow(buffer);
						
						buffer.clear();
						
						rawBytes.push_back(newRow);
					}
				}
			}
		}
	}

	if (rawBytes.size() != (unsigned int)size.Height)
	{
		throw runtime_error("The image is corrupt. The height is incorrect.");
	}

	file.close();

	remove(path.c_str());
}
RawImage::RawImage(int width, int height)
{
	if (width < 0 || height < 0) throw runtime_error("The width and height must be greater than or equal to zero.");

	int rowLength = width * 3;

	for (int row = 0; row < height; row++)
	{
		vector<byte> newRow;

		for (int col = 0; col < rowLength; col++)
		{
			newRow.push_back(0);
		}

		rawBytes.push_back(newRow);
	}

	size = { width, height };
}

HsiImage RawImage::ExportHsi(Rectangle roi)
{
	vector<vector<double>> H;
	vector<vector<double>> S;
	vector<vector<int>> I;

	for (int y = roi.Y; y < roi.Bottom; y++)
	{
		vector<double> hRow;
		vector<double> sRow;
		vector<int> iRow;

		for (int x = roi.X; x < roi.Right; x++)
		{
			Color color = GetPixel(x, y);
			double r = color.R;
			double b = color.B;
			double g = color.G;

			int i = (int)(r + b + g) / 3;

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
		}

		H.push_back(hRow);
		S.push_back(sRow);
		I.push_back(iRow);
	}

	return{ H, S, I };
}
void RawImage::ApplyHsi(Rectangle roi, HsiImage hsiImage)
{
	for (int y = 0; y < roi.Height; y++)
	{
		for (int x = 0; x < roi.Width; x++)
		{
			SetPixel(x + roi.X, y + roi.Y,
				hsiImage.H.at(y).at(x),
				hsiImage.S.at(y).at(x),
				hsiImage.I.at(y).at(x));
		}
	}
}

void RawImage::Save(string path)
{
	NetpbmFormat format;
	ofstream file;
	file.open(path);

	if (endsWith(path, ".pbm"))
	{
		format = PBM_P1;
		file << "P1\n";
	}
	else if (endsWith(path, ".pgm"))
	{
		format = PBM_P2;
		file << "P2\n";
	}
	else
	{
		format = PBM_P3;
		file << "P3\n";
	}

	file << size.Width << " " << size.Height << "\n";

	if (format != PBM_P1)
	{
		file << "255\n";
	}

	for (int row = 0; row < size.Height; row++)
	{
		for (int col = 0; col < size.Width; col++)
		{
			if (col)
			{
				file << " ";
			}

			byte value;
			switch (format)
			{
				case PBM_P1:
					value = GetBrightness(col, row);
					file << (value ? "1" : "0");
					break;
				case PBM_P2:
					value = GetBrightness(col, row);
					file << to_string((int)value);
					break;
				case PBM_P3:
					Color color = GetPixel(col, row);
					file << 
						to_string((int)color.R) << " " <<
						to_string((int)color.G) << " " <<
						to_string((int)color.B);
					break;
			}
		}

		file << "\n";
	}

	file.close();
}

byte RawImage::getBrightness(int x, int y)
{
	/*
	byte maxValue = max(max(rawBytes.at(y).at(x), rawBytes.at(y).at(x + 1)), rawBytes.at(y).at(x + 2));
	byte minValue = min(min(rawBytes.at(y).at(x), rawBytes.at(y).at(x + 1)), rawBytes.at(y).at(x + 2));

	return (maxValue + minValue) / 2;
	*/

	return (rawBytes.at(y).at(x) + rawBytes.at(y).at(x + 1) + rawBytes.at(y).at(x + 2)) / 3;
}

Mat RawImage::CloneToMat(Rectangle roi)
{
	Mat returnValue(Mat(roi.Height, roi.Width, CV_8UC3));
	returnValue = Scalar(0, 0, 0);

	for (int row = roi.Y, y = 0; y < roi.Height; row++, y++)
	{
		for (int col = roi.X * 3, x = 0; x < roi.Width; col += 3, x++)
		{
			Vec3b pixel = returnValue.at<Vec3b>(Point(x, y));

			//blue
			pixel.val[0] = rawBytes.at(row).at(col + 2);
			//greeen
			pixel.val[1] = rawBytes.at(row).at(col + 1);
			//red
			pixel.val[2] = rawBytes.at(row).at(col);

			returnValue.at<Vec3b>(Point(x, y)) = pixel;
		}
	}

	return returnValue;
}
RawImage* RawImage::Clone()
{
	return new RawImage(this);
}
ImageSize RawImage::GetSize()
{
	return size;
}
void RawImage::Import(Mat img, int placeOnX, int placeOnY)
{
	if (img.type() == CV_8UC1)
	{
		cvtColor(img, img, CV_GRAY2RGB);
	}

	for (int row = placeOnY, y = 0; y < img.rows; row++, y++)
	{
		for (int col = placeOnX * 3, x = 0; x < img.cols; col += 3, x++)
		{
			Vec3b pixel = img.at<Vec3b>(Point(x, y));

			rawBytes.at(row).at(col) = pixel.val[2];
			rawBytes.at(row).at(col + 1) = pixel.val[1];
			rawBytes.at(row).at(col + 2) = pixel.val[0];
		}
	}
}

Color RawImage::GetAverage(int x, int y, int width, int height, Rectangle roi)
{
	unsigned int r = 0, g = 0, b = 0;

	int startX = max(x, roi.X);
	int stopX = min(roi.Right, x + width);

	int startY = max(y, roi.Y);
	int stopY = min(roi.Bottom, y + height);

	unsigned int count = (stopY - startY) * (stopX - startX);

	startX *= 3;
	stopX *= 3;

	for (int xIndex, yIndex = startY; yIndex < stopY; yIndex++)
	{
		for (xIndex = startX; xIndex < stopX; xIndex += 3)
		{
			r += rawBytes.at(yIndex).at(xIndex);
			g += rawBytes.at(yIndex).at(xIndex + 1);
			b += rawBytes.at(yIndex).at(xIndex + 2);
		}
	}

	return { (byte)(r / count), (byte)(g / count), (byte)(b / count) };
}
Color RawImage::GetPixel(int x, int y)
{
	x *= 3;
	return
	{
		//red
		rawBytes.at(y).at(x),
		//greeen
		rawBytes.at(y).at(x + 1),
		//blue
		rawBytes.at(y).at(x + 2)
	};
}

SobelResult RawImage::Sobel(int x, int y, Rectangle roi, vector<vector<int>> masks)
{
	SobelResult returnValue = { 0, 0, 0, 0 };

	SobelTargetRoi target = getSobelTargetRoi(x, y, roi, masks);

	target.StartX *= 3;
	target.StopX *= 3;

	for (int maskX, maskY = target.StartMaskY, xIndex, yIndex = target.StartY; yIndex < target.StopY; maskY++, yIndex++)
	{
		for (maskX = target.StartMaskX, xIndex = target.StartX; xIndex < target.StopX; maskX++, xIndex += 3)
		{
			byte pixel = getBrightness(xIndex, yIndex);

			returnValue.Gx += pixel * masks.at(maskX).at(maskY);
			returnValue.Gy += pixel * masks.at(maskY).at(maskX);
		}
	}

	returnValue.G = roundToByte((abs(returnValue.Gx) + abs(returnValue.Gy)) / 10);

	returnValue.Direction = returnValue.Gx == 0 ? 0 : 
		(int)round(atan((double)returnValue.Gy / (double)returnValue.Gx) * 180.0 / PI);

	return returnValue;
}
SobelRgbColorResult RawImage::SobelRgb(int x, int y, Rectangle roi, vector<vector<int>> masks)
{
	SobelRgbColorResult returnValue =
	{
		//R
		{ 0, 0, 0, 0 },
		//G
		{ 0, 0, 0, 0 },
		//B
		{ 0, 0, 0, 0 }
	};

	SobelTargetRoi target = getSobelTargetRoi(x, y, roi, masks);

	for (int maskX, maskY = target.StartMaskY, xIndex, yIndex = target.StartY; yIndex < target.StopY; maskY++, yIndex++)
	{
		for (maskX = target.StartMaskX, xIndex = target.StartX; xIndex < target.StopX; maskX++, xIndex++)
		{
			Color pixel = GetPixel(xIndex, yIndex);

			returnValue.R.Gx += pixel.R * masks.at(maskX).at(maskY);
			returnValue.R.Gy += pixel.R * masks.at(maskY).at(maskX);

			returnValue.G.Gx += pixel.G * masks.at(maskX).at(maskY);
			returnValue.G.Gy += pixel.G * masks.at(maskY).at(maskX);

			returnValue.B.Gx += pixel.B * masks.at(maskX).at(maskY);
			returnValue.B.Gy += pixel.B * masks.at(maskY).at(maskX);
		}
	}

	returnValue.R.G = roundToByte((abs(returnValue.R.Gx) + abs(returnValue.R.Gy)) / 10);
	returnValue.G.G = roundToByte((abs(returnValue.G.Gx) + abs(returnValue.G.Gy)) / 10);
	returnValue.B.G = roundToByte((abs(returnValue.B.Gx) + abs(returnValue.B.Gy)) / 10);

	returnValue.R.Direction = returnValue.R.Gx == 0 ? 0 :
		(int)round(atan((double)returnValue.R.Gy / (double)returnValue.R.Gx) * 180.0 / PI);
	returnValue.G.Direction = returnValue.G.Gx == 0 ? 0 :
		(int)round(atan((double)returnValue.G.Gy / (double)returnValue.G.Gx) * 180.0 / PI);
	returnValue.B.Direction = returnValue.B.Gx == 0 ? 0 :
		(int)round(atan((double)returnValue.B.Gy / (double)returnValue.B.Gx) * 180.0 / PI);

	return returnValue;
}

void RawImage::SetPixel(int x, int y, Color color)
{
	x *= 3;
	rawBytes.at(y).at(x) = color.R;
	rawBytes.at(y).at(x + 1) = color.G;
	rawBytes.at(y).at(x + 2) = color.B;
}
void RawImage::SetPixel(int x, int y, byte color)
{
	SetPixel(x, y, color, color, color);
}
void RawImage::SetPixel(int x, int y, byte r, byte g, byte b)
{
	x *= 3;
	rawBytes.at(y).at(x) = r;
	rawBytes.at(y).at(x + 1) = g;
	rawBytes.at(y).at(x + 2) = b;
}
void RawImage::SetPixel(int x, int y, double h, double s, byte i)
{
	Color color;

	if (h == 0)
	{
		color.B = roundToByte(i*(1 - s));
		color.G = roundToByte(i*(1 - s));
		color.R = roundToByte(i*(1 + 2 * s));
	}
	else if (h > 0.0 && h < 120.0)
	{
		color.B = roundToByte(i*(1 - s));
		color.R = roundToByte(i*(1 + s*cos((h / 180.0)*PI) / cos(PI*(60 - h) / 180.0)));
		color.G = roundToByte(3 * i - (double)color.R - (double)color.B);
	}
	else if (h == 120.0)
	{
		color.B = roundToByte(i*(1 - s));
		color.G = roundToByte(i*(1 + 2 * s));
		color.R = roundToByte(i*(1 - s));
	}
	else if (h > 120.0 && h < 240.0)
	{
		color.R = roundToByte(i*(1 - s));
		color.G = roundToByte(i*(1 + s*cos(PI*(h - 120.0) / 180.0) / cos(PI*(180.0 - h) / 180.0)));
		color.B = roundToByte(3 * i - (double)color.R - (double)color.G);
	}
	else if (h == 240.0)
	{
		color.B = roundToByte(i*(1 + 2 * s));
		color.G = roundToByte(i*(1 - s));
		color.R = roundToByte(i*(1 - s));
	}
	else if (h > 240.0 && h <= 360.0)
	{
		color.G = roundToByte(i*(1 - s));
		color.B = roundToByte(i*(1 + s*cos(PI*(h - 240.0) / 180.0) / cos(PI*(300.0 - h) / 180.0)));
		color.R = roundToByte(3 * i - (double)color.G - (double)color.B);
	}

	SetPixel(x, y, color);
}

byte RawImage::GetBrightness(int x, int y)
{
	return getBrightness(x * 3, y);
}

void RawImage::Grayscale(int x, int y)
{
	x *= 3;

	byte color = getBrightness(x, y);

	rawBytes.at(y).at(x) = color;
	rawBytes.at(y).at(x + 1) = color;
	rawBytes.at(y).at(x + 2) = color;
}

byte RawImage::ThresholdFilter(int x, int y, byte threshold)
{
	return getBrightness(x * 3, y) >= threshold ? 255 : 0;
}

HistogramResult RawImage::GetHistogram(Rectangle roi)
{
	HistogramResult returnValue;

	for (int i = 0; i < 256; i++)
	{
		returnValue.R.push_back(0);
		returnValue.B.push_back(0);
		returnValue.G.push_back(0);

		returnValue.I.push_back(0);
	}


	for (int y = roi.Y; y < roi.Bottom; y++)
		for (int x = roi.X; x < roi.Right; x++)
		{
			int xOffset = x * 3;

			returnValue.R.at(rawBytes.at(y).at(xOffset))++;
			returnValue.G.at(rawBytes.at(y).at(xOffset + 1))++;
			returnValue.B.at(rawBytes.at(y).at(xOffset + 2))++;

			returnValue.I.at(GetBrightness(x, y))++;
		}

	return returnValue;
}