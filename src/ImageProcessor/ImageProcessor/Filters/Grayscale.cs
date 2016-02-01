using ImageProcessor.Models;

namespace ImageProcessor.Filters
{
	public class Grayscale : FilterBase<GrayscaleModel>
	{
		public override CommandsLineArg Argument
		{
			get
			{
				return CommandsLineArg.Grayscale;
			}
		}
		public override RawImage Process(GrayscaleModel arg, RawImage image)
		{
			var roi = (arg.Roi ?? arg.DefaultRoi).Region;

			for (var y = roi.Y; y < roi.Bottom; y++)
				for (var x = roi.X; x < roi.Right; x++)
					image.SetPixel(x, y, image.GetBrightness(x, y));

			return image;
		}
		public override GrayscaleModel Parse(CommandLineArgModel arg)
		{
			return new GrayscaleModel
			{
				Roi = RoiModel.Parse(arg)
			};
		}
	}
	public class GrayscaleModel : IRoiModel
	{
		public RoiModel DefaultRoi
		{
			get;
			set;
		}
		public RoiModel Roi
		{
			get;
			set;
		}
	}
}
