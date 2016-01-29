using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using ImageProcessor.Attributes;
using ImageProcessor.Effects;
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
		private void saveFile(List<CommandLineArgModel> args, effectInteration[] effects, RawImage image)
		{
			var output = args.First(arg => arg.Argument == CommandsLineArg.Output).Parameters.First();
			var extention = (Path.GetExtension(output) ?? "").ToLower();

			var isGrayscale = effects.Length > 0 && effects.Last().EffectParams.Argument == CommandsLineArg.Grayscale;
			var isBlackAndWhite = effects.Length > 0 && effects.Last().EffectParams.Argument == CommandsLineArg.ThresholdFilter;

			switch (extention)
			{
				case ".pbm":
					if (!isBlackAndWhite)
						image = ThresholdFilter.Process(new ThresholdFilterModel
						{
							DefaultRoi = new RoiModel(image),
							Threshold = 0.5
						},
						image);

					break;
				case ".pgm":
					if (!isGrayscale && !isBlackAndWhite)
						image = Grayscale.Process(new GrayscaleModel
						{
							DefaultRoi = new RoiModel(image)
						},
						image);
					break;
			}

			if (File.Exists(output)) File.Delete(output);

			image.Save(output);
		}

		public IEffect<GrayscaleModel> Grayscale
		{
			get;
			set;
		}
		public IEffect<ThresholdFilterModel> ThresholdFilter
		{
			get;
			set;
		}
		public IEffectBase[] Effects
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
			var inputImage = new RawImage(inputFile);
			
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
				if (arg.EffectParams.ParsedModel is IRoiModel) Log.Info("Roi: " + roi.Region);

				var nextImage = arg.Effect.ProcessInput(arg.EffectParams, currentImage);

				currentImage = nextImage;
			}

			saveFile(args, effects, currentImage);
		}
	}
}
