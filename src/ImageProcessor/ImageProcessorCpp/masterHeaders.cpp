#include "masterHeaders.h"

byte roundToByte(double value)
{
	return (byte)min(255.0, max(0.0, round(value)));
}
byte roundToByte(int value)
{
	return (byte)min(255, max(0, value));
}
Rectangle fromSize(ImageSize size)
{
	return 
	{
		//X
		0,
		//Y
		0,
		//Width
		size.Width,
		//Height
		size.Height,
		//Bottom
		size.Height,
		//Right
		size.Width
	};
}
Rectangle parseRoi(string param)
{
	string value = getParamValue("roi", param);

	if (value.length())
	{
		stringstream ss(value);
		string cell;
		vector<int> roi;

		while (getline(ss, cell, ','))
		{
			roi.push_back(stoi(trim(cell)));
		}

		return
		{
			//X
			roi.at(0),
			//Y
			roi.at(1),
			//Width
			roi.at(2),
			//Height
			roi.at(3),
			//Bottom
			roi.at(3) + roi.at(1),
			//Right
			roi.at(2) + roi.at(0)
		};
	}

	return{ -1, -1, -1, -1, -1, -1 };
}

SobelHsiColorResult sobelHsi(HsiImage* image, int x, int y, Rectangle roi, vector<vector<int>> masks)
{

	SobelHsiColorResult returnValue =
	{
		//H
		{ 0.0, 0.0, 0.0, 0 },
		//S
		{ 0.0, 0.0, 0.0, 0 },
		//I
		{ 0, 0, 0, 0 }
	};

	SobelTargetRoi target = getSobelTargetRoi(x, y, roi, masks);

	for (int maskX, maskY = target.StartMaskY, xIndex, yIndex = target.StartY; yIndex < target.StopY; maskY++, yIndex++)
	{
		for (maskX = target.StartMaskX, xIndex = target.StartX; xIndex < target.StopX; maskX++, xIndex++)
		{
			double h = image->H.at(yIndex).at(xIndex);
			double s = image->S.at(yIndex).at(xIndex);
			int i = image->I.at(yIndex).at(xIndex);

			returnValue.H.Gx += h * masks.at(maskX).at(maskY);
			returnValue.H.Gy += h * masks.at(maskY).at(maskX);

			returnValue.S.Gx += s * masks.at(maskX).at(maskY);
			returnValue.S.Gy += s * masks.at(maskY).at(maskX);

			returnValue.I.Gx += i * masks.at(maskX).at(maskY);
			returnValue.I.Gy += i * masks.at(maskY).at(maskX);
		}
	}

	returnValue.H.G = (abs(returnValue.H.Gx) + abs(returnValue.H.Gy)) / 10;
	if (returnValue.H.G > 360.0) returnValue.H.G = fmod(returnValue.H.G, 360.0);
	if (returnValue.H.G < 0.0) returnValue.H.G = 360 - fmod(abs(returnValue.H.G), 360.0);

	returnValue.S.G = (abs(returnValue.S.Gx) + abs(returnValue.S.Gy)) / 10;
	if (returnValue.S.G > 1.0) returnValue.S.G = 1.0;

	returnValue.I.G = roundToByte((abs(returnValue.I.Gx) + abs(returnValue.I.Gy)) / 10);

	returnValue.H.Direction = returnValue.H.Gx == 0.0 ? 0 :
		(int)round(atan((double)returnValue.H.Gy / (double)returnValue.H.Gx) * 180.0 / PI);
	returnValue.S.Direction = returnValue.S.Gx == 0.0 ? 0 :
		(int)round(atan((double)returnValue.S.Gy / (double)returnValue.S.Gx) * 180.0 / PI);
	returnValue.I.Direction = returnValue.I.Gx == 0 ? 0 :
		(int)round(atan((double)returnValue.I.Gy / (double)returnValue.I.Gx) * 180.0 / PI);

	return returnValue;
}

SobelTargetRoi getSobelTargetRoi(int x, int y, Rectangle roi, vector<vector<int>> masks)
{
	int window = (int)masks.size();

	int padding = (window - 1) / 2;

	int startX = max(x, roi.X);
	int stopX = min(roi.Right, x + window);

	int startY = max(y, roi.Y);
	int stopY = min(roi.Bottom, y + window);

	int startMaskX = 0;
	int startMaskY = 0;

	//if the target x does not have enough space around it for the whole window then
	if (startX - roi.X < padding || roi.Right - stopX < padding)
	{
		startMaskX = padding - min(startX - roi.X, roi.Right - stopX);
		stopX -= startMaskX * 2;
	}

	//if the target y does not have enough space around it for the whole window then
	if (startY - roi.Y < padding || roi.Bottom - stopY < padding)
	{
		startMaskY = padding - min(startY - roi.Y, roi.Bottom - stopY);
		stopY -= startMaskY * 2;
	}

	return
	{
		padding,
		window,

		startX,
		stopX,
		startMaskX,

		startY,
		stopY,
		startMaskY
	};
}

string getParamValue(string paramName, string param)
{
	vector<string> values = splitParam(param);

	return values.size() == 2 && isMatch(values.at(0), paramName) ? values.at(1) : "";
}

string toLowerString(string s)
{
	string returnValue;
	
	for (unsigned int index = 0; index < s.length(); index++)
	{
		returnValue += (char)tolower(s[index]);
	}
	
	return returnValue;
}

string trim(string str)
{
	while (str.length() && isWhiteSpace(str[str.length() - 1]))
	{
		str = str.substr(0, str.length() - 1);
	}
	while (str.length() && isWhiteSpace(str[0]))
	{
		str = str.substr(1);
	}
	return str;
}

vector<string> splitParam(string param)
{
	vector<string> returnValue;

	for (unsigned int index = 0; index < param.length(); index++)
	{
		if (param.at(index) == ':')
		{
			returnValue.push_back(trim(param.substr(0, index)));
			returnValue.push_back(trim(param.substr(index + 1)));
			break;
		}
	}

	return returnValue;
}

bool endsWith(string str, string end)
{
	if (str.length() < end.length()) return false;

	for (unsigned int e = end.length() - 1, s = str.length() - 1; e >= 0 && s >= 0; e--, s--)
	{
		if (toupper(str[s]) != toupper(end[e]))
		{
			return false;
		}
	}

	return true;
}

bool isMatch(string a, string b)
{
	if (a.length() != b.length()) return false;
	
	for (unsigned int index = 0; index < a.length(); index++)
	{
		if (toupper(a[index]) != toupper(b[index]))
		{
			return false;
		}
	}
	
	return true;
}

bool isWhiteSpace(char c)
{
	return
		c == ' ' ||
		c == '\t' ||
		c == '\r' ||
		c == '\n' ||
		c == '\v' ||
		c == '\f';
}

bool fileExists(const string& path)
{
	return ifstream(path) ? true : false;
}

void appendAndWrap(string* builder, string text, unsigned int width)
{
	appendAndWrap(builder, text, width, "");
}

void appendAndWrap(string* builder, string text, unsigned int width, string beginEachStringWith)
{
	while (text.length() > 0)
	{
		string line = text.length() <= width ? text : text.substr(0, min(text.substr(0, width).find_last_of(' '), text.length()));
		builder->append(beginEachStringWith);
		builder->append(trim(line));
		builder->append("\n");
		text = text.substr(line.length());
	}
}

