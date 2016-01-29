using System;
using System.Drawing;
using System.Linq;
using ImageProcessor.Models;

namespace ImageProcessor.Effects
{
	public class Scale : EffectBase<ScaleModel>
	{
		private RawImage scaleDown(double scale, RawImage image)
		{
			var newHeight = Convert.ToInt32(Math.Round(image.Size.Height * scale));
			var newWidth = Convert.ToInt32(Math.Round(image.Size.Width * scale));
			var returnValue = new RawImage(newWidth, newHeight);
			var roi = new Rectangle(Point.Empty, image.Size);
			var scaleUp = Math.Pow(scale, -1);
			var scaleWindow = Math.Max(Convert.ToInt32(Math.Floor(scaleUp)), 1);

			if (scaleWindow%2 == 0) scaleWindow++;

			for (var y = 0; y < newHeight; y++)
				for (var x = 0; x < newWidth; x++)
				{
					var sourceX = Convert.ToInt32(Math.Round(x * scaleUp));
					var sourceY = Convert.ToInt32(Math.Round(y * scaleUp));

					returnValue.SetPixel(x, y, image.GetAverage(sourceX, sourceY, scaleWindow, scaleWindow, roi));
				}

			return returnValue;
		}
		private RawImage scaleUp(double scale, RawImage image)
		{
			var newHeight = Convert.ToInt32(Math.Round(image.Size.Height * scale));
			var newWidth = Convert.ToInt32(Math.Round(image.Size.Width * scale));
			var returnValue = new RawImage(newWidth, newHeight);
			var roi = new Rectangle(Point.Empty, image.Size);
			var scaleDown = Math.Pow(scale, -1);

			for (var y = 0; y < newHeight; y++)
				for (var x = 0; x < newWidth; x++)
				{
					var sourceX = Convert.ToInt32(Math.Round(x * scaleDown));
					var sourceY = Convert.ToInt32(Math.Round(y * scaleDown));

					if (sourceX < roi.Width && sourceY < roi.Height)
						returnValue.SetPixel(x, y, image.GetPixel(sourceX, sourceY));
				}

			return Median.Process(new MedianModel
				{
					DefaultRoi = new RoiModel(returnValue),
					Window = Convert.ToInt32(Math.Ceiling(scale)),
					X = true,
					Y = true
				},
				returnValue);
		}

		public IEffect<MedianModel> Median
		{
			get;
			set;
		}
		public override CommandsLineArg Argument
		{
			get
			{
				return CommandsLineArg.Scale;
			}
		}
		public override RawImage Process(ScaleModel arg, RawImage image)
		{
			var value = arg.Scale;

			if (value.Equals(0d)) return new RawImage(0, 0);
			if (value < 1) return scaleDown(value, image);
			if (value > 1) return scaleUp(value, image);

			return image;
		}
		public override ScaleModel Parse(CommandLineArgModel arg)
		{
			return new ScaleModel
			{
				Scale = Convert.ToDouble(arg.Parameters.First())
			};
		}
	}
	public class ScaleModel
	{
		public double Scale
		{
			get;
			set;
		}
	}
}
