using System;
namespace ImageProcessor.Attributes
{
	[AttributeUsage(AttributeTargets.Field, AllowMultiple = true)]
	public class ParamterAttribute : Attribute
	{
		public ParamterAttribute(string format)
		{
			Format = format;
			Optional = true;
		}
		public bool Optional
		{
			get;
			set;
		}
		public string Description
		{
			get;
			set;
		}
		public string Format
		{
			get;
			set;
		}
	}
}
