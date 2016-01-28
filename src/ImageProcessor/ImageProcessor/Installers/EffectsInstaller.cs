using Castle.MicroKernel.Registration;
using Castle.MicroKernel.SubSystems.Configuration;
using Castle.Windsor;

namespace ImageProcessor.Installers
{
	public class EffectsInstaller : IWindsorInstaller
	{
		public void Install(IWindsorContainer container, IConfigurationStore store)
		{
			container.Register(Classes
				.FromThisAssembly()
				.InNamespace("ImageProcessor.Effects")
				.WithServiceAllInterfaces()
				.LifestyleSingleton());

			container.Register(Component
				.For<IEffectBase[]>()
				.Instance(container.ResolveAll<IEffectBase>())
				.LifestyleSingleton());
		}
	}
}
