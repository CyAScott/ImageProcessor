namespace ImageProcessor.Models
{
	public class CommandLineArgModel
	{
		public CommandsLineArg Argument
		{
			get;
			set;
		}
		public object ParsedModel
		{
			get;
			set;
		}
		public string[] Parameters
		{
			get;
			set;
		}
	}
}
