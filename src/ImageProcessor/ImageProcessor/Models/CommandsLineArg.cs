using System.ComponentModel.DataAnnotations;
using ImageProcessor.Attributes;
using ImageProcessor.Effects;

namespace ImageProcessor.Models
{
	/// <summary>
	/// These are all the possible arguments with there possible parameters.
	/// 
	/// For roi (region of interest) parameters (i.e. roi:0,0,10,10) the format is roi:left,top,width,height.
	/// If the parameter is optional and is not provided, then the whole image will be used for the roi.
	/// </summary>
	public enum CommandsLineArg
	{
		/// <summary>
		/// The Input argument is used to tell the program where the read the input file.
		/// </summary>
		[OnlyOne, Range(1, 1, ErrorMessage = "The -Input argument requires one and only one parameter (i.e. -Input input.jpg)."), Required]
		Input,
		
		/// <summary>
		/// The Output argument is used to tell the program where to write the output file. If the file already exists then it will be over written.
		/// </summary>
		[OnlyOne, Range(1, 1, ErrorMessage = "The -Output argument requires one and only one parameter (i.e. -Output output.jpg)."), Required]
		Output,

		/// <summary>
		/// The Scale argument is used to scale an image. 
		/// The number parameter repsents percentage to increase or decrease the image by (1.0 = 100% or no scaling).
		/// This parameter is not optional.
		/// </summary>
		[Range(1, 1, ErrorMessage = "The -Scale argument requires one and only one parameter (i.e. -Scale 2.0).")]
		[Regex(@"(\d+\.?|\d*\.\d+)")]
		Scale,

		/// <summary>
		/// The ThresholdFilter argument is used to convert the image into a set of black or white pixels.
		/// The 'brightness' of each pixel will be calculated into a value between 0 and 1 (where 1 is white and 0 is black).
		/// 
		/// roi (optional)
		/// This effect will only be applied to the roi parameter.
		/// 
		/// threshold (optional)
		/// The threshold parameter must be a value between 0 and 1. 
		/// If threshold parameter is not provided, then 0.5 will be used.
		/// If the parameter is 0 then the whole image will be white.
		/// If the parameter is 1 then the whole image will be black.
		/// </summary>
		[Range(0, 2, ErrorMessage = "The -ThresholdFilter argument requires at most two parameters (i.e. -ThresholdFilter roi:0,0,10,10 threshold:0.5).")]
		[Regex("(" + RoiModel.RoiPattern + "|" + ThresholdFilterModel.ThresholdPattern + ")")]
		ThresholdFilter,

		/// <summary>
		/// Converts the image into colors ranging between black and white (all gray colors).
		/// 
		/// roi (optional)
		/// This effect will only be applied to the roi parameter.
		/// </summary>
		[Range(0, 2, ErrorMessage = "The -Grayscale argument requires at most one parameter (i.e. -ThresholdFilter roi:0,0,10,10).")]
		[Regex(RoiModel.RoiPattern)]
		Grayscale,
	}
}
