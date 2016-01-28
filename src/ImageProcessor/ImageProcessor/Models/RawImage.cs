using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;

namespace ImageProcessor.Models
{
	public class RawImage
	{
		private byte[][] rawBytes;
		private byte getBrightness(int x, int y)
		{
			var max = (int)Math.Max(Math.Max(rawBytes[y][x], rawBytes[y][x + 1]), rawBytes[y][x + 2]);
			var min = (int)Math.Min(Math.Min(rawBytes[y][x], rawBytes[y][x + 1]), rawBytes[y][x + 2]);

			return (byte)((max + min) / 2);
		}
		private void parse(Bitmap image)
		{
			if (image == null) throw new ArgumentNullException("image");

			//this is the fastest way to get all the pixels from the image
			var bmp = image.LockBits(new Rectangle(0, 0, image.Width, image.Height), ImageLockMode.ReadOnly, PixelFormat.Format32bppArgb);
			var imagePointer = bmp.Scan0;
			var pixels = new byte[bmp.Stride * image.Height];
			var rowLength = image.Width * 4;
			Marshal.Copy(imagePointer, pixels, 0, pixels.Length);

			rawBytes = new byte[image.Height][];

			for (var row = 0; row < rawBytes.Length; row++)
			{
				var newRow = rawBytes[row] = new byte[rowLength];

				Buffer.BlockCopy(pixels, row * rowLength, newRow, 0, newRow.Length);
			}

			Size = image.Size;
		}
		
		public RawImage(string file)
		{
			using (var image = Image.FromFile(file) as Bitmap)
				parse(image);
		}
		public RawImage(Bitmap image)
		{
			parse(image);
		}
		public RawImage(int width, int height)
		{
			if (width < 0 || height < 0) throw new IndexOutOfRangeException("The width and height must be greater than or equal to zero.");

			var rowLength = width*4;

			rawBytes = new byte[height][];

			for (var row = 0; row < rawBytes.Length; row++)
				rawBytes[row] = new byte[rowLength];

			Size = new Size(width, height);
		}

		public Size Size
		{
			get;
			private set;
		}

		public Bitmap ToBitmap()
		{
			var returnValue = new Bitmap(Size.Width, Size.Height);

			for (int x, y = 0; y < rawBytes.Length; y++)
				for (x = 0; x < rawBytes[y].Length; x += 4)
					returnValue.SetPixel(x / 4, y, Color.FromArgb(rawBytes[y][x + 3], rawBytes[y][x + 2], rawBytes[y][x + 1], rawBytes[y][x]));

			return returnValue;
		}

		public Color GetPixel(int x, int y)
		{
			x *= 4;
			return Color.FromArgb(rawBytes[y][x + 3], rawBytes[y][x + 2], rawBytes[y][x + 1], rawBytes[y][x]);
		}
		public void SetPixel(int x, int y, Color color)
		{
			x *= 4;
			rawBytes[y][x] = color.B;
			rawBytes[y][x + 1] = color.G;
			rawBytes[y][x + 2] = color.R;
			rawBytes[y][x + 3] = color.A;
		}

		public float GetBrightness(int x, int y)
		{
			return getBrightness(x * 4, y);
		}

		public void Grayscale(int x, int y)
		{
			x *= 4;

			var color = getBrightness(x, y);

			rawBytes[y][x] = color;
			rawBytes[y][x + 1] = color;
			rawBytes[y][x + 2] = color;
		}

		public void ThresholdFilter(int x, int y, byte threshold)
		{
			x *= 4;

			if (getBrightness(x, y) >= threshold)
			{
				//black
				rawBytes[y][x] = 0;
				rawBytes[y][x + 1] = 0;
				rawBytes[y][x + 2] = 0;
			}
			else
			{
				//white
				rawBytes[y][x] = 255;
				rawBytes[y][x + 1] = 255;
				rawBytes[y][x + 2] = 255;
			}
		}
	}
}
