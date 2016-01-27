using System;
using System.ComponentModel.DataAnnotations;
using System.Text.RegularExpressions;

namespace ImageProcessor.Attributes
{
	public class RegexAttribute : ValidationAttribute
	{
		public RegexAttribute(string pattern)
		{
			Pattern = pattern;
			RegexOptions = RegexOptions.Compiled | RegexOptions.IgnoreCase;
			WholeStringMatch = true;
		}

		public RegexOptions RegexOptions { get; set; }
		public bool WholeStringMatch { get; set; }
		public string Pattern { get; set; }

		public override bool IsValid(object value)
		{
			var valueString = Convert.ToString(value);

			if (String.IsNullOrEmpty(valueString)) return true;

			return WholeStringMatch ?
				Regex.IsMatch(valueString, "^" + Pattern + "$", RegexOptions) :
				Regex.IsMatch(valueString, Pattern, RegexOptions);
		}
	}
}
