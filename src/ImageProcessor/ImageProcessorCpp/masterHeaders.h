#ifndef MASTERHEADERS_H
#define MASTERHEADERS_H

#include <algorithm>
#include <ctype.h>
#include <fstream>
#include <iostream>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>

# define PI           3.14159265358979323846  /* pi */

using namespace std;

typedef unsigned char byte;

enum NetpbmFormat
{
	PBM_P1 = 1,
	PBM_P2 = 2,
	PBM_P3 = 3
};

enum ColorSpaceEnum
{
	GRAY = 1,
	RGB = 2,
	HSI = 3
};

struct HistogramResult
{
	vector<int> R;
	vector<int> G;
	vector<int> B;

	vector<int> I;
};

struct ParamAttribute
{
	string Format;
	string Pattern;
	string Description;
	bool Optional;
	bool IsCaseSensitive;
};

struct ArgAttribute
{
	string Arg;
	string Description;

	bool AllowRoi;
	bool IsFilter;
	bool OnlyOne;
	bool Required;

	//paramater count range
	string ParamRangeError;
	int ParamMin;
	int ParamMax;

	vector<ParamAttribute> Params;
};

struct Color
{
	byte R;
	byte G;
	byte B;
};

struct HsiColor
{
	double H;
	double S;
	int I;
};

struct HsiImage
{
	vector<vector<double>> H;
	vector<vector<double>> S;
	vector<vector<int>> I;
};

struct Rectangle
{
	int X;
	int Y;

	int Width;
	int Height;

	int Bottom;
	int Right;
};

struct Size
{
	int Width;
	int Height;
};

struct SobelResult
{
	//dx
	int Gx;
	//dy
	int Gy;
	//gradient amplitude (abs(Gx) + abs(Gy)) / 10
	int G;
	//edge direction
	int Direction;
};

struct SobelDoubleResult
{
	//dx
	double Gx;
	//dy
	double Gy;
	//gradient amplitude (abs(Gx) + abs(Gy)) / 10
	double G;
	//edge direction
	int Direction;
};

struct SobelHsiColorResult
{
	SobelDoubleResult H;
	SobelDoubleResult S;
	SobelResult I;
};

struct SobelRgbColorResult
{
	SobelResult R;
	SobelResult G;
	SobelResult B;
};

struct SobelTargetRoi
{
	int Padding;
	int Window;

	int StartX;
	int StopX;
	int StartMaskX;

	int StartY;
	int StopY;
	int StartMaskY;
};

class CommandLineArgParsedModel
{
	public:
		Rectangle Roi;
		bool hasRoi;
};

class CommandLineArgModel
{
	public:
		ArgAttribute Argument;
		CommandLineArgParsedModel* ParsedModel = NULL;
		vector<string>* Parameters = NULL;
};

class RawImage;

class FilterHelper
{
	public:
		virtual RawImage* ProcessInput(CommandLineArgModel* arg, RawImage* image)
		{
			return image;
		}
		virtual bool IsThisArgumentForThisFilter(string arg)
		{
			return false;
		}
		virtual void ParseArgument(CommandLineArgModel* arg)
		{
		}
};

byte roundToByte(double value);
byte roundToByte(int value);
Rectangle fromSize(Size size);
Rectangle parseRoi(string param);
SobelHsiColorResult sobelHsi(HsiImage* image, int x, int y, Rectangle roi, vector<vector<int>> masks);
SobelTargetRoi getSobelTargetRoi(int x, int y, Rectangle roi, vector<vector<int>> masks);
string getParamValue(string paramName, string param);
string toLowerString(string s);
string trim(string str);
vector<string> splitParam(string param);
bool endsWith(string str, string end);
bool fileExists(const string& path);
bool isWhiteSpace(char c);
bool isMatch(string a, string b);
void appendAndWrap(string* builder, string text, unsigned int width);
void appendAndWrap(string* builder, string text, unsigned int width, string beginEachStringWith);
#endif
