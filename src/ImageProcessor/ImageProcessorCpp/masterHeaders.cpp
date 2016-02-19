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
	string value = getParamValue("\\s*roi\\s*", param);

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

	return values.size() == 2 && regex_match(values.at(0), regex(paramName)) ? values.at(1) : "";
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
	while (str.length() && regex_match(str.substr(str.length() - 1, 1), regex("\\s")))
	{
		str = str.substr(0, str.length() - 1);
	}
	while (str.length() && regex_match(str.substr(0, 1), regex("\\s")))
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

bool fileExists(const string& path)
{
	return ifstream(path) ? true : false;
}

bool readLine(ifstream& stream, vector<string>& line)
{
	string cell, wholeLine;
	while (getline(stream, wholeLine))
	{
		if (wholeLine.length() && !regex_match(wholeLine, regex("^\\s*#.*$")))
		{
			stringstream ss(wholeLine);
			
			line.clear();

			while (getline(ss, cell, ' '))
			{
				if (cell.length())
				{
					line.push_back(cell);
				}
			}

			return true;
		}
	}

	return false;
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

