using Castle.MicroKernel.Registration;
using Castle.MicroKernel.SubSystems.Configuration;
using Castle.Windsor;
using RavuAlHemio.PbmNet;
using Component = Castle.MicroKernel.Registration.Component;

namespace ImageProcessor.Installers
{
	public class HelperInstaller : IWindsorInstaller
	{
		public void Install(IWindsorContainer container, IConfigurationStore store)
		{
			container.Register(Classes
				.FromThisAssembly()
				.InNamespace("ImageProcessor.Helpers")
				.WithServiceAllInterfaces()
				.LifestyleSingleton());

			container.Register(
				Component
					.For<ImageFactories.Image8Factory>()
					.Instance(new ImageFactories.Image8Factory())
					.LifestyleSingleton(),
				Component
					.For<ImageFactories.Image16Factory>()
					.Instance(new ImageFactories.Image16Factory())
					.LifestyleSingleton(),
				Component
					.For<ImageFactories.Image32Factory>()
					.Instance(new ImageFactories.Image32Factory())
					.LifestyleSingleton());
		}
	}
}
