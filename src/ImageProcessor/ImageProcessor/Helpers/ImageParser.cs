using System;
using System.IO;
using System.Drawing;
using System.Drawing.Imaging;
using ImageProcessor.Extensions;
using RavuAlHemio.PbmNet;

namespace ImageProcessor.Helpers
{
	public interface IImageParser
	{
		bool TryParse(string path, out Bitmap image);
		void SaveOutput(ICommandArgumentsHelper helper);
	}
	public class ImageParser : IImageParser
	{
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
		public IProcessor Processor
		{
			get;
			set;
		}
		public Locations Locations
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

		public void SaveOutput(ICommandArgumentsHelper helper)
		{
			var extention = (Path.GetExtension(Locations.Output) ?? "").ToLower();

			if (String.IsNullOrEmpty(extention))
			{
				Locations.NewImage.Save(Locations.Output, ImageFormat.Png);
			}
			else
			{
				var writer = new NetpbmWriter();

				switch (extention)
				{
					case ".pbm":
						if (!helper.CommandAndArguments.ContainsKey(CommandsWithAnArgument.ThresholdFilter))
							Processor.ThresholdFilter(0.5);

						using (var stream = File.OpenWrite(Locations.Output))
							writer.WriteImage(
								new NetpbmImage8(
									new NetpbmHeader<byte>(
										ImageType.PBM,
										Locations.NewImage.Width,
										Locations.NewImage.Height,
										1,
										new[] { Component.Black },
										1), Locations.NewImage.EachRow(false)), 
									stream, 
									ImageType.PlainPBM);
						
						break;
					case ".pgm":
						if (!helper.Commands.Contains(CommandsWithOutAnArgument.Grayscale) &&
							!helper.CommandAndArguments.ContainsKey(CommandsWithAnArgument.ThresholdFilter))
							Processor.Grayscale();

						using (var stream = File.OpenWrite(Locations.Output))
							writer.WriteImage(
								new NetpbmImage8(
									new NetpbmHeader<byte>(
										ImageType.PBM,
										Locations.NewImage.Width,
										Locations.NewImage.Height,
										1,
										new[] { Component.Black },
										255), Locations.NewImage.EachRow(false)),
									stream,
									ImageType.PlainPGM);

						break;
					case ".ppm":
						using (var stream = File.OpenWrite(Locations.Output))
							writer.WriteImage(
								new NetpbmImage8(
									new NetpbmHeader<byte>(
										ImageType.PBM,
										Locations.NewImage.Width,
										Locations.NewImage.Height,
										1,
										new[] { Component.Red, Component.Green, Component.Blue },
										255), Locations.NewImage.EachRow()),
									stream,
									ImageType.PlainPPM);

						break;
					default:
						Locations.NewImage.Save(Locations.Output);
						break;
						
				}
			}
		}
	}
}
