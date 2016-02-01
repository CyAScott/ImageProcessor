using ImageProcessor.Models;

namespace ImageProcessor
{
	public interface IFilterBase
	{
		RawImage ProcessInput(CommandLineArgModel arg, RawImage image);
		CommandsLineArg Argument
		{
			get;
		}
		object ParseInput(CommandLineArgModel arg);
	}
	public interface IFilter<TModel> : IFilterBase
		where TModel : class
	{
		RawImage Process(TModel arg, RawImage image);
		TModel Parse(CommandLineArgModel arg);
	}
	public abstract class FilterBase<TModel> : IFilter<TModel>
		where TModel : class
	{
		public RawImage ProcessInput(CommandLineArgModel arg, RawImage image)
		{
			return Process((TModel)arg.ParsedModel, image);
		}
		public abstract CommandsLineArg Argument
		{
			get;
		}
		public abstract RawImage Process(TModel arg, RawImage image);
		public abstract TModel Parse(CommandLineArgModel arg);
		public object ParseInput(CommandLineArgModel arg)
		{
			return Parse(arg);
		}
	}
}
