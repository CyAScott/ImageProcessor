using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using ImageProcessor.Models;

namespace ImageProcessor.Helpers
{
	public interface IEffectPipe
	{
		void ParseArguments(List<CommandLineArgModel> args);
		void Process(List<CommandLineArgModel> args);
	}
	public class EffectPipe : IEffectPipe
	{
		private class effectInteration
		{
			public CommandLineArgModel EffectParams
			{
				get;
				set;
			}
			public IEffectBase Effect
			{
				get;
				set;
			}
		}
		private IEnumerable<effectInteration> getEffectPipe(IEnumerable<CommandLineArgModel> args)
		{
			return args
				.Select(arg => new effectInteration
				{
					Effect = Effects.FirstOrDefault(effect => effect.Argument == arg.Argument),
					EffectParams = arg
				})
				.Where(arg => arg.Effect != null);
		}

		public IEffectBase[] Effects
		{
			get;
			set;
		}
		public IImageParser ImageParser
		{
			get;
			set;
		}
		public void ParseArguments(List<CommandLineArgModel> args)
		{
			foreach (var arg in getEffectPipe(args))
				arg.EffectParams.ParsedModel = arg.Effect.ParseInput(arg.EffectParams);
		}
		public void Process(List<CommandLineArgModel> args)
		{
			var inputFile = args.First(arg => arg.Argument == CommandsLineArg.Input).Parameters.First();
			Bitmap inputImage;
			if (!ImageParser.TryParse(inputFile, out inputImage)) throw new ArgumentException("Unable to read the input image.");

			var effects = getEffectPipe(args).ToArray();

			var currentImage = inputImage;
			foreach (var arg in effects)
			{
				if (arg.EffectParams.ParsedModel is IRoiModel)
					(arg.EffectParams.ParsedModel as IRoiModel).DefaultRoi = new RoiModel(currentImage);

				var nextImage = arg.Effect.ProcessInput(arg.EffectParams, currentImage);

				if (!ReferenceEquals(currentImage, nextImage)) currentImage.Dispose();

				currentImage = nextImage;
			}

			var output = args.First(arg => arg.Argument == CommandsLineArg.Output).Parameters.First();
			ImageParser.SaveOutput(
				currentImage, 
				effects.Length > 0 && effects.Last().EffectParams.Argument == CommandsLineArg.Grayscale,
				effects.Length > 0 && effects.Last().EffectParams.Argument == CommandsLineArg.ThresholdFilter,
				output);
		}
	}
}
