using System;

namespace ImageProcessor.Attributes
{
	[AttributeUsage(AttributeTargets.Field)]
	public class RoiParamterAttribute : ParamterAttribute
	{
		public RoiParamterAttribute(string format = null)
			: base(format ?? "roi:<x>,<y>,<width>,<height>")
		{
			Description = "Applies the effect to a target section of the image. The default value is the whole image.";
		}
	}
}
