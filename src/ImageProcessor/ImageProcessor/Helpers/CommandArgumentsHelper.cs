using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.IO;
using System.Linq;
using Castle.Core.Internal;
using ImageProcessor.Attributes;
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

			if (arguments.Count == 0) throw new ArgumentException("No arguments were provided.");

			return arguments;
		}
		private void validate(List<CommandLineArgModel> args)
		{
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
				if (enumMember.Member.GetAttribute<RequiredAttribute>() != null &&
					enumMember.Args.Length == 0)
					throw new ArgumentException(String.Format("The -{0} argument is required.", enumMember.Value));

				if (enumMember.Member.GetAttribute<OnlyOneAttribute>() != null &&
					enumMember.Args.Length > 1)
					throw new ArgumentException(String.Format("Only one -{0} argument is allowed.", enumMember.Value));

				var range = enumMember.Member.GetAttribute<RangeAttribute>();
				if (range != null && enumMember.Args.Length > 0 &&
					enumMember.Args.Any(arg => arg.Parameters.Length < (int)range.Minimum || arg.Parameters.Length > (int)range.Maximum))
					throw new ArgumentException(range.ErrorMessage);

				var regex = enumMember.Member.GetAttribute<RegexAttribute>();
				if (regex != null && 
					enumMember.Args.Length > 0 &&
					enumMember.Args.SelectMany(arg => arg.Parameters).Any(param => !regex.IsValid(param)))
					throw new ArgumentException(String.Format("The -{0} argument paramters are invalid.", enumMember.Value));

				if (enumMember.Value == CommandsLineArg.Input && !File.Exists(enumMember.Args.First().Parameters.FirstOrDefault()))
					throw new FileNotFoundException("The input file was not found.");
			}
		}

		public IEffectPipe EffectPipe
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
			
			validate(arguments);

			EffectPipe.ParseArguments(arguments);

			EffectPipe.Process(arguments);
		}
	}
}
