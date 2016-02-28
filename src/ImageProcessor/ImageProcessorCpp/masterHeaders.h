#ifndef MASTERHEADERS_H
#define MASTERHEADERS_H

#include <algorithm>
#include <ctype.h>
#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <typeinfo>
#include <vector>
using namespace std;

typedef unsigned char byte;

enum NetpbmFormat
{
	PBM_P1 = 1,
	PBM_P2 = 2,
	PBM_P3 = 3
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

Rectangle fromSize(Size size);
Rectangle parseRoi(string param);
string getParamValue(string paramName, string param);
string toLowerString(string s);
string trim(string str);
vector<string> splitParam(string param);
bool fileExists(const string& path);
bool isMatch(string a, string b);
bool readLine(istream& is, vector<string>& line);
void appendAndWrap(string* builder, string text, unsigned int width);
void appendAndWrap(string* builder, string text, unsigned int width, string beginEachStringWith);
#endif
