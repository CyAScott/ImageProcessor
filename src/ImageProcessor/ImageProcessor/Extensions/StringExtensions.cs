using System;
using System.Collections.Generic;
using System.Text;

namespace ImageProcessor.Extensions
{
	public static class StringExtensions
	{
		public static IEnumerable<string> Wrap(this string text, int width)
		{
			while (text.Length > 0)
			{
				var line = text.Length <= width ? text : text.Substring(0, Math.Min(text.Substring(0, width).LastIndexOf(' '), text.Length));
				yield return line.Trim();
				text = text.Substring(line.Length);
			}
		}
		public static void AppendAndWrap(this StringBuilder builder, string text, int width, string beginEachStringWith = null)
		{
			beginEachStringWith = beginEachStringWith ?? "";
			foreach (var line in text.Wrap(width - beginEachStringWith.Length))
				builder.AppendLine(beginEachStringWith + line);
		}
	}
}
