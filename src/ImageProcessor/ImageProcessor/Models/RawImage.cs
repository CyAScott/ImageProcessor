using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Runtime.InteropServices;
using ImageProcessor.Extensions;
using RavuAlHemio.PbmNet;

namespace ImageProcessor.Models
{
	public partial class RawImage
	{
		public static bool TryParse8BitPbm(string path, out RawImage image)
		{
			try
			{
				var reader = new NetpbmReader();

				using (var stream = File.OpenRead(path))
				{
					var pbmImage = reader.ReadImage(stream, Image8Factory);

					image = pbmImage.ToRawImage();
				}

				return true;
			}
			catch
			{
				image = null;
				return false;
			}
		}
		public static bool TryParse16BitPbm(string path, out RawImage image)
		{
			try
			{
				var reader = new NetpbmReader();

				using (var stream = File.OpenRead(path))
				{
					var pbmImage = reader.ReadImage(stream, Image8Factory);

					image = pbmImage.ToRawImage();
				}

				return true;
			}
			catch
			{
				image = null;
				return false;
			}
		}
		public static bool TryParse32BitPbm(string path, out RawImage image)
		{
			try
			{
				var reader = new NetpbmReader();

				using (var stream = File.OpenRead(path))
				{
					var pbmImage = reader.ReadImage(stream, Image8Factory);

					image = pbmImage.ToRawImage();
				}

				return true;
			}
			catch
			{
				image = null;
				return false;
			}
		}
		public static bool TryParse(string path, out RawImage image)
		{
			try
			{
				if (TryParse8BitPbm(path, out image) ||
					TryParse16BitPbm(path, out image) ||
					TryParse32BitPbm(path, out image))
					return true;

				using (var bmp = (Bitmap)Image.FromFile(path))
					image = new RawImage(bmp);

				return true;
			}
			catch
			{
				image = null;
				return false;
			}
		}

		public static readonly ImageFactories.Image8Factory Image8Factory = new ImageFactories.Image8Factory();
		public static readonly ImageFactories.Image16Factory Image16Factory = new ImageFactories.Image16Factory();
		public static readonly ImageFactories.Image32Factory Image32Factory = new ImageFactories.Image32Factory();
	}
	public partial class RawImage
	{
		private IEnumerable<IEnumerable<byte>> eachRow(bool useAllColorValues = true, bool binary = false)
		{
			for (var y = 0; y < Size.Height; y++)
				yield return eachColumn(y, useAllColorValues, binary);
		}
		private IEnumerable<byte> eachColumn(int y, bool useAllColorValues = true, bool binary = false)
		{
			for (var x = 0; x < rawBytes[y].Length; x += 4)
			{
				var b = rawBytes[y][x + 2];
				yield return binary ? b == 0 ? (byte)0 : (byte)1 : b;
				if (useAllColorValues)
				{
					yield return rawBytes[y][x + 1];
					yield return rawBytes[y][x];
				}
			}
		}
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
	}
	public partial class RawImage
	{
		public RawImage(Bitmap image)
		{
			parse(image);
		}
		public RawImage(int width, int height)
		{
			if (width < 0 || height < 0) throw new IndexOutOfRangeException("The width and height must be greater than or equal to zero.");

			var rowLength = width * 4;

			rawBytes = new byte[height][];

			for (var row = 0; row < rawBytes.Length; row++)
				rawBytes[row] = new byte[rowLength];

			Size = new Size(width, height);
		}
		public RawImage(string file)
		{
			if (!File.Exists(file))
				throw new FileNotFoundException("The image file does not exists.");

			RawImage newImage;
			if (!TryParse(file, out newImage))
				throw new ArgumentException("The image file is not a valid format.");

			rawBytes = newImage.rawBytes;
			Size = newImage.Size;
		}

		public Bitmap ToBitmap()
		{
			var returnValue = new Bitmap(Size.Width, Size.Height);

			for (int x, y = 0; y < rawBytes.Length; y++)
				for (x = 0; x < rawBytes[y].Length; x += 4)
					returnValue.SetPixel(x / 4, y, Color.FromArgb(rawBytes[y][x + 3], rawBytes[y][x + 2], rawBytes[y][x + 1], rawBytes[y][x]));

			return returnValue;
		}

