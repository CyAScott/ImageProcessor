using System;
using System.IO;
using System.Drawing;
using System.Drawing.Imaging;
using ImageProcessor.Attributes;
using ImageProcessor.Effects;
using ImageProcessor.Extensions;
using RavuAlHemio.PbmNet;

namespace ImageProcessor.Helpers
{
	public interface IImageIo
	{
		bool TryParse(string path, out Bitmap image);
		void SaveOutput(Bitmap image, bool isGrayscale, bool isBlackAndWhite, string savePath);
	}
	[LoggerName("Image IO")]
	public class ImageIo : IImageIo
	{
		public IEffect<GrayscaleModel> Grayscale
		{
			get;
			set;
		}
		public IEffect<ThresholdFilterModel> ThresholdFilter
		{
			get;
			set;
		}
		public ImageFactories.Image8Factory Image8Factory
		{
			get;
			set;
		}
		public ImageFactories.Image16Factory Image16Factory
		{
			get;
			set;
		}
		public ImageFactories.Image32Factory Image32Factory
		{
			get;
			set;
		}

		public bool TryParse8BitPbm(string path, out Bitmap image)
		{
			try
			{
				var reader = new NetpbmReader();

				using (var stream = File.OpenRead(path))
				{
					var pbmImage = reader.ReadImage(stream, Image8Factory);

					image = pbmImage.ToImage();
				}

				return true;
			}
			catch
			{
				image = null;
				return false;
			}
		}
		public bool TryParse16BitPbm(string path, out Bitmap image)
		{
			try
			{
				var reader = new NetpbmReader();

				using (var stream = File.OpenRead(path))
				{
					var pbmImage = reader.ReadImage(stream, Image8Factory);

					image = pbmImage.ToImage();
				}

				return true;
			}
			catch
			{
				image = null;
				return false;
			}
		}
		public bool TryParse32BitPbm(string path, out Bitmap image)
		{
			try
			{
				var reader = new NetpbmReader();

				using (var stream = File.OpenRead(path))
				{
					var pbmImage = reader.ReadImage(stream, Image8Factory);

					image = pbmImage.ToImage();
				}

				return true;
			}
			catch
			{
				image = null;
				return false;
			}
		}
		public bool TryParse(string path, out Bitmap image)
		{
			try
			{
				if (TryParse8BitPbm(path, out image) ||
					TryParse16BitPbm(path, out image) ||
					TryParse32BitPbm(path, out image))
					return true;

				image = (Bitmap)Image.FromFile(path);
				return true;
			}
			catch
			{
				image = null;
				return false;
			}
		}

		public void SaveOutput(Bitmap image, bool isGrayscale, bool isBlackAndWhite, string savePath)
		{
			var extention = (Path.GetExtension(savePath) ?? "").ToLower();

			if (String.IsNullOrEmpty(extention))
			{
				image.Save(savePath, ImageFormat.Png);
			}
			else
			{
				var writer = new NetpbmWriter();

				switch (extention)
				{
					case ".pbm":
						//if (!isBlackAndWhite)
						//	image = ThresholdFilter.Process(new ThresholdFilterModel
						//	{
						//		DefaultRoi = new RoiModel(image)
						//	},
						//	image);

						using (var stream = File.OpenWrite(savePath))
							writer.WriteImage(
								new NetpbmImage8(
									new NetpbmHeader<byte>(
										ImageType.PBM,
										image.Width,
										image.Height,
										1,
										new[] { Component.Black },
										1), image.EachRow(false)), 
									stream, 
									ImageType.PlainPBM);
						
						break;
					case ".pgm":
						//if (!isGrayscale && !isBlackAndWhite)
						//		image = Grayscale.Process(new GrayscaleModel
						//		{
						//			DefaultRoi = new RoiModel(image)
						//		},
						//		image);

						using (var stream = File.OpenWrite(savePath))
							writer.WriteImage(
								new NetpbmImage8(
									new NetpbmHeader<byte>(
										ImageType.PBM,
										image.Width,
										image.Height,
										1,
										new[] { Component.Black },
										255), image.EachRow(false)),
									stream,
									ImageType.PlainPGM);

						break;
					case ".ppm":
						using (var stream = File.OpenWrite(savePath))
							writer.WriteImage(
								new NetpbmImage8(
									new NetpbmHeader<byte>(
										ImageType.PBM,
										image.Width,
										image.Height,
										1,
										new[] { Component.Red, Component.Green, Component.Blue },
										255), image.EachRow()),
									stream,
									ImageType.PlainPPM);

						break;
					case ".bmp":
						image.Save(savePath, ImageFormat.Bmp);
						break;
					case ".gif":
						image.Save(savePath, ImageFormat.Gif);
						break;
					case ".ico":
						image.Save(savePath, ImageFormat.Icon);
						break;
					case ".jpeg":
					case ".jpg":
					case ".jif":
					case ".jfif":
						image.Save(savePath, ImageFormat.Jpeg);
						break;
					case ".png":
						image.Save(savePath, ImageFormat.Png);
						break;
					case ".tif":
					case ".tiff":
						image.Save(savePath, ImageFormat.Tiff);
						break;
					default:
						image.Save(savePath, ImageFormat.Png);
						break;
						
				}
			}
		}
	}
}
