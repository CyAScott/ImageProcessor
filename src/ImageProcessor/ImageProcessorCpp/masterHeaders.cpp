#include "masterHeaders.h"

Rectangle fromSize(Size size)
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