		public void Save(string file)
		{
			var extention = (Path.GetExtension(file) ?? "").ToLower();
			var writer = new NetpbmWriter();

			switch (extention)
			{
				case ".pbm":
					using (var stream = File.OpenWrite(file))
						writer.WriteImage(
							new NetpbmImage8(
								new NetpbmHeader<byte>(
									ImageType.PlainPGM,
									Size.Width,
									Size.Height,
									1,
									new[] { Component.White },
									1),
								eachRow(false, true)),
							stream,
							ImageType.PlainPGM);

					break;
				case ".pgm":
					using (var stream = File.OpenWrite(file))
						writer.WriteImage(
							new NetpbmImage8(
								new NetpbmHeader<byte>(
									ImageType.PlainPGM,
									Size.Width,
									Size.Height,
									1,
									new[] { Component.White },
									255), eachRow(false)),
								stream,
								ImageType.PlainPGM);

					break;
				case ".ppm":
					using (var stream = File.OpenWrite(file))
						writer.WriteImage(
							new NetpbmImage8(
								new NetpbmHeader<byte>(
									ImageType.PlainPPM,
									Size.Width,
									Size.Height,
									1,
									new[] { Component.Red, Component.Green, Component.Blue },
									255), eachRow()),
								stream,
								ImageType.PlainPPM);

					break;
				case ".bmp":
					using (var image = ToBitmap())
						image.Save(file, ImageFormat.Bmp);
					break;
				case ".gif":
					using (var image = ToBitmap())
						image.Save(file, ImageFormat.Gif);
					break;
				case ".ico":
					using (var image = ToBitmap())
						image.Save(file, ImageFormat.Icon);
					break;
				case ".jpeg":
				case ".jpg":
				case ".jif":
				case ".jfif":
					using (var image = ToBitmap())
						image.Save(file, ImageFormat.Jpeg);
					break;
				case ".png":
					using (var image = ToBitmap())
						image.Save(file, ImageFormat.Png);
					break;
				case ".tif":
				case ".tiff":
					using (var image = ToBitmap())
						image.Save(file, ImageFormat.Tiff);
					break;
				default:
					using (var image = ToBitmap())
						image.Save(file, ImageFormat.Png);
					break;
			}
		}
	}
	public partial class RawImage
	{
		public Size Size
		{
			get;
			private set;
		}

		public Color GetAverage(int x, int y, int width, int height, Rectangle roi)
		{
			long a = 0, r = 0, g = 0, b = 0;

			var startX = Math.Max(x, roi.X);
			var stopX = Math.Min(roi.Right, x + width);

			var startY = Math.Max(y, roi.Y);
			var stopY = Math.Min(roi.Bottom, y + height);

			var count = (stopY - startY) * (stopX - startX);

			startX *= 4;
			stopX *= 4;

			for (int xIndex, yIndex = startY; yIndex < stopY; yIndex++)
				for (xIndex = startX; xIndex < stopX; xIndex += 4)
				{
					a += rawBytes[yIndex][xIndex + 3];
					r += rawBytes[yIndex][xIndex + 2];
					g += rawBytes[yIndex][xIndex + 1];
					b += rawBytes[yIndex][xIndex];
				}

			return Color.FromArgb((byte)(a / count), (byte)(r / count), (byte)(g / count), (byte)(b / count));
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
		public void SetPixel(int x, int y, byte color)
		{
			SetPixel(x, y, 255, color, color, color);
		}
		public void SetPixel(int x, int y, byte r, byte g, byte b)
		{
			SetPixel(x, y, 255, r, g, b);
		}
		public void SetPixel(int x, int y, byte a, byte r, byte g, byte b)
		{
			x *= 4;
			rawBytes[y][x] = b;
			rawBytes[y][x + 1] = g;
			rawBytes[y][x + 2] = r;
			rawBytes[y][x + 3] = a;
		}

		public byte GetBrightness(int x, int y)
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

		public byte ThresholdFilter(int x, int y, byte threshold)
		{
			return getBrightness(x * 4, y) >= threshold ? (byte)255 : (byte)0;
		}
	}
}
