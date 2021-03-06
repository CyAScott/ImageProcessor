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
				"Must be a value greater than or equal to zero.",
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
		"The -HistogramEqualization argument requires at most four parameters (i.e. -HistogramEqualization roi:0,0,10,10 h:true s:alse i:true).",
		//ParamMin
		0,
		//ParamMax
		4,

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
	},
	{
		"SobelEdgeDetection",
		"Edge detection using Sobel operator.",

		//AllowRoi
		true,
		//IsFilter
		true,
		//OnlyOne
		false,
		//Required
		false,

		//ParamRangeError
		"The -SobelEdgeDetection argument requires at most eight parameters (i.e. -SobelEdgeDetection roi:0,0,10,10 window:3 threshold:90 direction:45 colorChannel1:true colorChannel2:false colorChannel3:false).",
		//ParamMin
		0,
		//ParamMax
		8,

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
				"window:<integer>",
				//Pattern,
				"\\s*window\\s*\\:\\s*\\(3|5)\\s*",
				//Description
				"When calculating the resulting pixel, a window surrounding the target pixel is used for the calculation. "
				"This value must be a positive odd integer (only 3 and 5 are supported). "
				"The default value is 3.",
				//Optional
				true
			},
			{
				//Format
				"threshold:<byte>",
				//Pattern,
				"\\s*threshold\\s*\\:\\s*(22[0-5]|2[01]\\d|1\\d+\\d+|\\d+\\d+|\\d+)\\s*",
				//Description
				"Must be a value 0 and 255. "
				"Colors greater than or equal to the value will be set as white else black. "
				"If no threshold value is provided then the image will be grayscale.",
				//Optional
				true,
				//IsCaseSensitive
				false
			},
			{
				//Format
				"direction:<integer>",
				//Pattern,
				"\\s*direction\\s*\\:\\s*\\d+\\s*",
				//Description
				"A direction measured in degrees. "
				"Only edges that match this angle will be used. "
				"If no direction value is provided then all angles will be used.",
				//Optional
				true,
				//IsCaseSensitive
				false
			},
			{
				//Format
				"color:<enum>",
				//Pattern,
				"\\s*color\\s*\\:\\s*(gray|rgb|hsi)\\s*",
				//Description
				"The color space to use. "
				"The default value is gray.",
				//Optional
				true,
				//IsCaseSensitive
				false
			},
			{
				//Format
				"colorChannel1:<boolean>",
				//Pattern,
				"\\s*colorChannel1\\s*\\:\\s*(true|false)\\s*",
				//Description
				"If the color parameter is rgb or hsi then r or h channel will be used of edge detection. "
				"The default value is true.",
				//Optional
				true,
				//IsCaseSensitive
				false
			},
			{
				//Format
				"colorChannel2:<boolean>",
				//Pattern,
				"\\s*colorChannel2\\s*\\:\\s*(true|false)\\s*",
				//Description
				"If the color parameter is rgb or hsi then g or s channel will be used of edge detection. "
				"The default value is true.",
				//Optional
				true,
				//IsCaseSensitive
				false
			},
			{
				//Format
				"colorChannel3:<boolean>",
				//Pattern,
				"\\s*colorChannel3\\s*\\:\\s*(true|false)\\s*",
				//Description
				"If the color parameter is rgb or hsi then b or i channel will be used of edge detection. "
				"The default value is true.",
				//Optional
				true,
				//IsCaseSensitive
				false
			}
		}
	},
	{
		"DisparityMap",
		"Converts a side by side stereo image into a depth gray image where dark is far away and light is up close.",

		//AllowRoi
		true,
		//IsFilter
		true,
		//OnlyOne
		false,
		//Required
		false,

		//ParamRangeError
		"The -DisparityMap argument requires at most one parameter (i.e. -DisparityMap roi:0,0,10,10).",
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
	/*
	_______________________________________________________________

	Open CV Filters
	_______________________________________________________________
	*/
	{
		"OpenCvGrayHistogramEqualization",
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
		"The -OpenCvGrayHistogramEqualization argument requires at most one parameter (i.e. -OpenCvGrayHistogramEqualization roi:0,0,10,10).",
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
		"OpenCvHistogramEqualization",
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
		"The -OpenCvHistogramEqualization argument requires at most one parameter (i.e. -OpenCvHistogramEqualization roi:0,0,10,10).",
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
		"OpenCvYCrCbHistogramEqualization",
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
		"The -OpenCvYCrCbHistogramEqualization argument requires at most four parameters (i.e. -OpenCvYCrCbHistogramEqualization roi:0,0,10,10 y:true cr:false cb:true).",
		//ParamMin
		0,
		//ParamMax
		4,

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
				"y:<boolean>",
				//Pattern,
				"\\s*y\\s*\\:\\s*(true|false)\\s*",
				//Description
				"Applies the filter only to the Y channel. "
				"The color of the image will change if this filter option is used. "
				"The default value is true.",
				//Optional
				true,
				//IsCaseSensitive
				false
			},
			{
				//Format
				"Cr:<boolean>",
				//Pattern,
				"\\s*Cr\\s*\\:\\s*(true|false)\\s*",
				//Description
				"Applies the filter only to the Cr channel. "
				"The default value is false.",
				//Optional
				true,
				//IsCaseSensitive
				false
			},
			{
				//Format
				"Cb:<boolean>",
				//Pattern,
				"\\s*Cb\\s*\\:\\s*(true|false)\\s*",
				//Description
				"Applies the filter only to the Cb channel. "
				"The default value is false.",
				//Optional
				true,
				//IsCaseSensitive
				false
			}
		}
	},
	{
		"OpenCvSobelEdgeDetection",
		"Edge detection using Sobel operator.",

		//AllowRoi
		true,
		//IsFilter
		true,
		//OnlyOne
		false,
		//Required
		false,

		//ParamRangeError
		"The -OpenCvSobelEdgeDetection argument requires at most eight parameters (i.e. -OpenCvSobelEdgeDetection roi:0,0,10,10 window:3 threshold:90 direction:45 colorChannel1:true colorChannel2:false colorChannel3:false).",
		//ParamMin
		0,
		//ParamMax
		8,

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
				"window:<integer>",
				//Pattern,
				"\\s*window\\s*\\:\\s*\\(3|5)\\s*",
				//Description
				"When calculating the resulting pixel, a window surrounding the target pixel is used for the calculation. "
				"This value must be a positive odd integer (only 3 and 5 are supported). "
				"The default value is 3.",
				//Optional
				true
			},
			{
				//Format
				"threshold:<byte>",
				//Pattern,
				"\\s*threshold\\s*\\:\\s*(22[0-5]|2[01]\\d|1\\d+\\d+|\\d+\\d+|\\d+)\\s*",
				//Description
				"Must be a value 0 and 255. "
				"Colors greater than or equal to the value will be set as white else black. "
				"If no threshold value is provided then the image will be grayscale.",
				//Optional
				true,
				//IsCaseSensitive
				false
			},
			{
				//Format
				"direction:<integer>",
				//Pattern,
				"\\s*direction\\s*\\:\\s*\\d+\\s*",
				//Description
				"A direction measured in degrees. "
				"Only edges that match this angle will be used. "
				"If no direction value is provided then all angles will be used.",
				//Optional
				true,
				//IsCaseSensitive
				false
			},
			{
				//Format
				"color:<enum>",
				//Pattern,
				"\\s*color\\s*\\:\\s*(gray|rgb|hsi)\\s*",
				//Description
				"The color space to use. "
				"The default value is gray.",
				//Optional
				true,
				//IsCaseSensitive
				false
			},
			{
				//Format
				"colorChannel1:<boolean>",
				//Pattern,
				"\\s*colorChannel1\\s*\\:\\s*(true|false)\\s*",
				//Description
				"If the color parameter is rgb or hsi then r or h channel will be used of edge detection. "
				"The default value is true.",
				//Optional
				true,
				//IsCaseSensitive
				false
			},
			{
				//Format
				"colorChannel2:<boolean>",
				//Pattern,
				"\\s*colorChannel2\\s*\\:\\s*(true|false)\\s*",
				//Description
				"If the color parameter is rgb or hsi then g or s channel will be used of edge detection. "
				"The default value is true.",
				//Optional
				true,
				//IsCaseSensitive
				false
			},
			{
				//Format
				"colorChannel3:<boolean>",
				//Pattern,
				"\\s*colorChannel3\\s*\\:\\s*(true|false)\\s*",
				//Description
				"If the color parameter is rgb or hsi then b or i channel will be used of edge detection. "
				"The default value is true.",
				//Optional
				true,
				//IsCaseSensitive
				false
			}
		}
	},
	{
		"OpenCvCannyEdgeDetection",
		"Edge detection using Sobel operator.",

		//AllowRoi
		true,
		//IsFilter
		true,
		//OnlyOne
		false,
		//Required
		false,

		//ParamRangeError
		"The -OpenCvCannyEdgeDetection argument requires at most eight parameters (i.e. -OpenCvCannyEdgeDetection roi:0,0,10,10 window:3 threshold:90 direction:45 colorChannel1:true colorChannel2:false colorChannel3:false).",
		//ParamMin
		0,
		//ParamMax
		8,

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
				"window:<integer>",
				//Pattern,
				"\\s*window\\s*\\:\\s*\\(3|5)\\s*",
				//Description
				"When calculating the resulting pixel, a window surrounding the target pixel is used for the calculation. "
				"This value must be a positive odd integer (only 3 and 5 are supported). "
				"The default value is 3.",
				//Optional
				true
			},
			{
				//Format
				"threshold:<byte>",
				//Pattern,
				"\\s*threshold\\s*\\:\\s*(22[0-5]|2[01]\\d|1\\d+\\d+|\\d+\\d+|\\d+)\\s*",
				//Description
				"Must be a value 0 and 255. "
				"Colors greater than or equal to the value will be set as white else black. "
				"If no threshold value is provided then the image will be grayscale.",
				//Optional
				true,
				//IsCaseSensitive
				false
			},
			{
				//Format
				"direction:<integer>",
				//Pattern,
				"\\s*direction\\s*\\:\\s*\\d+\\s*",
				//Description
				"A direction measured in degrees. "
				"Only edges that match this angle will be used. "
				"If no direction value is provided then all angles will be used.",
				//Optional
				true,
				//IsCaseSensitive
				false
			},
			{
				//Format
				"color:<enum>",
				//Pattern,
				"\\s*color\\s*\\:\\s*(gray|rgb|ycrcb)\\s*",
				//Description
				"The color space to use. "
				"The default value is gray.",
				//Optional
				true,
				//IsCaseSensitive
				false
			},
			{
				//Format
				"colorChannel1:<boolean>",
				//Pattern,
				"\\s*colorChannel1\\s*\\:\\s*(true|false)\\s*",
				//Description
				"If the color parameter is rgb or hsi then r or h channel will be used of edge detection. "
				"The default value is true.",
				//Optional
				true,
				//IsCaseSensitive
				false
			},
			{
				//Format
				"colorChannel2:<boolean>",
				//Pattern,
				"\\s*colorChannel2\\s*\\:\\s*(true|false)\\s*",
				//Description
				"If the color parameter is rgb or hsi then g or s channel will be used of edge detection. "
				"The default value is true.",
				//Optional
				true,
				//IsCaseSensitive
				false
			},
			{
				//Format
				"colorChannel3:<boolean>",
				//Pattern,
				"\\s*colorChannel3\\s*\\:\\s*(true|false)\\s*",
				//Description
				"If the color parameter is rgb or hsi then b or i channel will be used of edge detection. "
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
void testRegExPatterns();

#endif
