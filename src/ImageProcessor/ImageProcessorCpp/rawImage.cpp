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
					byte value = min(255.0, max(0.0, round(stod(line.at(index)) * wordCoefficient)));

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
	byte maxValue = max(max(rawBytes.at(y).at(x), rawBytes.at(y).at(x + 1)), rawBytes.at(y).at(x + 2));
	byte minValue = min(min(rawBytes.at(y).at(x), rawBytes.at(y).at(x + 1)), rawBytes.at(y).at(x + 2));

	return (maxValue + minValue) / 2;
}

RawImage* RawImage::Clone()
{
	return new RawImage(this);
}
Size RawImage::GetSize()
{
	return size;
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