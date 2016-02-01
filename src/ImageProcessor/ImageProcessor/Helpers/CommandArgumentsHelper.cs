using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.ComponentModel.DataAnnotations;
using System.IO;
using System.Linq;
using System.Text;
using Castle.Core.Internal;
using ImageProcessor.Attributes;
using ImageProcessor.Extensions;
using ImageProcessor.Models;
using NLog;

namespace ImageProcessor.Helpers
{
	public interface ICommandArgumentsHelper
	{
		void ParseArgs(string[] args);
	}
	[LoggerName("Command Line Parser")]
	public class CommandArgumentsHelper : ICommandArgumentsHelper
	{
		private List<CommandLineArgModel> parse(string[] args)
		{
			CommandLineArgModel currentArgument = null;
			var parameters = new List<string>();
			var arguments = new List<CommandLineArgModel>();

			foreach (var arg in args.Where(arg => !String.IsNullOrEmpty(arg)))
			{
				if (arg.StartsWith("-"))
				{
					if (currentArgument != null)
					{
						currentArgument.Parameters = parameters.ToArray();
						arguments.Add(currentArgument);
					}

					var argument = arg.Substring(1);

					CommandsLineArg commandLineArg;
					if (!Enum.TryParse(argument, true, out commandLineArg))
						throw new ArgumentException(String.Format("The argument \"{0}\" is not recognized.", argument));

					parameters.Clear();

					currentArgument = new CommandLineArgModel
					{
						Argument = commandLineArg
					};
				}
				else if (currentArgument == null)
				{
					throw new ArgumentException("the first command line argument must be an argument (i.e. -Input)");
				}
				else
				{
					parameters.Add(arg);
				}
			}

			if (currentArgument != null)
			{
				currentArgument.Parameters = parameters.ToArray();
				arguments.Add(currentArgument);
			}

			return arguments;
		}
		private bool validate(List<CommandLineArgModel> args)
		{
			const int documentationWidth = 80;
			var displayHelp = args.Any(arg => arg.Argument == CommandsLineArg.Help) && args.Count == 1;
			var documentation = new StringBuilder();

			#region Help Header
			if (displayHelp)
			{
				documentation.AppendAndWrap("Displaying help info:", documentationWidth);
				documentation.AppendLine();

				documentation.AppendAndWrap("The general format for arguments is:", documentationWidth, "\t");
				documentation.AppendLine();
				documentation.AppendAndWrap("-Input <input file path> ... {pipe line arguments} ... -Output <output file path>", documentationWidth, "\t\t");
				documentation.AppendLine();

				documentation.AppendAndWrap(
					"Pipe line arguments are arguments where the order does matter. " +
					"Each pipe line argument applies an filter on the image and applies it in the order in which is was sent to the program. " +
					"The format for an argument and paramters is:", documentationWidth, "\t");
				documentation.AppendLine();
				documentation.AppendAndWrap("-{Argument} {Paramter 1} {Paramter 2} ... {Paramter N}", documentationWidth, "\t\t");
				documentation.AppendLine();

				documentation.AppendAndWrap("The following arguments can be used:", documentationWidth, "\t");
			}
			#endregion

			foreach (var enumMember in Enum.GetValues(typeof(CommandsLineArg))
				.Cast<CommandsLineArg>()
				.Select(value => new
				{
					Args = args.Where(arg => arg.Argument == value).ToArray(),
					Member = typeof(CommandsLineArg).GetMember(Convert.ToString(value)).FirstOrDefault(),
					Value = value
				})
				.Where(method => method.Member != null))
			{
				var isRequired = enumMember.Member.GetAttribute<RequiredAttribute>() != null;
				if (!displayHelp && isRequired && enumMember.Args.Length == 0)
					throw new ArgumentException(String.Format("The -{0} argument is required.", enumMember.Value));

				documentation.AppendAndWrap(String.Format("-{0}: {1}", enumMember.Value, isRequired ? "Required" : "Optional"), documentationWidth, "\t\t");
				documentation.AppendLine();

				#region Argument Attributes
				var description = enumMember.Member.GetAttribute<DescriptionAttribute>();
				var descriptionText = new StringBuilder();
				if (displayHelp && description != null) descriptionText.Append(description.Description + " ");

				var isOnlyOneAllowed = enumMember.Member.GetAttribute<OnlyOneAttribute>() != null;
				if (displayHelp && isOnlyOneAllowed)
					descriptionText.Append("Multiple arguments of this type are not allowed. ");
				if (!displayHelp && isOnlyOneAllowed && enumMember.Args.Length > 1)
					throw new ArgumentException(String.Format("Only one -{0} argument is allowed.", enumMember.Value));

				var range = enumMember.Member.GetAttribute<RangeAttribute>();
				if (displayHelp && isOnlyOneAllowed)
					descriptionText.Append(String.Format(Equals(range.Minimum, range.Maximum) ?
						"The argument requires exactly {0} paramter(s). " : 
						"This argument requires a range of paramters of {0} to {1}. ", range.Minimum, range.Maximum));
				if (!displayHelp && range != null && enumMember.Args.Length > 0 &&
					enumMember.Args.Any(arg => arg.Parameters.Length < (int)range.Minimum || arg.Parameters.Length > (int)range.Maximum))
					throw new ArgumentException(range.ErrorMessage);

				var regex = enumMember.Member.GetAttribute<RegexAttribute>();
				if (!displayHelp && regex != null && 
					enumMember.Args.Length > 0 &&
					enumMember.Args.SelectMany(arg => arg.Parameters).Any(param => !regex.IsValid(param)))
					throw new ArgumentException(String.Format("The -{0} argument paramters are invalid.", enumMember.Value));

				if (!displayHelp && enumMember.Value == CommandsLineArg.Input && !File.Exists(enumMember.Args.First().Parameters.FirstOrDefault()))
					throw new FileNotFoundException("The input file was not found.");

				if (descriptionText.Length > 0)
				{
					documentation.AppendAndWrap(descriptionText.ToString(), documentationWidth, "\t\t\t");
					documentation.AppendLine();
				}
				#endregion

				#region Argument Attributes
				var paramters = enumMember.Member.GetAttributes<ParamterAttribute>();
				if (displayHelp && paramters != null && paramters.Length > 0)
				{
					documentation.AppendAndWrap("Argument Parameter(s):", documentationWidth, "\t\t\t");
					
					foreach (var paramter in paramters)
					{
						documentation.AppendAndWrap("Format: " + paramter.Format, documentationWidth, "\t\t\t\t");
						documentation.AppendAndWrap("Optional: " + paramter.Optional, documentationWidth, "\t\t\t\t");
						documentation.AppendAndWrap("Description:", documentationWidth, "\t\t\t\t");
						documentation.AppendAndWrap(paramter.Description, documentationWidth, "\t\t\t\t\t");
						documentation.AppendLine();
					}
				}
				documentation.AppendLine();
				#endregion
			}

			if (displayHelp)
			{
				Log.Info(documentation);
				return false;
			}

			if (args.Count == 0) throw new ArgumentException("No arguments were provided.");

			return true;
		}

		public IFilterPipe FilterPipe
		{
			get;
			set;
		}
		public ILogger Log
		{
			get;
			set;
		}
		public void ParseArgs(string[] args)
		{
			Log.Info("Parsing Input");

			var arguments = parse(args);
			
			if (!validate(arguments)) return;

			FilterPipe.ParseArguments(arguments);

			FilterPipe.Process(arguments);
		}
	}
}
