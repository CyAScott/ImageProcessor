using System;
using System.Collections.Generic;
using System.Drawing;

namespace ImageProcessor.Helpers
{
	public interface IProcessor
	{
		void Grayscale();
		void Scale(double value);
		void ThresholdFilter(double threshold);
	}
	public class Processor : IProcessor
	{
		private struct ScalePixel
		{
			public Color ToColor()
			{
				return Count > 0 ? Color.FromArgb(Convert.ToInt32(R / Count), Convert.ToInt32(G / Count), Convert.ToInt32(B / Count)) : Color.Black;
			}
			public override string ToString()
			{
				return ToColor().ToString();
			}
			public void Reduce()
			{
				var color = ToColor();

				Count = 0;
				R = color.R;
				B = color.B;
				G = color.G;
			}
			public long Count;
			public long R;
			public long B;
			public long G;
		}
		public Locations Locations
		{
			get;
			set;
		}

		public void Grayscale()
		{
			var image = Locations.NewImage;

			for (var y = 0; y < image.Height; y++)
				for (var x = 0; x < image.Width; x++)
				{
					var color = Convert.ToInt32(255d * image.GetPixel(x, y).GetBrightness());

					image.SetPixel(x, y, Color.FromArgb(color, color, color));
				}
		}
		public void Scale(double value)
		{
			if (value.Equals(1d)) return;

			using (var image = Locations.NewImage)
			{
				int height = Convert.ToInt32(image.Height * value),
					width = Convert.ToInt32(image.Width * value),
					x, y;

				var imagePixels = new ScalePixel[width, height];
				var newImage = new Bitmap(width, height);
				var xs = new HashSet<int>();
				var ys = new HashSet<int>();
				Locations.NewImage = newImage;

				for (y = 0; y < image.Height; y++)
				{
					var centerY = y * value;

					var targetY = Convert.ToInt32(Math.Round(centerY, 0));
					if (targetY >= newImage.Height) targetY = newImage.Height - 1;

					for (x = 0; x < image.Width; x++)
					{
						var centerX = x * value;

						var targetX = Convert.ToInt32(Math.Round(centerX, 0));
						if (targetX >= newImage.Width) targetX = newImage.Width - 1;

						var pixel = image.GetPixel(x, y);

						imagePixels[targetX, targetY].R += pixel.R;
						imagePixels[targetX, targetY].B += pixel.B;
						imagePixels[targetX, targetY].G += pixel.G;

						imagePixels[targetX, targetY].Count++;

						xs.Add(targetX);
						ys.Add(targetY);
					}
				}

				var hasBlanks = false;

				for (y = 0; y < newImage.Height; y++)
					for (x = 0; x < newImage.Width; x++)
					{
						if (imagePixels[x, y].Count == 0)
						{
							int subx, suby;

							if (ys.Contains(y))
							{
								for (subx = x; subx >= 0 && imagePixels[x, y].Count == 0; subx--)
									if (imagePixels[subx, y].Count > 0)
										imagePixels[x, y] = imagePixels[subx, y];
							}
							else if (xs.Contains(x))
							{
								for (suby = y; suby >= 0 && imagePixels[x, y].Count == 0; suby--)
									if (imagePixels[x, suby].Count > 0)
										imagePixels[x, y] = imagePixels[x, suby];
							}


							if (imagePixels[x, y].Count == 0) hasBlanks = true;
						}

						if (imagePixels[x, y].Count > 0) newImage.SetPixel(x, y, imagePixels[x, y].ToColor());
					}

				if (hasBlanks)
					for (y = 0; y < newImage.Height; y++)
						for (x = 0; x < newImage.Width; x++)
							if (imagePixels[x, y].Count == 0)
							{
								int subx, suby;


								for (subx = x; subx >= 0; subx--)
									if (imagePixels[subx, y].Count > 0)
									{
										imagePixels[x, y].R += imagePixels[subx, y].R;
										imagePixels[x, y].B += imagePixels[subx, y].B;
										imagePixels[x, y].G += imagePixels[subx, y].G;

										imagePixels[x, y].Count += imagePixels[subx, y].Count;
										break;
									}

								for (suby = y; suby >= 0; suby--)
									if (imagePixels[x, suby].Count > 0)
									{
										imagePixels[x, y].R += imagePixels[x, suby].R;
										imagePixels[x, y].B += imagePixels[x, suby].B;
										imagePixels[x, y].G += imagePixels[x, suby].G;

										imagePixels[x, y].Count += imagePixels[x, suby].Count;
										break;
									}

								newImage.SetPixel(x, y, imagePixels[x, y].ToColor());

								imagePixels[x, y].Reduce();
							}
			}
		}
		public void ThresholdFilter(double threshold)
		{
			var image = Locations.NewImage;

			for (var y = 0; y < image.Height; y++)
				for (var x = 0; x < image.Width; x++)
				{
					var brightness = 1d - image.GetPixel(x, y).GetBrightness();

					image.SetPixel(x, y, brightness > threshold ? Color.Black : Color.White);
				}
		}
	}
}
