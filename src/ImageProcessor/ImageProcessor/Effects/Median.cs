using System;
using System.Linq;
using System.Text.RegularExpressions;
using ImageProcessor.Models;

namespace ImageProcessor.Effects
{
	public class Median : EffectBase<MedianModel>
	{
		public override CommandsLineArg Argument
		{
			get
			{
				return CommandsLineArg.Median;
			}
		}
		public override RawImage Process(MedianModel arg, RawImage image)
		{
			if (arg.Window == 1) return image;

			var roi = (arg.Roi ?? arg.DefaultRoi).Region;
			var padding = (arg.Window - 1)/2;

			RawImage returnValue = null;

			if (padding > 1)
			{
				if (arg.Y)
				{
					returnValue = new RawImage(image.Size.Width, image.Size.Height);
					for (var x = roi.X; x < roi.Right; x++)
						for (var y = roi.Y; y < roi.Bottom; y++)
							returnValue.SetPixel(x, y, image.GetAverage(x, y - padding, 1, arg.Window, roi));
					image = returnValue;
				}

				if (arg.X)
				{
					returnValue = new RawImage(image.Size.Width, image.Size.Height);
					for (var x = roi.X; x < roi.Right; x++)
						for (var y = roi.Y; y < roi.Bottom; y++)
							returnValue.SetPixel(x, y, image.GetAverage(x - padding, y, arg.Window, 1, roi));
				}
			}

			return returnValue ?? image;
		}
		public override MedianModel Parse(CommandLineArgModel arg)
		{
			var returnValue = new MedianModel
			{
				Roi = RoiModel.Parse(arg),
				Window = arg.Parameters
					.Where(param => Regex.IsMatch(param, "^" + MedianModel.WindowPattern + "$", RegexOptions.IgnoreCase))
					.Select(param => (int?)Convert.ToInt32(param.Substring(param.IndexOf(':') + 1).Trim()))
					.FirstOrDefault() ?? 3,
				X = arg.Parameters
					.Where(param => Regex.IsMatch(param, "^" + MedianModel.XPattern + "$", RegexOptions.IgnoreCase))
					.Select(param => (bool?)Convert.ToBoolean(param.Substring(param.IndexOf(':') + 1).Trim()))
					.FirstOrDefault() ?? true,
				Y = arg.Parameters
					.Where(param => Regex.IsMatch(param, "^" + MedianModel.YPattern + "$", RegexOptions.IgnoreCase))
					.Select(param => (bool?)Convert.ToBoolean(param.Substring(param.IndexOf(':') + 1).Trim()))
					.FirstOrDefault() ?? true
			};

			if (returnValue.Window % 2 != 1) throw new ArgumentException("The Window parameter for the Median effect most be an odd integer.");

			return returnValue;
		}
	}
	public class MedianModel : IRoiModel
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
		public bool X
		{
			get;
			set;
		}
		public bool Y
		{
			get;
			set;
		}
		public const string WindowPattern = @"\s*window\s*\:\s*\d+\s*";
		public const string XPattern = @"\s*x\s*\:\s*(true|false)\s*";
		public const string YPattern = @"\s*y\s*\:\s*(true|false)\s*";
		public int Window
		{
			get;
			set;
		}
	}
}