using System;
using System.Drawing;
using System.Linq;
using System.Text.RegularExpressions;
using ImageProcessor.Models;

namespace ImageProcessor.Effects
{
	public class ThresholdFilter : EffectBase<ThresholdFilterModel>
	{
		public override CommandsLineArg Argument
		{
			get
			{
				return CommandsLineArg.ThresholdFilter;
			}
		}
		public override Bitmap Process(ThresholdFilterModel arg, Bitmap image)
		{
			var roi = (arg.Roi ?? arg.DefaultRoi).Region;
			var threshold = arg.Threshold;

			for (var y = roi.Y; y < roi.Bottom; y++)
				for (var x = roi.X; x < roi.Right; x++)
				{
					var brightness = 1d - image.GetPixel(x, y).GetBrightness();

					image.SetPixel(x, y, brightness >= threshold ? Color.Black : Color.White);
				}

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
