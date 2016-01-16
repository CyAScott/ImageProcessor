using System.Collections.Generic;
using System.Drawing;

namespace ImageProcessor.Extensions
{
	public static class ImageExntensions
	{
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
