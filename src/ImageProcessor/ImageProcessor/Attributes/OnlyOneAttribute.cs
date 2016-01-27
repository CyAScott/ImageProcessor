using System;

namespace ImageProcessor.Attributes
{
	[AttributeUsage(AttributeTargets.Field)]
	public class OnlyOneAttribute : Attribute
	{
	}
}
