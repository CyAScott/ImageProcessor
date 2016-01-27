using System;
using System.Drawing;
using System.Linq;
using System.Text.RegularExpressions;

namespace ImageProcessor.Models
{
	public interface IRoiModel
	{
		RoiModel DefaultRoi
		{
			get;
			set;
		}
		RoiModel Roi
		{
			get;
			set;
		}
	}
	public class RoiModel
	{
		public RoiModel()
		{
		}
		public RoiModel(Image image)
		{
			Region = new Rectangle(Point.Empty, image.Size);
		}
		public Rectangle Region
		{
			get;
			set;
		}
		public const string RoiPattern = @"\s*roi\s*\:\s*\d+\s*,\s*\d+\s*,\s*\d+\s*,\s*\d+\s*";
		public static RoiModel Parse(CommandLineArgModel args)
		{
			var roi = args.Parameters.FirstOrDefault(arg => Regex.IsMatch(arg, "^" + RoiPattern + "$", RegexOptions.IgnoreCase));
			if (String.IsNullOrEmpty(roi)) return null;

			var values = roi
				.Substring(roi.IndexOf(':') + 1)
				.Split(',')
				.Select(value => Convert.ToInt32(value.Trim()))
				.ToArray();

			return new RoiModel
			{
				Region = new Rectangle(values[0], values[1], values[2], values[3])
			};
		}
	}
}
