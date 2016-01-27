using System.Drawing;
using ImageProcessor.Models;

namespace ImageProcessor
{
	public interface IEffectBase
	{
		Bitmap ProcessInput(CommandLineArgModel arg, Bitmap image);
		CommandsLineArg Argument
		{
			get;
		}
		object ParseInput(CommandLineArgModel arg);
	}
	public interface IEffect<TModel> : IEffectBase
		where TModel : class
	{
		Bitmap Process(TModel arg, Bitmap image);
		TModel Parse(CommandLineArgModel arg);
	}
	public abstract class EffectBase<TModel> : IEffect<TModel>
		where TModel : class
	{
		public Bitmap ProcessInput(CommandLineArgModel arg, Bitmap image)
		{
			return Process((TModel)arg.ParsedModel, image);
		}
		public abstract CommandsLineArg Argument
		{
			get;
		}
		public abstract Bitmap Process(TModel arg, Bitmap image);
		public abstract TModel Parse(CommandLineArgModel arg);
		public object ParseInput(CommandLineArgModel arg)
		{
			return Parse(arg);
		}
	}
}
