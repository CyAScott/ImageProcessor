using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using ImageProcessor.Attributes;
using ImageProcessor.Models;
using NLog;

namespace ImageProcessor.Helpers
{
	public interface IEffectPipe
	{
		void ParseArguments(List<CommandLineArgModel> args);
		void Process(List<CommandLineArgModel> args);
	}
	[LoggerName("Effect Pipe")]
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
		public IImageIo ImageIo
		{
			get;
			set;
		}
		public ILogger Log
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
			if (!ImageIo.TryParse(inputFile, out inputImage)) throw new ArgumentException("Unable to read the input image.");

			var effects = getEffectPipe(args).ToArray();

			var currentImage = inputImage;
			foreach (var arg in effects)
			{
				var roi = new RoiModel(currentImage);
				if (arg.EffectParams.ParsedModel is IRoiModel)
				{
					(arg.EffectParams.ParsedModel as IRoiModel).DefaultRoi = roi;
					roi = (arg.EffectParams.ParsedModel as IRoiModel).Roi ?? roi;
				}

				Log.Info("Processing: -{0} {1}", arg.EffectParams.Argument, String.Join(" ", arg.EffectParams.Parameters.Select(param => String.Format("\"{0}\"", param))));
				Log.Info("Roi: " + roi.Region);

				var nextImage = arg.Effect.ProcessInput(arg.EffectParams, currentImage);

				if (!ReferenceEquals(currentImage, nextImage)) currentImage.Dispose();

				currentImage = nextImage;
			}

			var output = args.First(arg => arg.Argument == CommandsLineArg.Output).Parameters.First();
			ImageIo.SaveOutput(
				currentImage, 
				effects.Length > 0 && effects.Last().EffectParams.Argument == CommandsLineArg.Grayscale,
				effects.Length > 0 && effects.Last().EffectParams.Argument == CommandsLineArg.ThresholdFilter,
				output);
		}
	}
}
