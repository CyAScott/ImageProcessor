using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using ImageProcessor.Attributes;
using ImageProcessor.Filters;
using ImageProcessor.Models;
using NLog;

namespace ImageProcessor.Helpers
{
	public interface IFilterPipe
	{
		void ParseArguments(List<CommandLineArgModel> args);
		void Process(List<CommandLineArgModel> args);
	}
	[LoggerName("Filter Pipe")]
	public class FilterPipe : IFilterPipe
	{
		private class filterIteration
		{
			public CommandLineArgModel FilterParams
			{
				get;
				set;
			}
			public IFilterBase Filter
			{
				get;
				set;
			}
		}
		private IEnumerable<filterIteration> getFilterPipe(IEnumerable<CommandLineArgModel> args)
		{
			return args
				.Select(arg => new filterIteration
				{
					Filter = Filters.FirstOrDefault(filter => filter.Argument == arg.Argument),
					FilterParams = arg
				})
				.Where(arg => arg.Filter != null);
		}
		private void saveFile(List<CommandLineArgModel> args, filterIteration[] filters, RawImage image)
		{
			var output = args.First(arg => arg.Argument == CommandsLineArg.Output).Parameters.First();
			var extention = (Path.GetExtension(output) ?? "").ToLower();

			var isGrayscale = filters.Length > 0 && filters.Last().FilterParams.Argument == CommandsLineArg.Grayscale;
			var isBlackAndWhite = filters.Length > 0 && filters.Last().FilterParams.Argument == CommandsLineArg.ThresholdFilter;

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

		public IFilter<GrayscaleModel> Grayscale
		{
			get;
			set;
		}
		public IFilter<ThresholdFilterModel> ThresholdFilter
		{
			get;
			set;
		}
		public IFilterBase[] Filters
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
			foreach (var arg in getFilterPipe(args))
				arg.FilterParams.ParsedModel = arg.Filter.ParseInput(arg.FilterParams);
		}
		public void Process(List<CommandLineArgModel> args)
		{
			var inputFile = args.First(arg => arg.Argument == CommandsLineArg.Input).Parameters.First();
			var inputImage = new RawImage(inputFile);
			
			var filters = getFilterPipe(args).ToArray();

			var currentImage = inputImage;
			foreach (var arg in filters)
			{
				var roi = new RoiModel(currentImage);
				if (arg.FilterParams.ParsedModel is IRoiModel)
				{
					(arg.FilterParams.ParsedModel as IRoiModel).DefaultRoi = roi;
					roi = (arg.FilterParams.ParsedModel as IRoiModel).Roi ?? roi;
				}

				Log.Info("Processing: -{0} {1}", arg.FilterParams.Argument, String.Join(" ", arg.FilterParams.Parameters.Select(param => String.Format("\"{0}\"", param))));
				if (arg.FilterParams.ParsedModel is IRoiModel) Log.Info("Roi: " + roi.Region);

				var nextImage = arg.Filter.ProcessInput(arg.FilterParams, currentImage);

				currentImage = nextImage;
			}

			saveFile(args, filters, currentImage);
		}
	}
}
