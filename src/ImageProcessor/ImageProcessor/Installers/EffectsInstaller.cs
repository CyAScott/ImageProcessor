using Castle.MicroKernel.Registration;
using Castle.MicroKernel.SubSystems.Configuration;
using Castle.Windsor;

namespace ImageProcessor.Installers
{
	public class FiltersInstaller : IWindsorInstaller
	{
		public void Install(IWindsorContainer container, IConfigurationStore store)
		{
			container.Register(Classes
				.FromThisAssembly()
				.InNamespace("ImageProcessor.Filters")
				.WithServiceAllInterfaces()
				.LifestyleSingleton());

			container.Register(Component
				.For<IFilterBase[]>()
				.Instance(container.ResolveAll<IFilterBase>())
				.LifestyleSingleton());
		}
	}
}
