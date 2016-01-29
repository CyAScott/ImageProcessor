using System;
using Castle.Windsor;
using Castle.Windsor.Installer;
using ImageProcessor.Helpers;
using NLog;

namespace ImageProcessor
{
	public static class Program
	{
		public static void Main(string[] args)
		{
			var log = LogManager.GetLogger("Image Processor", typeof(Program));
			try
			{
				//args = new[]
				//{
				//	"-Input", @"D:\Samples\Sample Images\Color\11.jpg", 
				//	"-Output", @"D:\output.png",
				//	//"-Median", "window:101"//, "y:false",
				//	"-Median", "Window:101"
				//};

				var container = new WindsorContainer();

				container.Install(FromAssembly.This());

				var helper = container.Resolve<ICommandArgumentsHelper>();

				helper.ParseArgs(args);

				log.Info("Done");
			}
			catch (Exception error)
			{
				log.Error(error, error.Message);
			}
			log.Info("Press the enter key to exit.");
			Console.ReadLine();
		}
	}
}
