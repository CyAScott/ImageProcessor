using Castle.Core;
using Castle.MicroKernel;
using Castle.MicroKernel.ComponentActivator;
using Castle.MicroKernel.Context;
using Castle.MicroKernel.Registration;
using Castle.MicroKernel.SubSystems.Configuration;
using Castle.Windsor;
using NLog;

namespace ImageProcessor.Installers
{
	public class LogInstaller : IWindsorInstaller
	{
		public void Install(IWindsorContainer container, IConfigurationStore store)
		{
			container.Register(Component
				.For<ILogger>()
				.Activator<LoggerActivator>()
				.LifestyleTransient());
		}
	}
	public class LoggerActivator : AbstractComponentActivator
	{
		public LoggerActivator(
			ComponentModel model,
			IKernel kernel,
			ComponentInstanceDelegate onCreation,
			ComponentInstanceDelegate onDestruction)
			: base(model, (IKernelInternal)kernel, onCreation, onDestruction)
		{
		}

		protected override object InternalCreate(CreationContext context)
		{
			var type = context.Handler.ComponentModel.Implementation;
			return LogManager.GetLogger(type.Name, type);
		}

		protected override void InternalDestroy(object instance)
		{
		}
	}
}
