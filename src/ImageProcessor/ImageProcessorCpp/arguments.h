#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include "masterHeaders.h"

ArgAttribute const ArgAttributes[] =
{
	{
		"Help",
		"Will list all possible command line arguments and their descriptions.",
		
		//AllowRoi
		false,
		//IsFilter
		false,
		//OnlyOne
		false,
		//Required
		false,

		//ParamRangeError
		"",
		//ParamMin
		0,
		//ParamMax
		0,

		{}
	},
	{
		"Input",
		"The Input argument is used to tell the program where the read the input file.",

		//AllowRoi
		false,
		//IsFilter
		false,
		//OnlyOne
		true,
		//Required
		true,

		//ParamRangeError
		"The -Input argument requires one and only one parameter (i.e. -Input input.jpg).",
		//ParamMin
		1,
		//ParamMax
		1,

		{
			{
				//Format
				"<string>",
				//Pattern,
				".*",
				//Description
				"The relative path to the file.",
				//Optional
				false,
				//IsCaseSensitive
				true
			}
		}
	},
	{
		"Output",
		"The Output argument is used to tell the program where to write the output file. "
		"If the file already exists then it will be over written.",

		//AllowRoi
		false,
		//IsFilter
		false,
		//OnlyOne
		true,
		//Required
		true,

		//ParamRangeError
		"The -Output argument requires one and only one parameter (i.e. -Output output.jpg).",
		//ParamMin
		1,
		//ParamMax
		1,

		{
			{
				//Format
				"<string>",
				//Pattern,
				".*",
				//Description
				"The relative path to the file.",
				//Optional
				false,
				//IsCaseSensitive
				true
			}
		}
	},
	{
		"Scale",
		"The Scale argument is used to scale an image.  "
		"The number parameter repsents percentage to increase or decrease the image by (1.0 = 100% or no scaling). "
		"This parameter is not optional.",

		//AllowRoi
		false,
		//IsFilter
		true,
		//OnlyOne
		false,
		//Required
		false,

		//ParamRangeError
		"The -Scale argument requires one and only one parameter (i.e. -Scale 2.0).",
		//ParamMin
		1,
		//ParamMax
		1,

		{
			{
				//Format
				"<double>",
				//Pattern,
				"(\\d+\\.?|\\d*\\.\\d+)",
				//Description
				"Must be a value greater than or equal to zero. "
				"Values below 1.0 will shrink the image and values above 1.0 will increase the image size. "
				"If the value is 1.0 then the image will not be altered.",
				//Optional
				false,
				//IsCaseSensitive
				false
			}
		}
	},
	{
		"ThresholdFilter",
		"The ThresholdFilter argument is used to convert the image into a set of black or white pixels or color binarization. "
		"The 'brightness' of each pixel will be calculated into a value between 0 and 1 (where 0 is white and 1 is black).",

		//AllowRoi
		true,
		//IsFilter
		true,
		//OnlyOne
		false,
		//Required
		false,

		//ParamRangeError
		"The -ThresholdFilter argument requires at most two parameters (i.e. -ThresholdFilter roi:0,0,10,10 threshold:0.5).",
		//ParamMin
		0,
		//ParamMax
		2,

		{
			{
				//Format
				"threshold:<double>",
				//Pattern,
				"\\s*threshold\\s*\\:\\s*(0*|0*1(\\.?|\\.0*)|0*\\.\\d+)\\s*",
				//Description
				"Must be a value greater than or equal to zero. "
				"Values below 1.0 will shrink the image and values above 1.0 will increase the image size. "
				"If the value is 1.0 then the image will not be altered.",
				//Optional
				true,
				//IsCaseSensitive
				false
			},
			{
				//Format
				"roi:<x>,<y>,<width>,<height>",
				//Pattern,
				"\\s*roi\\s*\\:\\s*\\d+\\s*,\\s*\\d+\\s*,\\s*\\d+\\s*,\\s*\\d+\\s*",
				//Description
				"Applies the filter to a target section of the image. The default value is the whole image",
				//Optional
				true,
				//IsCaseSensitive
				false
			}
		}
	},
	{
		"Grayscale",
		"Converts the image into colors ranging between black and white (all gray colors).",

		//AllowRoi
		true,
		//IsFilter
		true,
		//OnlyOne
		false,
		//Required
		false,

		//ParamRangeError
		"The -Grayscale argument requires at most one parameter (i.e. -ThresholdFilter roi:0,0,10,10).",
		//ParamMin
		0,
		//ParamMax
		1,

		{
			{
				//Format
				"roi:<x>,<y>,<width>,<height>",
				//Pattern,
				"\\s*roi\\s*\\:\\s*\\d+\\s*,\\s*\\d+\\s*,\\s*\\d+\\s*,\\s*\\d+\\s*",
				//Description
				"Applies the filter to a target section of the image. The default value is the whole image",
				//Optional
				true,
				//IsCaseSensitive
				false
			}
		}
	},
	{
		"Median",
		"Applies an uniform smoothing filter to an image using the median method.",

		//AllowRoi
		true,
		//IsFilter
		true,
		//OnlyOne
		false,
		//Required
		false,

		//ParamRangeError
		"The -Median argument requires at most four parameters (i.e. -Median roi:0,0,10,10 window:3 x:true y:false).",
		//ParamMin
		0,
		//ParamMax
		4,

		{
			{
				//Format
				"x:<boolean>",
				//Pattern,
				"\\s*x\\s*\\:\\s*(true|false)\\s*",
				//Description
				"Applies the filter only in the left and right directions. "
				"The default value is false.",
				//Optional
				true,
				//IsCaseSensitive
				false
			},
			{
				//Format
				"y:<boolean>",
				//Pattern,
				"\\s*y\\s*\\:\\s*(true|false)\\s*",
				//Description
				"Applies the filter only in the up and down directions. "
				"The default value is false.",
				//Optional
				true,
				//IsCaseSensitive
				false
			},
			{
				//Format
				"window:<integer>",
				//Pattern,
				"\\s*window\\s*\\:\\s*\\d+\\s*",
				//Description
				"When calculating the resulting pixel, a window surrounding the target pixel is used for the calculation. "
				"This value must be a positive odd integer. "
				"The default value is 3.",
				//Optional
				true
			},
			{
				//Format
				"roi:<x>,<y>,<width>,<height>",
				//Pattern,
				"\\s*roi\\s*\\:\\s*\\d+\\s*,\\s*\\d+\\s*,\\s*\\d+\\s*,\\s*\\d+\\s*",
				//Description
				"Applies the filter to a target section of the image. The default value is the whole image",
				//Optional
				true,
				//IsCaseSensitive
				false
			}
		}
	},
	{
		"GrayHistogram",
		"Generates a histogram image based on a gray scale render of the image.",

		//AllowRoi
		true,
		//IsFilter
		true,
		//OnlyOne
		false,
		//Required
		false,

		//ParamRangeError
		"The -GrayHistogram argument requires at most one parameter (i.e. -GrayHistogram roi:0,0,10,10).",
		//ParamMin
		0,
		//ParamMax
		1,

		{
			{
				//Format
				"roi:<x>,<y>,<width>,<height>",
				//Pattern,
				"\\s*roi\\s*\\:\\s*\\d+\\s*,\\s*\\d+\\s*,\\s*\\d+\\s*,\\s*\\d+\\s*",
				//Description
				"Applies the filter to a target section of the image. The default value is the whole image",
				//Optional
				true,
				//IsCaseSensitive
				false
			}
		}
	},
	{
		"GrayHistogramEqualization",
		"Changes the contrast of a gray image based on the histogram.",

		//AllowRoi
		true,
		//IsFilter
		true,
		//OnlyOne
		false,
		//Required
		false,

		//ParamRangeError
		"The -GrayHistogramEqualization argument requires at most one parameter (i.e. -GrayHistogramEqualization roi:0,0,10,10).",
		//ParamMin
		0,
		//ParamMax
		1,

		{
			{
				//Format
				"roi:<x>,<y>,<width>,<height>",
				//Pattern,
				"\\s*roi\\s*\\:\\s*\\d+\\s*,\\s*\\d+\\s*,\\s*\\d+\\s*,\\s*\\d+\\s*",
				//Description
				"Applies the filter to a target section of the image. The default value is the whole image",
				//Optional
				true,
				//IsCaseSensitive
				false
			}
		}
	},
	{
		"HistogramStretch",
		"Changes the contrast of an image based on a percentile in the histogram.",

		//AllowRoi
		true,
		//IsFilter
		true,
		//OnlyOne
		false,
		//Required
		false,

		//ParamRangeError
		"The -HistogramStretch argument requires at most two parameters (i.e. -HistogramStretch roi:0,0,10,10 percentile:0.5).",
		//ParamMin
		0,
		//ParamMax
		2,

		{
			{
				//Format
				"roi:<x>,<y>,<width>,<height>",
				//Pattern,
				"\\s*roi\\s*\\:\\s*\\d+\\s*,\\s*\\d+\\s*,\\s*\\d+\\s*,\\s*\\d+\\s*",
				//Description
				"Applies the filter to a target section of the image. The default value is the whole image",
				//Optional
				true,
				//IsCaseSensitive
				false
			},
			{
				//Format
				"percentile:<double>",
				//Pattern,
				"\\s*percentile\\s*\\:\\s*(0*|0*1(\\.?|\\.0*)|0*\\.\\d+)\\s*",
				//Description
				"Must be a value between 0 and 1. "
				"The default value is 0.05. "
				"This value will be used as the lower and upper percentile of the histogram.",
				//Optional
				true,
				//IsCaseSensitive
				false
			}
		}
	},
	{
		"HistogramEqualization",
		"Changes the contrast of an image based on the histogram.",

		//AllowRoi
		true,
		//IsFilter
		true,
		//OnlyOne
		false,
		//Required
		false,

		//ParamRangeError
		"The -HistogramEqualization argument requires at most one parameter (i.e. -HistogramEqualization roi:0,0,10,10).",
		//ParamMin
		0,
		//ParamMax
		1,

		{
			{
				//Format
				"roi:<x>,<y>,<width>,<height>",
				//Pattern,
				"\\s*roi\\s*\\:\\s*\\d+\\s*,\\s*\\d+\\s*,\\s*\\d+\\s*,\\s*\\d+\\s*",
				//Description
				"Applies the filter to a target section of the image. The default value is the whole image",
				//Optional
				true,
				//IsCaseSensitive
				false
			}
		}
	},
	{
		"HsiHistogramEqualization",
		"Changes the contrast of an image based on the histogram.",

		//AllowRoi
		true,
		//IsFilter
		true,
		//OnlyOne
		false,
		//Required
		false,

		//ParamRangeError
		"The -HistogramEqualization argument requires at most one parameter (i.e. -HistogramEqualization roi:0,0,10,10).",
		//ParamMin
		0,
		//ParamMax
		1,

		{
			{
				//Format
				"roi:<x>,<y>,<width>,<height>",
				//Pattern,
				"\\s*roi\\s*\\:\\s*\\d+\\s*,\\s*\\d+\\s*,\\s*\\d+\\s*,\\s*\\d+\\s*",
				//Description
				"Applies the filter to a target section of the image. The default value is the whole image",
				//Optional
				true,
				//IsCaseSensitive
				false
			},
			{
				//Format
				"h:<boolean>",
				//Pattern,
				"\\s*h\\s*\\:\\s*(true|false)\\s*",
				//Description
				"Applies the filter only to the H channel. "
				"The color of the image will change if this filter option is used. "
				"The default value is false.",
				//Optional
				true,
				//IsCaseSensitive
				false
			},
			{
				//Format
				"s:<boolean>",
				//Pattern,
				"\\s*s\\s*\\:\\s*(true|false)\\s*",
				//Description
				"Applies the filter only to the S channel. "
				"The default value is true.",
				//Optional
				true,
				//IsCaseSensitive
				false
			},
			{
				//Format
				"i:<boolean>",
				//Pattern,
				"\\s*i\\s*\\:\\s*(true|false)\\s*",
				//Description
				"Applies the filter only to the I channel. "
				"The default value is true.",
				//Optional
				true,
				//IsCaseSensitive
				false
			}
		}
	}
};

vector<CommandLineArgModel*> parse(unsigned int argc, char* argv[]);
bool validate(vector<CommandLineArgModel*> args);

#endif
