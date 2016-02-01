using System;
using System.Drawing;
using System.Linq;
using System.Text.RegularExpressions;
using ImageProcessor.Models;

namespace ImageProcessor.Filters
{
	public class ThresholdFilter : FilterBase<ThresholdFilterModel>
	{
		public override CommandsLineArg Argument
		{
			get
			{
				return CommandsLineArg.ThresholdFilter;
			}
		}
		public override RawImage Process(ThresholdFilterModel arg, RawImage image)
		{
			var roi = (arg.Roi ?? arg.DefaultRoi).Region;
			var threshold = Convert.ToByte(arg.Threshold * 255);

			for (var y = roi.Y; y < roi.Bottom; y++)
				for (var x = roi.X; x < roi.Right; x++)
					image.SetPixel(x, y, image.ThresholdFilter(x, y, threshold));

			return image;
		}
		public override ThresholdFilterModel Parse(CommandLineArgModel arg)
		{
			return new ThresholdFilterModel
			{
				Roi = RoiModel.Parse(arg),
				Threshold = arg.Parameters
					.Where(param => Regex.IsMatch(param, "^" + ThresholdFilterModel.ThresholdPattern + "$", RegexOptions.IgnoreCase))
					.Select(param => (double?)Convert.ToDouble(param.Substring(param.IndexOf(':') + 1).Trim()))
					.FirstOrDefault() ?? 0.5d
			};
		}
	}
	public class ThresholdFilterModel : IRoiModel
	{
		public RoiModel DefaultRoi
		{
			get;
			set;
		}
		public RoiModel Roi
		{
			get;
			set;
		}
		public const string ThresholdPattern = @"\s*threshold\s*\:\s*(0*|0*1(\.?|\.0*)|0*\.\d+)\s*";
		public double Threshold
		{
			get;
			set;
		}
	}
}
