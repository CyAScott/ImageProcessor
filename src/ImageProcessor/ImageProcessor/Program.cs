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
		}
	}
}
