using System;

namespace ImageProcessor.Attributes
{
	[AttributeUsage(AttributeTargets.Class)]
	public class LoggerNameAttribute : Attribute
	{
		public LoggerNameAttribute(string name)
		{
			Name = name;
		}
		public string Name
		{
			get;
			set;
		}
	}
}
