using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;

namespace ImageProcessor.Helpers
{
	public interface ICommandArgumentsHelper
	{
		Dictionary<CommandsWithAnArgument, string> CommandAndArguments
		{
			get;
			set;
		}
		HashSet<CommandsWithOutAnArgument> Commands
		{
			get;
			set;
		}
		void ParseArgs(string[] args);
	}
	public class CommandArgumentsHelper : ICommandArgumentsHelper
	{
		public CommandArgumentsHelper()
		{
			CommandAndArguments = new Dictionary<CommandsWithAnArgument, string>();
			Commands = new HashSet<CommandsWithOutAnArgument>();
		}
		public Dictionary<CommandsWithAnArgument, string> CommandAndArguments
		{
			get;
			set;
		}
		public HashSet<CommandsWithOutAnArgument> Commands
		{
			get;
			set;
		}
		public IImageParser ImageParser
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
		public override string ToString()
		{
			return String.Join(" ",
				Commands.Select(command => String.Format("-{0}", command))
				.Concat(CommandAndArguments.Select(command => String.Format("-{0} \"{1}\"", command.Key, command.Value))));
		}
		public void ParseArgs(string[] args)
		{
			for (var index = 0; index < args.Length; index++)
			{
				CommandsWithAnArgument commandsWithAnArgument;
				CommandsWithOutAnArgument commandsWithOutAnArgument;

				if (Enum.TryParse(args[index].TrimStart('-'), true, out commandsWithAnArgument))
				{
					if (index == args.Length - 1) throw new ArgumentException(String.Format("The {0} command requires an argument.", commandsWithAnArgument));
					index++;
					CommandAndArguments[commandsWithAnArgument] = args[index];
				}
				else if (Enum.TryParse(args[index].TrimStart('-'), true, out commandsWithOutAnArgument))
				{
					Commands.Add(commandsWithOutAnArgument);
				}
				else
				{
					throw new ArgumentException("The following command was not recognized: " + args[index]);
				}
			}

			if (!CommandAndArguments.ContainsKey(CommandsWithAnArgument.Input)) throw new ArgumentException("Missing the input file.");
			if (!CommandAndArguments.ContainsKey(CommandsWithAnArgument.Output)) throw new ArgumentException("Missing the output file.");

			Locations.Input = CommandAndArguments[CommandsWithAnArgument.Input];
			if (!File.Exists(Locations.Input)) throw new ArgumentException("The input file is missing.");

			Locations.Output = CommandAndArguments[CommandsWithAnArgument.Output];
			if (File.Exists(Locations.Output)) File.Delete(Locations.Output);

			Bitmap inputImage;
			if (!ImageParser.TryParse(Locations.Input, out inputImage)) throw new InvalidProgramException("Unable to read the image.");
			Locations.InputImage = Locations.NewImage = inputImage;

			ProcessImage();
			
			if (Locations.NewImage == null) throw new InvalidProgramException("Failed to generate an image.");

			ImageParser.SaveOutput(this);
		}

		public void ProcessImage()
		{
			Locations.NewImage = Locations.InputImage;

			if (CommandAndArguments.ContainsKey(CommandsWithAnArgument.Scale))
			{
				double scale;
				if (!Double.TryParse(CommandAndArguments[CommandsWithAnArgument.Scale], out scale))
					throw new ArgumentException("The Scale argument must be a number between 0 and 1.");
				if (scale < 0) throw new ArgumentException("The scale value must be >= 0.");

				Processor.Scale(scale);
			}

			if (Commands.Contains(CommandsWithOutAnArgument.Grayscale))
			{
				Processor.Grayscale();
			}

			if (CommandAndArguments.ContainsKey(CommandsWithAnArgument.ThresholdFilter))
			{
				double threshold;
				if (!Double.TryParse(CommandAndArguments[CommandsWithAnArgument.ThresholdFilter], out threshold))
					throw new ArgumentException("The ThresholdFilter argument must be a number between 0 and 1.");
				if (threshold < 0)
					threshold = 0;
				else if (threshold > 1)
					threshold = 1;
				Processor.ThresholdFilter(threshold);
			}
		}
	}
	public enum CommandsWithAnArgument
	{
		Input,
		Output,

		Scale,
		ThresholdFilter
	}
	public enum CommandsWithOutAnArgument
	{
		Grayscale
	}
}
