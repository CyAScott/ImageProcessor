using System.ComponentModel;
using System.ComponentModel.DataAnnotations;
using ImageProcessor.Attributes;
using ImageProcessor.Filters;

namespace ImageProcessor.Models
{
	public enum CommandsLineArg
	{
		[Description("Will list all possible command line arguments and their descriptions.")]
		Help,

		[Description("The Input argument is used to tell the program where the read the input file.")]
		[Paramter("<string>", Description =
			"The relative path to the file.",
			Optional = false)]
		[OnlyOne, Range(1, 1, ErrorMessage = "The -Input argument requires one and only one parameter (i.e. -Input input.jpg)."), Required]
		Input,
		
		[Description(
			"The Output argument is used to tell the program where to write the output file. " +
			"If the file already exists then it will be over written.")]
		[Paramter("<string>", Description =
			"The relative path to the file.",
			Optional = false)]
		[OnlyOne, Range(1, 1, ErrorMessage = "The -Output argument requires one and only one parameter (i.e. -Output output.jpg)."), Required]
		Output,

		[Description(
			"The Scale argument is used to scale an image.  " +
			"The number parameter repsents percentage to increase or decrease the image by (1.0 = 100% or no scaling). " +
			"This parameter is not optional.")]
		[Paramter("<double>", Description =
			"Must be a value greater than or equal to zero. " +
			"Values below 1.0 will shrink the image and values above 1.0 will increase the image size. " +
			"If the value is 1.0 then the image will not be altered.",
			Optional = false)]
		[Range(1, 1, ErrorMessage = "The -Scale argument requires one and only one parameter (i.e. -Scale 2.0).")]
		[Regex(@"(\d+\.?|\d*\.\d+)")]
		Scale,

		[Description(
			"The ThresholdFilter argument is used to convert the image into a set of black or white pixels or color binarization. " +
			"The 'brightness' of each pixel will be calculated into a value between 0 and 1 (where 0 is white and 1 is black).")]
		[Paramter("threshold:<double>", Description =
			"If the brightness of the pixel is greater than or equal to this value then the resulting pixel will be white else it will be black. " +
			"The default value is 0.5.")]
		[RoiParamter]
		[Range(0, 2, ErrorMessage = "The -ThresholdFilter argument requires at most two parameters (i.e. -ThresholdFilter roi:0,0,10,10 threshold:0.5).")]
		[Regex("(" + RoiModel.RoiPattern + "|" + ThresholdFilterModel.ThresholdPattern + ")")]
		ThresholdFilter,

		[Description("Converts the image into colors ranging between black and white (all gray colors).")]
		[RoiParamter]
		[Range(0, 2, ErrorMessage = "The -Grayscale argument requires at most one parameter (i.e. -ThresholdFilter roi:0,0,10,10).")]
		[Regex(RoiModel.RoiPattern)]
		Grayscale,

		[Description("Applies an uniform smoothing filter to an image using the median method.")]
		[Paramter("x:<boolean>", Description = 
			"Applies the filter only in the left and right directions. " +
			"The default value is false.")]
		[Paramter("y:<boolean>", Description = 
			"Applies the filter only in the up and down directions. " +
			"The default value is false.")]
		[Paramter("window:<integer>", Description = 
			"When calculating the resulting pixel, a window surrounding the target pixel is used for the calculation. " +
			"This value must be a positive odd integer. " +
			"The default value is 3.")]
		[RoiParamter]
		[Range(0, 4, ErrorMessage = "The -Median argument requires at most four parameters (i.e. -Median roi:0,0,10,10 window:3 x:true y:false).")]
		[Regex("(" + RoiModel.RoiPattern + "|" + MedianModel.WindowPattern + "|" + MedianModel.XPattern + "|" + MedianModel.YPattern + ")")]
		Median,
	}
}
