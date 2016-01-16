using System;
using System.Drawing;
using System.Linq;
using RavuAlHemio.PbmNet;

namespace ImageProcessor.Extensions
{
	public static class NetpbmImageExtensions
	{
		public static Bitmap ToImage<TPixelComponent>(this NetpbmImage<TPixelComponent> @this)
		{
			if (@this == null) return null;

			var bmp = new Bitmap(@this.Header.Width, @this.Header.Height);

			try
			{
				var y = 0;
				foreach (var row in @this.NativeRows.Select(row => row.ToArray()))
				{
					for (var x = 0; x < @this.Header.Width; x++)
					{
						Color color;
						switch (@this.Header.ImageType)
						{
							case ImageType.PBM:
							case ImageType.PlainPBM:
								color = Convert.ToInt32(row[x]) == 0 ? Color.Black : Color.White;
								break;
							case ImageType.PGM:
							case ImageType.PlainPGM:
								var value = Convert.ToInt32(row[x]);
								color = Color.FromArgb(value, value, value);
								break;
							case ImageType.PPM:
							case ImageType.PlainPPM:
								color = Color.FromArgb(
									Convert.ToInt32(row[x * 3 + 0]),
									Convert.ToInt32(row[x * 3 + 1]),
									Convert.ToInt32(row[x * 3 + 2]));
								break;
							default:
								throw new InvalidProgramException("Netpbm format not supported.");
						}

						bmp.SetPixel(x, y, color); 
					}
					y++;
				}

				return bmp;
			}
			catch
			{
				bmp.Dispose();
				throw;
			}
		}
	}
}
