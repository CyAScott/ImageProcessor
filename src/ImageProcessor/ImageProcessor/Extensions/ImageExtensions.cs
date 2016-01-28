using System;
using System.Collections.Generic;
using System.Drawing;

namespace ImageProcessor.Extensions
{
	public static class ImageExtensions
	{
		public static Color Average(this IEnumerable<Color> pixels)
		{
			long count = 0, a = 0, r = 0, b = 0, g = 0;
			
			foreach (var pixel in pixels)
			{
				a += pixel.A;
				r += pixel.R;
				b += pixel.B;
				g += pixel.G;
				count++;
			}

			return Color.FromArgb((byte)(a / count), (byte)(r / count), (byte)(g / count), (byte)(b / count));
		}
		public static IEnumerable<Color> GetPixels(this Bitmap @this, Rectangle roi, int x, int y, int xPadding, int yPadding)
		{
			int columnStart = Math.Max(roi.X, x - xPadding),
				columns = Math.Min(roi.Right, x + xPadding),
				rowStart = Math.Max(roi.Y, y - yPadding),
				rows = Math.Min(roi.Bottom, y + yPadding);
			for (var col = columnStart; col < columns; col++)
				for (var row = rowStart; row < rows; row++)
					yield return @this.GetPixel(col, row);
		}
		public static IEnumerable<IEnumerable<byte>> EachRow(this Bitmap @this, bool useAllColorValues = true)
		{
			for (var row = 0; row < @this.Height; row++)
				yield return @this.EachColumn(row, useAllColorValues);
		}
		public static IEnumerable<byte> EachColumn(this Bitmap @this, int row, bool useAllColorValues = true)
		{
			for (var col = 0; col < @this.Width; col++)
			{
				var pixel = @this.GetPixel(col, row);
				yield return pixel.R;
				if (useAllColorValues)
				{
					yield return pixel.G;
					yield return pixel.B;
				}
			}
		}
	}
}
