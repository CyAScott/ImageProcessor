using System;
using System.Drawing;
using ImageProcessor.Models;

namespace ImageProcessor.Effects
{
	public class Grayscale : EffectBase<GrayscaleModel>
	{
		public override CommandsLineArg Argument
		{
			get
			{
				return CommandsLineArg.Grayscale;
			}
		}
		public override Bitmap Process(GrayscaleModel arg, Bitmap image)
		{
			var roi = (arg.Roi ?? arg.DefaultRoi).Region;

			for (var y = roi.Y; y < roi.Bottom; y++)
				for (var x = roi.X; x < roi.Right; x++)
				{
					var color = Convert.ToInt32(255d * image.GetPixel(x, y).GetBrightness());

					image.SetPixel(x, y, Color.FromArgb(color, color, color));
				}

			return image;
		}
		public override GrayscaleModel Parse(CommandLineArgModel arg)
		{
			return new GrayscaleModel
			{
				Roi = RoiModel.Parse(arg)
			};
		}
	}
	public class GrayscaleModel : IRoiModel
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
	}
}
