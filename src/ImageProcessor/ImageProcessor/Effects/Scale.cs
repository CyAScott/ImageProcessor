using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using ImageProcessor.Models;

namespace ImageProcessor.Effects
{
	public class Scale : EffectBase<ScaleModel>
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
			if (value.Equals(1d)) return image;

			int height = Convert.ToInt32(image.Size.Height * value),
				width = Convert.ToInt32(image.Size.Width * value),
				x, y;

			var imagePixels = new ScalePixel[width, height];
			var newImage = new RawImage(width, height);
			var xs = new HashSet<int>();
			var ys = new HashSet<int>();

			for (y = 0; y < image.Size.Height; y++)
			{
				var centerY = y * value;

				var targetY = Convert.ToInt32(Math.Round(centerY, 0));
				if (targetY >= newImage.Size.Height) targetY = newImage.Size.Height - 1;

				for (x = 0; x < image.Size.Width; x++)
				{
					var centerX = x * value;

					var targetX = Convert.ToInt32(Math.Round(centerX, 0));
					if (targetX >= newImage.Size.Width) targetX = newImage.Size.Width - 1;

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

			for (y = 0; y < newImage.Size.Height; y++)
				for (x = 0; x < newImage.Size.Width; x++)
				{
					if (imagePixels[x, y].Count == 0)
					{
						if (ys.Contains(y))
						{
							for (var subx = x; subx >= 0 && imagePixels[x, y].Count == 0; subx--)
								if (imagePixels[subx, y].Count > 0)
									imagePixels[x, y] = imagePixels[subx, y];
						}
						else if (xs.Contains(x))
						{
							for (var suby = y; suby >= 0 && imagePixels[x, y].Count == 0; suby--)
								if (imagePixels[x, suby].Count > 0)
									imagePixels[x, y] = imagePixels[x, suby];
						}


						if (imagePixels[x, y].Count == 0) hasBlanks = true;
					}

					if (imagePixels[x, y].Count > 0) newImage.SetPixel(x, y, imagePixels[x, y].ToColor());
				}

			if (hasBlanks)
				for (y = 0; y < newImage.Size.Height; y++)
					for (x = 0; x < newImage.Size.Width; x++)
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

			return newImage;
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
